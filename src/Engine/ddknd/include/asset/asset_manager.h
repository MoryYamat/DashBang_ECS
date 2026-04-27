#pragma once
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>

#include "core/StrongID.h"


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
        AssetState state = AssetState::Unload;
        std::string vpath;
    };

    template <typename Tag>
    class AssetTable
    {
        public:
        AssetID<Tag> GetOrCreate(std::string_view vpath);
        const AssetMeta* TryGetMeta(AssetID<Tag> id) const;
        AssetMeta* TryGetMeta(AssetID<Tag> id);
        private:
         std::vector<AssetMeta> metas_;
         std::unordered_map<std::string, AssetID<Tag>> pathToId_;
    };

    class AssetManager
    {
      public:
        template<typename Tag>
        AssetID<Tag> GetOrCreate(std::string_view vpath)
        {
            return Table<Tag>().GetOrCreate(vpath);
        }
        
        template <typename Tag>
        std::string_view PathOf(AssetID<Tag> id) const
        {
            return Table<Tag>().TryGetMeta(id)->vpath;
        }

        template <typename Tag>
        AssetState StateOf(AssetID<Tag> id) const
        {
            return Table<Tag>().TryGetMeta(id)->state;
        }

        template <typename Tag>
        void SetState(AssetID<Tag> id, AssetState state)
        {
            Table<Tag>().TryGetMeta(id)->state = state;
        }

      private:

        template<typename Tag>
        AssetTable<Tag>& Table();

        template<typename Tag>
        const AssetTable<Tag>& Table()const;
    };
    
    template<typename T, typename Tag>
    class Storage
    {
        public:
            using ID = ::ddknd::core::HandleID<Tag>;

            T* TryGet(ID id)
            {
                const auto idx = id.Index();
                if(idx >= data_.size()) return nullptr;
                return &data_[idx];
            }

            const T* TryGet(ID id) const
            {
                const auto idx = id.Index();
                if(idx >= data_.size()) return nullptr;
                return &data_[idx];
            }

            void Set(ID id, T value)
            {
                const auto idx = id.Index();
                if(idx >= data_.size())
                {
                    data_.resize(idx + 1);
                }
                data_[idx] = std::move(value);
            }

        private:
            std::vector<T> data_;
    };
} // namespace ddknd::asset