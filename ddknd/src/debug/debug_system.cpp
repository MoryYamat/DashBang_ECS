#include <ddknd/debug/debug_system.h>

#include <ddknd/ecs/ecs.h>
#include <ddknd/system/system.h>

#include <ddknd/component/gfx_component.h>
#include <ddknd/graphics/debug_animation.h>
#include <ddknd/graphics/gfx_asset_loader.h>

namespace ddknd::debug
{
    void DebugSystemRunner::Update(::ddknd::ecs::World& world, const DebugContext& ctx)
    {
        assert(ctx.frame);
        assert(ctx.debugDraw);
        assert(ctx.config);

        if (ctx.config->drawSkeletons)
        {
            RunSkeletonDebug(world, ctx);
        }

        if(ctx.config->drawFps)
        {
            RunFpsDebug(ctx);
        }

        if(ctx.config->drawAxis)
        {
            RunAxisDebug(ctx);
        }
    }

    void DebugSystemRunner::Submit(const DebugContext& ctx)
    {
        assert(ctx.frame);
        assert(ctx.frame->renderer);
        assert(ctx.frame->graphicsAssetStore);
        assert(ctx.debugDraw);
        assert(ctx.resources);

        auto& renderer = *ctx.frame->renderer;
        const auto& graphicsStore = *ctx.frame->graphicsAssetStore;

        const auto* textShader = graphicsStore.TryGet(ctx.resources->textShader);
        const auto* lineShader = graphicsStore.TryGet(ctx.resources->lineShader);

        if (textShader && ctx.debugDraw->TextIndexCount() > 0)
        {
            renderer.Submit(::ddknd::graphics::DebugTextDrawCommand{.batch = ctx.debugDraw->TextBatch(),
                                                                    .shader = textShader->program,
                                                                    .texture = ctx.debugDraw->FontAtlas(),
                                                                    .indexCount = ctx.debugDraw->TextIndexCount()});
        }

        if (lineShader && ctx.debugDraw->LineVertexCount() > 0)
        {
            renderer.Submit(::ddknd::graphics::DebugLineDrawCommand{.batch = ctx.debugDraw->LineBatch(),
                                                                    .shader = lineShader->program,
                                                                    .vertexCount = ctx.debugDraw->LineVertexCount()});
        }
    }

    void DebugSystemRunner::RunSkeletonDebug(::ddknd::ecs::World& world, const DebugContext& ctx)
    {
        assert(ctx.frame->graphicsAssetStore);

        auto view = world.GetRegistry().view(
            ecs::query()
                .select<::ddknd::component::SkinnedModelComponent>()
                .require<::ddknd::component::PoseComponent, ::ddknd::component::TransformComponent>());

        for (auto [model, pose, transform] : view)
        {
            ::ddknd::animation::debug::SkeletonDebugDrawSystem::UpdateOne(
                model, pose, transform, *ctx.frame->graphicsAssetStore, ctx.config->skeletonStyle.color, *ctx.debugDraw);
        }
    }

    void DebugSystemRunner::RunFpsDebug(const DebugContext& ctx)
    {
        assert(ctx.frame);
        assert(ctx.debugDraw);
        assert(ctx.config);
        assert(ctx.frame->graphicsAssetStore);
        assert(ctx.resources);

        const auto* font = ctx.frame->graphicsAssetStore->TryGet(ctx.resources->font);
        if(!font)
        {
            return;
        }

        ctx.debugDraw->SetFont(font);
        ctx.debugDraw->Text(10.0f, 20.0f, std::format("FPS: {:.1f}", ctx.fps), ctx.config->fpsStyle.color);
    }

    void DebugSystemRunner::RunAxisDebug(const DebugContext& ctx)
    {
        assert(ctx.debugDraw);
        assert(ctx.config);

        const auto& style = ctx.config->axisStyle;

        ctx.debugDraw->Axis(style.origin, graphics::DebugAxisColors{style.axisX, style.axisY, style.axisZ}, style.length);
    }
} // namespace ddknd::debug