#include <ddknd/ui/ui_context.h>

#include <ddknd/graphics/renderer_backend.h>

#include <ddknd/system/system.h>
#include <ddknd/graphics/renderer.h>
#include <ddknd/graphics/gfx_asset_loader.h>

namespace ddknd::ui
{
    UIContext::UIContext(graphics::IRendererBackend& backend) : backend_(backend)
    {
        batch_ = backend_.CreateScreenQuadBatchWithoutTexture();
    };

    UIContext::~UIContext()
    {
        if(batch_.IsValid())
        {
            backend_.DestroyScreenQuadBatch(batch_);
        }
    }

    void UIContext::RegisterShpae(UIShape shape)
    {
        shapes_.push_back(shape);
    }
    void UIContext::RegisterShader(::ddknd::asset::AssetID<::ddknd::asset::tag::Shader> shader)
    {
        shader_ = shader;
    }

    std::vector<UIShape>& UIContext::GetShapes()
    {
        return shapes_;
    }

    void UIContext::BuildUIVertices()
    {
        vertices_.clear();
        indices_.clear();

        for(auto& shape : shapes_)
        {

            const float x0 = shape.position.x();
            const float y0 = shape.position.y();
            const float x1 = x0 + shape.width;
            const float y1 = y0 + shape.height;

            const auto base = static_cast<std::uint32_t>(vertices_.size());


            vertices_.push_back({{x0, y0}, {0.f, 0.f}, shape.color});
            vertices_.push_back({{x1, y0}, {1.0f, 0.0f}, shape.color});
            vertices_.push_back({{x1, y1}, {1.0f, 1.0f}, shape.color});
            vertices_.push_back({{x0, y1}, {0.0f, 1.0f}, shape.color});

            indices_.push_back(base + 0);
            indices_.push_back(base + 1);
            indices_.push_back(base + 2);

            indices_.push_back(base + 0);
            indices_.push_back(base + 2);
            indices_.push_back(base + 3);
        }
    }

    void UIContext::BeginFrame(ddknd::system::FrameContext& frame)
    {
        vertices_.clear();
        indices_.clear();
    }

    void UIContext::FlushUI(){
        BuildUIVertices();
        backend_.UpdateScreenQuadBatch(batch_, vertices_, indices_);
        // std::cerr << "ui flushed\n";
    }

    std::uint32_t UIContext::UIIndicesCount()
    {
        return static_cast<std::uint32_t>(indices_.size());
    }

    void UIContext::EndFrame(ddknd::system::FrameContext& frame)
    {
        FlushUI();
        if(!frame.renderer || !frame.graphicsAssetStore)
        {
            return;
        }
        const auto* uiShader = frame.graphicsAssetStore->TryGet(shader_);
        frame.renderer->Submit(ddknd::graphics::UIDrawCommand{.batch = batch_, .shader = uiShader->program, .indexCount = UIIndicesCount()});
    }

    graphics::types::GPUID<graphics::tag::ScreenQuadBatchTag> UIContext::Batch() const
    {
        return batch_;
    }

    std::uint32_t UIContext::IndexCount() const
    {
        return static_cast<std::uint32_t>(indices_.size());
    }

    const std::vector<UIShape>& UIContext::GetShapes() const
    {
        return shapes_;
    }
}