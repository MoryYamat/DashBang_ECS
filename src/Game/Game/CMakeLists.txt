cmake_minimum_required(VERSION 3.10)

add_library(dashbang STATIC)


file(GLOB_RECURSE dashbang_SRC CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
)

target_sources(dashbang PRIVATE ${dashbang_SRC})

target_include_directories(dashbang 
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_features(dashbang PUBLIC cxx_std_20)

target_link_libraries(dashbang 
    PUBLIC 
        ddknd
)

add_executable(DashBang_ECS
    main.cpp
)

target_compile_features(DashBang_ECS PRIVATE  cxx_std_20)

target_link_libraries(DashBang_ECS 
    PRIVATE 
        dashbang
)

set_target_properties(DashBang_ECS PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)


set(assets_src_dir "${CMAKE_SOURCE_DIR}/assets")

add_custom_command(TARGET DashBang_ECS POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${assets_src_dir}"
            "$<TARGET_FILE_DIR:DashBang_ECS>/assets"
    COMMENT "Copy assets -> $<TARGET_FILE_DIR:DashBang_ECS>/assets"
)