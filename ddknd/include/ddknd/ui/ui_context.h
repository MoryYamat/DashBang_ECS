#pragma once

#include <ddknd/ui/shape.h>
#include <ddknd/system/system_fwd.h>

#include <ddknd/asset/asset_tag.h>
#include <ddknd/graphics/graphics_fwd.h>
#include <ddknd/graphics/type/gfx_resource_types.h>

#include <ddknd/math/math.h>

#include <vector>
#include <cstdint>


namespace ddknd::ui
{
    class UIContext
    {
        public:
            UIContext(graphics::IRendererBackend& backend);
            ~UIContext();

            void RegisterShpae(UIShape shape);
            void RegisterShader(::ddknd::asset::AssetID<::ddknd::asset::tag::Shader> shader);
            
            void BeginFrame(ddknd::system::FrameContext& frame);
            void EndFrame(ddknd::system::FrameContext& frame);

            std::vector<UIShape>& GetShapes();
            const std::vector<UIShape>& GetShapes() const;

            graphics::types::GPUID<graphics::tag::ScreenQuadBatchTag> Batch() const;
            std::uint32_t IndexCount() const;

        private:
            void BuildUIVertices();
            void FlushUI();
            std::uint32_t  UIIndicesCount();

        private:

            std::vector<UIShape> shapes_{};

            graphics::IRendererBackend& backend_;
            ::ddknd::asset::AssetID<::ddknd::asset::tag::Shader> shader_;
            graphics::types::GPUID<graphics::tag::ScreenQuadBatchTag> batch_;
            std::vector<graphics::types::ScreenQuadVertex> vertices_;
            std::vector<std::uint32_t> indices_;

    };
}