#pragma once

#include <ddknd/asset/asset_id.h>

#include <vector>
#include <utility>

namespace ddknd::asset
{
    /**
    * Stores asset resources by AssetID index.
    *
    * Maps typed asset keys to AssetIDs and tracks their load state.
    * through AssetManager before accessing a resource.
    */
    template <typename T, typename Tag>
    class AssetStorage
    {
      public:
        using ID = AssetID<Tag>;

        T* TryGet(ID id)
        {
            const auto idx = id.Index();
            if (idx >= data_.size())
            {
                return nullptr;
            }

            return &data_[idx];
        }

        const T* TryGet(ID id) const
        {
            const auto idx = id.Index();
            if (idx >= data_.size())
            {
                return nullptr;
            }
            return &data_[idx];
        }

        void Set(ID id, T value)
        {
            const auto idx = id.Index();
            
            /**
            * Resources are stored at their AssetID index.
            * Resizing may default-construct unused intermediate entries.
            * Callers must confirm asset validity and load state through AssetManager
            * before accessing the stored resource.
            */
            if (idx >= data_.size())
            {
                data_.resize(idx + 1);
            }
            data_[idx] = std::move(value);
        }

      private:
        std::vector<T> data_;
    };
}