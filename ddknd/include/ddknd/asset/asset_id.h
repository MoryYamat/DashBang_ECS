#pragma once

#include "ddknd/core/StrongID.h"

namespace ddknd::asset
{
    template <typename Tag>
    using AssetID = ::ddknd::core::HandleID<Tag>;

}// namespace ddknd::asset