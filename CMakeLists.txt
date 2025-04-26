cmake_minimum_required(VERSION 3.10)

# プロジェクト名とC++標準の設定
project(ClashBattle)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ============================================================
# 外部ライブラリ設定
# ============================================================

# GLFW（インポート：事前ビルド済み静的ライブラリ）
add_library(glfw STATIC IMPORTED)
set_target_properties(glfw PROPERTIES
    IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/External/GLFW/lib-vc2022/glfw3.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/External/GLFW/include"
)

# ASSIMP
add_library(assimp STATIC IMPORTED)
set_target_properties(assimp PROPERTIES
    IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/External/Assimp/lib/assimp-vc143-mt.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/External/Assimp/include"
)

set(ASSIMP_DLL "${CMAKE_SOURCE_DIR}/External/Assimp/bin/Release/assimp-vc143-mt.dll")


# GLAD（ソースからビルド）
add_library(glad STATIC
    ${CMAKE_SOURCE_DIR}/External/GLAD/src/glad.c
)
target_include_directories(glad PUBLIC
    ${CMAKE_SOURCE_DIR}/External/GLAD/include
)

# ============================================================
# プロジェクトのソース・ヘッダー
# ============================================================

file(GLOB_RECURSE HEADER_FILES "${CMAKE_SOURCE_DIR}/include/*.h")
file(GLOB_RECURSE SRC_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp")

# 実行可能ファイルの作成
add_executable(ClashBattle ${SRC_FILES} ${HEADER_FILES})
# add_executable(ClashBattle
#     src/main.cpp
#     src/Shader.cpp
#     src/Camera.cpp
#     include/Shader.h
#     include/Camera.h
#     include/Component.h
#     include/ECS.h
# )




# インクルードディレクトリ
target_include_directories(ClashBattle PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/External/GLFW/include
    ${CMAKE_SOURCE_DIR}/External/GLAD/include
    ${CMAKE_SOURCE_DIR}/External/GLM
    ${CMAKE_SOURCE_DIR}/External/stb
    ${CMAKE_SOURCE_DIR}/External/Assimp/include

)

file(COPY "${CMAKE_SOURCE_DIR}/shaders" DESTINATION "${CMAKE_BINARY_DIR}")
file(COPY "${CMAKE_SOURCE_DIR}/Assets" DESTINATION "${CMAKE_BINARY_DIR}")




# リンクするライブラリ
target_link_libraries(ClashBattle
    glfw
    glad
    assimp
    opengl32
)


add_custom_command(TARGET ClashBattle POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${ASSIMP_DLL}"
        "$<TARGET_FILE_DIR:ClashBattle>"
)