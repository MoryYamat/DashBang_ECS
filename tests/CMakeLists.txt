add_custom_target(ddknd_tests)

function(add_ddknd_test target_name test_name source_file)
    add_executable("${target_name}" "${source_file}")
    target_link_libraries("${target_name}" PRIVATE ddknd)
    add_test(NAME "${test_name}" COMMAND "${target_name}")

    add_dependencies(ddknd_tests "${target_name}")
endfunction()


add_ddknd_test(ddknd_entity_tests ecs.entity ecs/entity_test.cpp)

add_ddknd_test(ddknd_registry_tests ecs.registry ecs/registry_test.cpp)

add_ddknd_test(ddknd_input_tests input.mapping input/input_test.cpp)