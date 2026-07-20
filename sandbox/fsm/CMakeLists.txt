add_library(fsmkit STATIC)


file(GLOB_RECURSE fsmkit_SRC CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
)

target_sources(fsmkit PRIVATE ${fsmkit_SRC})

target_include_directories(fsmkit 
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_features(fsmkit PUBLIC cxx_std_20)

target_link_libraries(fsmkit 
    PUBLIC # The game should not directly include ddknd.
        ddknd
)
add_executable(sandbox_fsm main.cpp)

target_compile_features(sandbox_fsm PRIVATE  cxx_std_20)

target_link_libraries(sandbox_fsm 
    PRIVATE
        fsmkit
        # game should use gamekit as its entry layer, not ddknd directly.
)