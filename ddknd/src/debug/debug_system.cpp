#include <ddknd/debug/debug_system.h>

#include "ddknd/graphics/debug_draw.h"
#include "ddknd/graphics/renderer.h"

#include <ddknd/ecs/ecs.h>
#include <ddknd/system/system.h>

#include <ddknd/component/gfx_component.h>
#include <ddknd/component/hitbox_component.h>

#include <ddknd/graphics/debug_animation.h>
#include <ddknd/graphics/gfx_asset_loader.h>

#include <ddknd/event/hit_event.h>

#include <ddknd/input/input.h>

namespace ddknd::debug
{
    void UpdateDebugConfigFromInput(const ddknd::input::DeviceInput& input, DebugConfig& config)
    {
        using namespace ddknd::input;
        if (input.IsKeyPressed(Key::F1))
        {
            config.drawFrameTimeInfo = !config.drawFrameTimeInfo;
        }
        if (input.IsKeyPressed(Key::F2))
        {
            const bool enabled = !(config.drawHitboxes && config.drawHurtboxes);

            config.drawHitboxes = enabled;
            config.drawHurtboxes = enabled;
        }
        if (input.IsKeyPressed(Key::F3))
        {
            config.drawSkeletons = !config.drawSkeletons;
        }
        if(input.IsKeyPressed(Key::F4))
        {
            config.drawHitEvents = !config.drawHitEvents;
        }
        if (input.IsKeyPressed(Key::F5))
        {
            switch (config.camera.overrideMode)
            {
            case CameraOverrideMode::None:
                config.camera.overrideMode = CameraOverrideMode::DebugCamera;
                break;
            case CameraOverrideMode::DebugCamera:
                config.camera.overrideMode = CameraOverrideMode::None;
                break;
            }
        }
    }

    void DebugSystemRunner::BeginFrame(const DebugContext& ctx)
    {
        assert(ctx.debugDraw);
        ctx.debugDraw->BeginFrame();
    }
    void DebugSystemRunner::EndFrame(const DebugContext& ctx)
    {
        assert(ctx.debugDraw);
        ctx.debugDraw->EndFrame();
    }

