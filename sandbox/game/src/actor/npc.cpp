#include "game/actor/character/npc.h"

#include <ddknd/component/gfx_component.h>
#include <ddknd/ecs/ecs.h>
#include <ddknd/component/hitbox_component.h>


#include "game/assets/actor/paladin_assets.h"
#include "game/assets/actor/mutant_assets.h"

#include "game/component/state_component.h"


namespace app::actor
{
    ddknd::ecs::Entity CreatePaladinNPC(ddknd::ecs::World& world,
                                        const app::assets::actor::PaladinAssetIDs& paladinAssets,
                                        const NPCSpawnDesc& desc)
    {
        auto e = CreatePaladin(world, paladinAssets);

        auto& reg = world.GetRegistry();
        auto& transform = reg.GetComponent<ddknd::component::TransformComponent>(e);
        transform.localTRS.translation = desc.position;
        transform.dirty = true;

        // **** Necessity for skinned mesh animation **** 
        reg.AddComponent<ddknd::component::HurtboxComponent>(e);
        reg.AddComponent<ddknd::component::SphereHurtboxComponent>(
            e, ddknd::component::SphereHurtboxComponent{.localOffset = {0.0f, 0.75f, 0.0f}});
        reg.AddComponent<app::component::PlayerLocomotionStateComponent>(e);
        reg.AddComponent<app::component::AttackStateComponent>(e);
        // **********************************************

        // used for debugging
        // std::cerr << "paladinAssetsIDs: ModelID = " <<  paladinAssets.model.Value() << "\n";
        // std::cerr << "paladinAssetsIDs: shader = " <<  paladinAssets.skinnedShader.Value() << "\n";
        // std::cerr << "paladinAssetsIDs: attack = " <<  paladinAssets.attack.Value() << "\n";

        return e;
    }

    ddknd::ecs::Entity CreateMutantNPC(ddknd::ecs::World& world,
                                       const app::assets::actor::MutantAssetIDs& mutantAssets,
                                       const NPCSpawnDesc& desc)
    {
        auto e = CreateMutant(world, mutantAssets);

        auto& reg = world.GetRegistry();
        auto& transform = reg.GetComponent<ddknd::component::TransformComponent>(e);
        transform.localTRS.translation = desc.position;
        transform.dirty = true;

        // **** Necessity for skinned mesh animation ****
        reg.AddComponent<ddknd::component::HurtboxComponent>(e);
        reg.AddComponent<ddknd::component::SphereHurtboxComponent>(
            e, ddknd::component::SphereHurtboxComponent{.localOffset = {0.0f, 0.75f, 0.0f}});
        reg.AddComponent<app::component::PlayerLocomotionStateComponent>(e);
        reg.AddComponent<app::component::AttackStateComponent>(e);
        // **********************************************

        // used for debugging.
        // std::cerr << "MutantAssetIDs: ModelID = " <<  mutantAssets.model.Value() << "\n";
        // std::cerr << "MutantAssetIDs: shader = " <<  mutantAssets.skinnedShader.Value() << "\n";
        // std::cerr << "MutantAssetIDs: idle = " <<  mutantAssets.idle.Value() << "\n";
 
        return e;
    }
} // namespace app::actor