#include <ddknd/fsm/runtime/definitions.h>
#include "generated/movement_definition.h"

#include "ddknd/math/math.h"


#include <cassert>

int main()
{
    using namespace fsm::MovementFSM;

    MovementFSMInstance movement_fsm_instance{};
    MovementFSMParameters movement_params{};

    assert(movement_fsm_instance.current == MovementFSMState::Idle);
    movement_params.MovementIntent = ddknd::math::lengthSquared(ddknd::math::Vec2f{1.0f, 1.0f});

    ddknd::fsm::Evaluate(movement_fsm_instance, movement_params);
    assert(movement_fsm_instance.current == MovementFSMState::Run);

    movement_params.MovementIntent = ddknd::math::lengthSquared(ddknd::math::Vec2f{0.0f, 0.0f});
    ddknd::fsm::Evaluate(movement_fsm_instance, movement_params);
    assert(movement_fsm_instance.current == MovementFSMState::Idle);
    assert(movement_fsm_instance.previous == MovementFSMState::Run);

    return 0;
}