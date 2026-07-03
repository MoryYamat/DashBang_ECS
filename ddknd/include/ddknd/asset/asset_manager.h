#pragma once
#include <cstdint>
#include <memory>
#include <string>
#include <string_view>
#include <optional>
#include <typeindex>
#include <unordered_map>
#include <vector>
#include <iostream>

#include "ddknd/core/StrongID.h"

namespace ddknd::asset
{
    template <typename Tag>
    using AssetID = ::ddknd::core::HandleID<Tag>;

    enum class AssetState : std::uint8_t
    {
        Unload,
        Loading,
        Loaded,
        Failed
    };

    struct AssetMeta
    {
        std::uint32_t generation = 0;
        AssetState state = AssetState::Unload;
        std::string vpath;
    };

    struct IAssetTable
    {
        virtual ~IAssetTable() = default;
    };

    template <typename Tag>
    class AssetTable final : public IAssetTable
    {
      public:
        using ID = AssetID<Tag>;

        ID GetOrCreate(std::string_view vpath)
        {
            const std::string key{vpath};
            auto it = pathToId_.find(key);
            if (it == pathToId_.end())
            {
                const std::uint32_t idx = static_cast<std::uint32_t>(metas_.size());

                AssetMeta meta{};
                meta.vpath = static_cast<std::string>(vpath);
                meta.generation = 0;

                ID id{meta.generation, idx};

                // @TODO allocator
                pathToId_.emplace(meta.vpath, id);
                metas_.push_back(std::move(meta));

                return id;
            }

            return it->second;
        }
        const AssetMeta* TryGetMeta(ID id) const
        {
            if (!id.Is_valid())
            {
                std::cerr << "[AssetTable::TryGetMeta]Invalid ID\n";
                return nullptr;
            }
                
            const auto idx = id.Index();
            if (idx >= metas_.size())
            {
                return nullptr;
            }

            const auto& meta = metas_[idx];
            if (meta.generation != id.Generation())
            {
                return nullptr;
            }
                
            return &metas_[idx];
        }
        AssetMeta* TryGetMeta(ID id)
        {
            if (!id.Is_valid())
            {
                std::cerr << "[AssetTable::TryGetMeta]Invalid ID\n";
                return nullptr;
            }
                
            const auto idx = id.Index();
            if (idx >= metas_.size())
            {
                return nullptr;
            }

            const auto& meta = metas_[idx];
            if (meta.generation != id.Generation())
            {

                return nullptr;
            }
                
            return &metas_[idx];
        }

      private:
        std::vector<AssetMeta> metas_;
        std::unordered_map<std::string, ID> pathToId_;
    };

    class AssetManager
    {
      public:
        template <typename Tag>
        AssetID<Tag> GetOrCreate(std::string_view vpath)
        {
            return Table<Tag>().GetOrCreate(vpath);// default scene
        }

        // extension
        // template <typename Tag>
        // AssetID<Tag> GetOrCreate(std::string_view vpath, int sceneIdx)
        // {
        //     return Table<Tag>().GetOrCreate(vpath);// default scene
        // }

        template <typename Tag>
        std::optional<std::string_view> TryPathOf(AssetID<Tag> id) const
        {
            const auto* meta = TryGetMeta(id);
            if(!meta)
            {
                std::cerr << "[AssetTable::TryPathOf] Failed to get meta information.\n";
                return std::nullopt;
            }

            return std::string_view{meta->vpath};
        }

        template <typename Tag>
        std::optional<AssetState> TryStateOf(AssetID<Tag> id) const
        {
            const auto* meta = TryGetMeta(id);
            if(!meta)
                return std::nullopt;

            return meta->state;
        }

        template <typename Tag>
        bool SetState(AssetID<Tag> id, AssetState state)
        {
            auto* meta = TryGetMeta(id);
            if(!meta)
                return false;
            meta->state = state;
            return true;
        }

        template<typename Tag>
        const AssetMeta* TryGetMeta(AssetID<Tag> id) const
        {
            const auto* table = TryGetTable<Tag>();
            if(!table)
                return nullptr;
            return table->TryGetMeta(id);
        }

        template<typename Tag>
        AssetMeta* TryGetMeta(AssetID<Tag> id)
        {
            auto* table = TryGetTable<Tag>();
            if(!table)
                return nullptr;
            return table->TryGetMeta(id);
        }

      private:
        std::unordered_map<std::type_index, std::unique_ptr<IAssetTable>> tables_;

        template <typename Tag>
        AssetTable<Tag>* TryGetTable()
        {
            const std::type_index key{typeid(Tag)};

            auto it = tables_.find(key);
            if (it == tables_.end())
                return nullptr;

            return static_cast<AssetTable<Tag>*>(it->second.get());
        }

        template <typename Tag>
        const AssetTable<Tag>* TryGetTable() const
        {
            const std::type_index key{typeid(Tag)};

            auto it = tables_.find(key);
            if (it == tables_.end())
                return nullptr;

            return static_cast<AssetTable<Tag>*>(it->second.get());
        }

        template <typename Tag>
        AssetTable<Tag>& Table()
        {
            if (auto* table = TryGetTable<Tag>())
                return *table;

            const std::type_index key{typeid(Tag)};

            auto table = std::make_unique<AssetTable<Tag>>();
            auto* ptr = table.get();
            tables_.emplace(key, std::move(table));

            return *ptr;
        }
    };

    template <typename T, typename Tag>
    class AssetStorage
    {
      public:
        using ID = ::ddknd::core::HandleID<Tag>;

        T* TryGet(ID id)
        {
            const auto idx = id.Index();
            if (idx >= data_.size())
                return nullptr;
            return &data_[idx];
        }

        const T* TryGet(ID id) const
        {
            const auto idx = id.Index();
            if (idx >= data_.size())
                return nullptr;
            return &data_[idx];
        }

        void Set(ID id, T value)
        {
            const auto idx = id.Index();
            if (idx >= data_.size())
            {
                data_.resize(idx + 1);
            }
            data_[idx] = std::move(value);
        }

      private:
        std::vector<T> data_;
    };
} // namespace ddknd::asset


// @TODO:
// LOW: 
// - asset_managerとvpath 解決の 境界と責務分離(AssetID<ModelTag> が一意に file + scene を指す)
//  課題:
//  - 現状: vpath-AssetID の直接対応(vpath内の違いを区別できない)
//  改善案:
//      AssetT::create_uri(vpath, options)
//          ↓
//      AssetID::GetOrCreateID(uri)(IR:similar to URI)
//          ↓
//      AssetT::parse_key(uri)
//          ↓
//      absPath + load options

// @TODO
// LOW:
// Editor機能との関連
// asset_id は ゲームオブジェクトの意味論(意味を持つ概念の区分)と対応するようにする
// そのために、asset_manager::create_asset_id(key)の入力である key は 事前に一意の識別子(uri)を生成する必要がある
// これはfile に含まれる sub asset のパース であり、ゲームエンジンのeditor機能に含まれる
// uriの仕様(案):
// - file + selector:
//      model key: res://foo.glb#scene=0 / res://foo.glb#animation=0 など