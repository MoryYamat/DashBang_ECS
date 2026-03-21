cmake_minimum_required(VERSION 3.25)

if(POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT
        "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project("DashBang_ECS" LANGUAGES CXX)

add_subdirectory("src/Engine")
add_subdirectory("src/Game")

if(MSVC)
    target_compile_options(DashBang_ECS PRIVATE /wd4828)
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-w)
endif()