add_custom_target(ddknd_tests)

function(add_ddknd_test target_name test_name source_file)
    add_executable("${target_name}" "${source_file}")
    target_include_directories("${target_name}" 
        PRIVATE 
            "${CMAKE_CURRENT_SOURCE_DIR}"
            "${PROJECT_SOURCE_DIR}/ddknd/src"
    )
    target_link_libraries("${target_name}" PRIVATE ddknd)
    add_test(NAME "${test_name}" COMMAND "${target_name}")

    add_dependencies(ddknd_tests "${target_name}")
endfunction()



add_ddknd_test(ddknd_entity_tests ecs.entity ecs/entity_test.cpp)
add_ddknd_test(ddknd_registry_tests ecs.registry ecs/registry_test.cpp)
add_ddknd_test(ddknd_query_tests ecs.query ecs/query_test.cpp)
add_ddknd_test(ddknd_view_tests ecs.view ecs/view_test.cpp)
add_ddknd_test(ddknd_storage_tests ecs.storage ecs/component_storage_test.cpp)
add_ddknd_test(ddknd_world_tests ecs.world ecs/world_test.cpp)

add_ddknd_test(ddknd_input_tests input.mapping input/input_test.cpp)

add_ddknd_test(ddknd_animation_pose_tests animation.pose graphics/pose_test.cpp)
add_ddknd_test(ddknd_skeleton_builder_tests skeleton.builder graphics/skeleton_builder_test.cpp)

add_ddknd_test(ddknd_fsm_symbol_table_tests fsm.AxisBuilder fsm/axis_builder_test.cpp)
add_ddknd_test(ddknd_fsm_compiler_tests fsm.AxisCompiler fsm/axis_compiler_test.cpp)