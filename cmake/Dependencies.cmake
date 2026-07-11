include(FetchContent)

# -----------------------
# vcpkg packages
# -----------------------
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

# -----------------------
# Google Test
# -----------------------
# include(FetchContent)
# FetchContent_Declare(
#   googletest
#   URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
# )
# For Windows: Prevent overriding the parent project's compiler/linker settings

# set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
# FetchContent_MakeAvailable(googletest)

# -----------------------
# cgltf
# -----------------------
FetchContent_Declare(
    cgltf
    GIT_REPOSITORY https://github.com/jkuhlmann/cgltf.git
    GIT_TAG        v1.15
)
FetchContent_MakeAvailable(cgltf)

if(NOT TARGET cgltf::cgltf)
  add_library(cgltf_cgltf INTERFACE)
  add_library(cgltf::cgltf ALIAS cgltf_cgltf)

  target_include_directories(cgltf_cgltf
    INTERFACE 
      ${cgltf_SOURCE_DIR}
  )
endif()

# -----------------------
# stb
# -----------------------
FetchContent_Declare(
  stb
  GIT_REPOSITORY  https://github.com/nothings/stb.git
  GIT_TAG         e6cd9561ea6dae43d41633797745789d142b691e
)
FetchContent_MakeAvailable(stb)

if(NOT TARGET stb::stb)
  add_library(stb_stb INTERFACE)
  add_library(stb::stb ALIAS stb_stb)

  target_include_directories(stb_stb
    INTERFACE
      ${stb_SOURCE_DIR}
  )
endif()

