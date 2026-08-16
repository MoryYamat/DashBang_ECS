add_executable(fsm_test main.cpp)

target_compile_features(fsm_test PRIVATE cxx_std_20)

target_include_directories(fsm_test
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/generated
        ${CMAKE_SOURCE_DIR}/ddknd/include
)