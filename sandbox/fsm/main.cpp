#include <iostream>

#include "fsm/user_fsm_definition.h"

#include <ddknd/fsm/FSMDefinition.h>

int main()
{
    std::cout << "hello fsm\n";

    ddknd::fsm::FSMDefinitionRegistry fsmRegistry{};

    fsm::user::UserFSMDeifinition(fsmRegistry);

    return 0;
}