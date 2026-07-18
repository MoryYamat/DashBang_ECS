#pragma once
#include <cstdint>
#include <memory>
#include <string>
#include <string_view>
#include <optional>
#include <typeindex>
#include <unordered_map>
#include <vector>
#include <ddknd/asset/asset_id.h>

namespace ddknd::asset
{

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

    /**
    * Stores metadata and key-to-ID mappings for one asset tag type.
    */
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

                pathToId_.emplace(meta.vpath, id);
                metas_.push_back(std::move(meta));

                return id;
            }

            return it->second;
        }
        const AssetMeta* TryGetMeta(ID id) const
        {
            if (!id.IsValid())
            {
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
            if (!id.IsValid())
            {
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

    /**
    * Maps typed asset keys to stable AssetIDs and tracks their load state.
    * 
    * Resource objects are stored separately from this manager.
    */
    class AssetManager
    {
      public:
        /**
        * Returns the existing ID for vpath, or creates a new metadata entry.
        * Repeated calls with same vpath return the same ID.
        */
        template <typename Tag>
        AssetID<Tag> GetOrCreate(std::string_view vpath)
        {
            return Table<Tag>().GetOrCreate(vpath);
        }

        template <typename Tag>
        std::optional<std::string_view> TryPathOf(AssetID<Tag> id) const
        {
            const auto* meta = TryGetMeta(id);
            if(!meta)
            {
                return std::nullopt;
            }

            return std::string_view{meta->vpath};
        }

        template <typename Tag>
        std::optional<AssetState> TryStateOf(AssetID<Tag> id) const
        {
            const auto* meta = TryGetMeta(id);
            if(!meta)
            {
                return std::nullopt;
            }

            return meta->state;
        }

        template <typename Tag>
        bool SetState(AssetID<Tag> id, AssetState state)
        {
            auto* meta = TryGetMeta(id);
            if(!meta)
            {
                return false;
            }

            meta->state = state;
            return true;
        }

        template<typename Tag>
        const AssetMeta* TryGetMeta(AssetID<Tag> id) const
        {
            const auto* table = TryGetTable<Tag>();
            if(!table)
            {
                return nullptr;
            }
            return table->TryGetMeta(id);
        }

        template<typename Tag>
        AssetMeta* TryGetMeta(AssetID<Tag> id)
        {
            auto* table = TryGetTable<Tag>();
            if(!table)
            {
                return nullptr;
            }
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
            {
                return nullptr;
            }

            return static_cast<AssetTable<Tag>*>(it->second.get());
        }

        template <typename Tag>
        const AssetTable<Tag>* TryGetTable() const
        {
            const std::type_index key{typeid(Tag)};

            auto it = tables_.find(key);
            if (it == tables_.end())
            {
                return nullptr;
            }

            return static_cast<AssetTable<Tag>*>(it->second.get());
        }

        template <typename Tag>
        AssetTable<Tag>& Table()
        {
            if (auto* table = TryGetTable<Tag>())
            {
                return *table;
            }

            const std::type_index key{typeid(Tag)};

            auto table = std::make_unique<AssetTable<Tag>>();
            auto* ptr = table.get();
            tables_.emplace(key, std::move(table));

            return *ptr;
        }
    };

} // namespace ddknd::asset