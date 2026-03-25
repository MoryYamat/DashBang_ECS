#pragma once
/* Asset の基本構造
1. Game/Sceneの初期化
    - `handle = AssetManager::Get(vpath)`
    - 内部で`GetOrCreate`によりvpath -> ID を確定
    - AssetID系は論理的識別子(水平方向)となる(ドメインを分割)

2. ロード(任意: 一括/逐次/非同期)
    - `AssetManager::LoadNow(handle)` または`RequestLoad(handle)`
    - 内部でvpath 解決 -> 読み込み -> Resource生成 -> SetLoaded

3. GameAssets / SceneAssets が handle/IDを保持
    - 以降vpath は触らない

4. EntityのComponentはIDを保持して使用
    - 毎フレームは`ID -> Resource`をO(1)で引く
*/

/* 命名規則

0. Tag
    - Asset論理ID(図書館の蔵書管理IDみたいなもの)
    - Resource の load 状況 にかかわらず、型付きID を Handle<Tag>::id_ として 返すことができる
    - Resource とは 時間軸と責務が違う ので 分離

1. Asset (外部定義)(実際の本(と同じ寿命を持つもの))
    - vpath で指すもの(shader descriptor/ .vert/.fragなど)
    - 変更・差し替え・Mod対象

2. Resource (実体リソースへのポインタ(ID)(本のカバーみたいなもの))
    - GpuIDやAudioIDなどドメインの中でのリソース識別子(垂直方向)を保持(発行は各ドメインシステム)(GpuID:
`program`/`vao`/`vbo`...)
    - メタ情報 (vertextCount, などのメタ情報)
    - 破棄やキャッシュ

3. Ref 使用側オブジェクト(参照)
    - Entity/Renderer が 持つのはID/handle
    - 実体は`TrygGet`して使うだけ


*/

#include "core/StrongID.hpp"

#include <cstdint>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>

// fwd
namespace ddknd::io
{
    class IPathResolver;
} // namespace ddknd::io

namespace ddknd::renderer
{
    class IRendererBackend;
}

// content
namespace ddknd::asset::type
{
    // tags
    struct ShaderAssetTag
    {
    };
    struct MeshAssetTag
    {
    };

    template <typename Tag>
    using ID = ::ddknd::core::HandleID<Tag>;

    // resources
    struct ShaderResource
    {
    };

    struct MeshResource
    {
    };

    // asset traits
    template <typename Resource>
    struct AssetTraits;

    template <>
    struct AssetTraits<ShaderResource>
    {
        using ID_type = ShaderAssetTag;
    };

    template <>
    struct AssetTraits<MeshResource>
    {
        using ID_type = MeshAssetTag;
    };
} // namespace ddknd::asset::type

namespace ddknd::asset
{

    template <typename Tag, typename Resource>
    class AssetStorage
    {
      private:
        enum class AssetState : std::uint8_t
        {
            Unload,
            Loading,
            Loaded,
            Failed
        };

        struct AssetHeader
        {
            explicit AssetHeader(std::string_view vpath) : vpath(std::string(vpath)) {}
            AssetState state = AssetState::Unload;
            std::string vpath;
        };

        template <typename T>
        struct AssetRecordT
        {
            explicit AssetRecordT(AssetHeader h) : h(h){}
            AssetHeader h;
            std::unique_ptr<T> data;
        };

      public:
        using ID = core::HandleID<Tag>;

        ID GetOrCreate(std::string_view vpath)
        {
            auto it = vpath_to_id_.find(std::string(vpath));
            if (it != vpath_to_id_.end())
                return it->second;

            // new slot
            const auto idx = records_.size();
            // TODO: need to consider generation transition
            ID id{static_cast<std::uint32_t>(idx), 0u};

            AssetRecordT<Resource> rec{AssetHeader(vpath)};
            records_.push_back(std::move(rec));
            vpath_to_id_.emplace(vpath, id);
            return id;
        }