    void DebugSystemRunner::Update(::ddknd::ecs::World& world, const DebugContext& ctx)
    {
        assert(ctx.frame);
        assert(ctx.debugDraw);
        assert(ctx.config);

        const bool textReady = PrepareTextDebug(ctx);

        if (ctx.config->drawSkeletons)
        {
            RunSkeletonDebug(world, ctx);
        }

        if (ctx.config->drawFrameTimeInfo && textReady)
        {
            RunFramePerfDebug(ctx);
        }

        if (ctx.config->drawAxis)
        {
            RunAxisDebug(ctx);
        }

        if (ctx.config->drawHitboxes)
        {
            RunHitboxDebug(world, ctx);
        }

        if (ctx.config->drawHurtboxes)
        {
            RunHurtboxDebug(world, ctx);
        }

        if (ctx.config->drawHitEvents)
        {
            RunHitEventDebug(ctx);
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
            renderer.Submit(ddknd::graphics::DebugTextDrawCommand{.batch = ctx.debugDraw->TextBatch(),
                                                                  .shader = textShader->program,
                                                                  .texture = ctx.debugDraw->FontAtlas(),
                                                                  .indexCount = ctx.debugDraw->TextIndexCount()});
        }

        if (lineShader && ctx.debugDraw->LineVertexCount() > 0)
        {
            renderer.Submit(ddknd::graphics::DebugLineDrawCommand{.batch = ctx.debugDraw->LineBatch(),
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
            ::ddknd::animation::debug::SkeletonDebugDrawSystem::UpdateOne(*ctx.debugDraw, model, pose, transform,
                                                                          *ctx.frame->graphicsAssetStore,
                                                                          ctx.config->skeletonStyle.color);
        }
    }

    void DebugSystemRunner::RunFramePerfDebug(const DebugContext& ctx)
    {
        assert(ctx.debugDraw);
        assert(ctx.config);

        const auto& perf = ctx.framePerformance;
        const auto& color = ctx.config->framePerformanceStyle.color;

        constexpr float x = 10.0f;
        constexpr float firstLineY = 20.0f;
        constexpr float lineHeight = 20.0f;

        ctx.debugDraw->Text(x, firstLineY, std::format("FPS: {:.1f}", perf.fps), color);

        ctx.debugDraw->Text(x, firstLineY + lineHeight, std::format("Current: {:.2f} ms", perf.currentMs), color);

        ctx.debugDraw->Text(x, firstLineY + lineHeight * 2.0f, std::format("P90: {:.2f} ms", perf.p90Ms), color);

        ctx.debugDraw->Text(x, firstLineY + lineHeight * 3.0f, std::format("P99: {:.2f} ms", perf.p99Ms), color);
    }

    void DebugSystemRunner::RunAxisDebug(const DebugContext& ctx)
    {
        assert(ctx.debugDraw);
        assert(ctx.config);

        const auto& style = ctx.config->axisStyle;

        ctx.debugDraw->Axis(style.origin, graphics::debug::DebugAxisColors{style.axisX, style.axisY, style.axisZ},
                            style.length);
    }

    void DebugSystemRunner::RunHitboxDebug(ddknd::ecs::World& world, const DebugContext& ctx)
    {
        assert(ctx.debugDraw);
        assert(ctx.config);

        using namespace ddknd::ecs;
        auto& reg = world.GetRegistry();

        auto view =
            reg.view(query()
                         .select<ddknd::component::HitboxComponent>()
                         .require<ddknd::component::HemisphereHitboxComponent, ddknd::component::TransformComponent>());

        const auto color = ctx.config->hitboxStyle.hitboxColor;
        for (auto [hitbox, hemi, transform] : view)
        {
            const auto center =
                ddknd::math::TransformPoint(transform.worldMatrix, ddknd::math::Vec3f{0.0f, 0.0f, 0.0f});
            const auto forward = ddknd::math::normalize(
                ddknd::math::TransformDirection(transform.worldMatrix, ddknd::math::Vec3f{0.0f, 0.0f, 1.0f}));

            ctx.debugDraw->WireHemisphere(center, forward, hemi.radius, color);
        }
    }
    void DebugSystemRunner::RunHurtboxDebug(ddknd::ecs::World& world, const DebugContext& ctx)
    {
        assert(ctx.debugDraw);
        assert(ctx.config);

        using namespace ddknd::ecs;
        auto& reg = world.GetRegistry();

        auto view =
            reg.view(query()
                         .select<ddknd::component::HurtboxComponent>()
                         .require<ddknd::component::SphereHurtboxComponent, ddknd::component::TransformComponent>());

        const auto color = ctx.config->hitboxStyle.hurtboxColor;
        for (auto [hurtbox, sphere, transform] : view)
        {
            const auto center = ddknd::math::TransformPoint(transform.worldMatrix, sphere.localOffset);

            ctx.debugDraw->WireSphere(center, sphere.radius, color);
        }
    }

    bool DebugSystemRunner::PrepareTextDebug(const DebugContext& ctx)
    {
        assert(ctx.frame);
        assert(ctx.frame->graphicsAssetStore);
        assert(ctx.debugDraw);
        assert(ctx.resources);

        const auto* font = ctx.frame->graphicsAssetStore->TryGet(ctx.resources->font);

        if (!font)
        {
            return false;
        }

        ctx.debugDraw->SetFont(font);
        return true;
    }

    void DebugSystemRunner::RunHitEventDebug(const DebugContext& ctx)
    {
        assert(ctx.frame);
        assert(ctx.debugDraw);
        assert(ctx.config);
        assert(ctx.frame->hitboxHitEvents);

        constexpr float displayDuration = 2.0f;

        if (ctx.frame->hitboxHitEvents)
        {
            for (const auto& e : ctx.frame->hitboxHitEvents->events)
            {
                const auto exists =
                    std::find_if(hitEventLines_.begin(), hitEventLines_.end(),
                                 [&](const DebugHitEventLine& line)
                                 {
                                     return line.owner == e.owner && line.hitbox == e.hitbox && line.target == e.target;
                                 }) != hitEventLines_.end();

                if (!exists)
                {
                    hitEventLines_.push_back(DebugHitEventLine{
                        .owner = e.owner,
                        .hitbox = e.hitbox,
                        .target = e.target,
                        .remaining = displayDuration,
                    });
                }
            }
        }

        const float dt = ctx.frame->deltaTime;

        for (auto& line : hitEventLines_)
        {
            line.remaining -= dt;
        }

        hitEventLines_.erase(std::remove_if(hitEventLines_.begin(), hitEventLines_.end(),
                                            [](const DebugHitEventLine& line) { return line.remaining <= 0.0f; }),
                             hitEventLines_.end());

        float y = 100.0f;

        for (const auto& line : hitEventLines_)
        {
            // const float alpha = std::clamp(line.remaining / displayDuration, 0.0f, 1.0f);// fade out
            const float alpha = 1.0f;
            ctx.debugDraw->Text(10.0f, y,
                                std::format("HIT owner={} hitbox={} target={}", line.owner.Index(), line.hitbox.Index(),
                                            line.target.Index()),
                                ddknd::math::Vec4f{1.0f, 0.2f, 0.2f, alpha});

            y += 18.0f;
        }
    }
} // namespace ddknd::debug