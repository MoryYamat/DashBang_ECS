cmake_minimum_required(VERSION 3.25)


project("engine_ddknd" LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_PDB_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# delete
# if(POLICY CMP0141)
#     cmake_policy(SET CMP0141 NEW)
#     set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT
#         "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
# endif()

# format
if (MSVC)
    add_compile_options(/utf-8)
endif()

include(cmake/Dependencies.cmake)


# The engine is always built.
add_subdirectory("ddknd")# engine runtime

add_subdirectory("sandbox/game")# game
add_subdirectory("sandbox/fsm")

include(CTest)

if(BUILD_TESTING)
    add_subdirectory("tests")
endif()