#include <ddknd/fsm/axis_builder_registry.h>

#include <cstdint>

namespace ddknd::fsm
{
    AxisBuilder& AxisBuilderRegistry::GetOrCreateAxis(std::string_view axisName)
    {
        const auto name = std::string{axisName};
        const auto it = nameToId_.find(name);

        if(it == nameToId_.end())
        {
            const auto size = axisBuilders_.size();
            const AxisID id = AxisID{static_cast<std::uint32_t>(size)};
            nameToId_[name] = id;
            auto axisBuilder = std::make_unique<AxisBuilder>();
            axisBuilders_.push_back(std::move(axisBuilder));
            return *axisBuilders_[static_cast<std::size_t>(id.Value())];
        }

        const AxisID id = it->second;
        return *axisBuilders_[static_cast<std::size_t>(id.Value())];
    }

    
    AxisBuildResult AxisBuilderRegistry::BuildAllAxis()
    {
        std::vector<AxisDefinition> definitions;
        definitions.reserve(axisBuilders_.size());

        for(auto& builder : axisBuilders_)
        {
            definitions.push_back(std::move(*builder).Build());
        }
        axisBuilders_.clear();

        AxisBuildResult result(std::move(definitions));

        return result;
    }
}