        const Resource* TryGetLoaded(ID id) const
        {
            const auto idx = static_cast<std::size_t>(id.Index());
            if (idx >= records_.size())
                return nullptr;

            const auto& rec = records_[idx];
            if (rec.h.state != AssetState::Loaded)
                return nullptr;
            return rec.data.get();
        }

        const AssetHeader* TryGetHeader(ID id) const
        {
            const auto idx = static_cast<std::size_t>(id.Index());
            if (idx >= records_.size())
                return nullptr;
            return &records_[idx].h;
        }

        AssetState GetState(ID id) const
        {
            if (auto* h = TryGetHeader(id))
                return h->state;
            return AssetState::Failed;
        }

        bool TryMarkLoading(ID id)
        {
            const auto idx = static_cast<std::size_t>(id.Index());
            if (idx >= records_.size())
                return false;
            auto& h = records_[idx].h;
            if (h.state == AssetState::Loading || h.state == AssetState::Loaded)
                return false;
            h.state = AssetState::Loading;
            return true;
        }

        void SetFailed(ID id)
        {
            const auto idx = static_cast<std::size_t>(id.Index());
            if (idx >= records_.size())
                return;
            records_[idx].data.reset();
            records_[idx].data.h.state = AssetState::Failed;
        }

        void SetLoaded(ID id, std::unique_ptr<Resource> r) const
        {
            const auto idx = static_cast<std::size_t>(id.Index());
            if (idx >= records_.size())
                return;
            auto& rec = records_[idx];
            rec.data = std::move(r);
            rec.h.state = rec.data ? AssetState::Loaded : AssetState::Failed;
        }

      private:
        std::vector<AssetRecordT<Resource>> records_;
        std::unordered_map<std::string, ID> vpath_to_id_;
    };

    // Asset ID Handle
    template <typename Tag>
    class AssetHandle
    {
      public:
        using ID = core::HandleID<Tag>;

        AssetHandle() = default;
        AssetHandle(ID id) : id_(id) {}

        ID Id() const noexcept
        {
            return id_;
        }
        bool Is_valid(ID id)
        {
            return id_.Is_valid();
        }

      private:
        ID id_;
    };

    // Asset API
    class AssetManager
    {
      public:
        template <typename Resource>
        using Tag_Of = typename type::AssetTraits<Resource>::ID_type;

        template <typename Resource>
        using ID = core::HandleID<Tag_Of<Resource>>;

        template <typename Resource>
        AssetHandle<Tag_Of<Resource>> GetOrCreate(std::string_view vpath)
        {
            auto& storage = Storage<Tag_Of<Resource>, Resource>();
            const auto id = storage.GetOrCreate(vpath);
            return {id};
        }

        template <typename Resource>
        const Resource* TryGet(ID<Resource> id) const
        {
            const auto& storage = Storage<Tag_Of<Resource>, Resource>();
            return storage.TryGetLoaded(id);
        }

        // bool LoadNowShader(ID<type::ShaderResource> id);
        // bool LoadNowMesh(ID<type::MeshResource> id);

      private:
        AssetStorage<type::ShaderAssetTag, type::ShaderResource> shaders_;
        AssetStorage<type::MeshAssetTag, type::MeshResource> meshes_;

        template <typename Tag, typename Resource>
        AssetStorage<Tag, Resource>& Storage();

        template <typename Tag, typename Resource>
        const AssetStorage<Tag, Resource>& Storage() const;

        // specialized
        template <>
        AssetStorage<type::ShaderAssetTag, type::ShaderResource>& Storage()
        {
            return shaders_;
        }

        template <>
        const AssetStorage<type::ShaderAssetTag, type::ShaderResource>& Storage() const
        {
            return shaders_;
        }

        template <>
        AssetStorage<type::MeshAssetTag, type::MeshResource>& Storage()
        {
            return meshes_;
        }

        template <>
        const AssetStorage<type::MeshAssetTag, type::MeshResource>& Storage() const
        {
            return meshes_;
        }
    };
} // namespace ddknd::asset