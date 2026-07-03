# Appendix: Quality Checks and Improvements

以下では、本ポートフォリオに対して行ったテスト、静的解析、メモリリーク確認、改善方針を記録する．

## もくじ


## 目的
ポートフォリオ資料の課題である、**テスト・計測不足** に対応するために、
実際に外部ツールや簡易コードを使用して逐次テストを行う．

その記録を **改善履歴** としてまとめる．

このようなテストは Tutorial 的なものが多く、本格的な調査はできていない．


## Valgrind による確認
以下は、wsl 経由で valgrind を使用して `--leak-check` を行った結果である．
```log
==30691== LEAK SUMMARY:
==30691==    definitely lost: 56 bytes in 1 blocks
==30691==    indirectly lost: 56 bytes in 1 blocks
==30691==      possibly lost: 0 bytes in 0 blocks
==30691==    still reachable: 234,379 bytes in 3,448 blocks
==30691==         suppressed: 0 bytes in 0 blocks
```

メモリリークが1つ確認できる．該当の割り当て呼び出し行を確認したところ次のログであった．

```log
==8851== 112 (56 direct, 56 indirect) bytes in 1 blocks are definitely lost in loss record 2,621 of 2,659
==8851==    at 0x484D953: calloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==8851==    by 0x67BE8AD: ???
==8851==    by 0x61C879B: ???
...
==8851==    by 0x5B3A2B1: ???
==8851==    by 0x5D1CD5D: ???
==8851==    by 0x23B36B: ddknd::graphics::OpenGLRendererBackend::CreateTexture2D(ddknd::graphics::types::Texture2DCreateDesc const&) (openglRendererbackend.cpp:514)
==8851==    by 0x1CD430: (anonymous namespace)::BuildTextureResourceFromImport(ddknd::graphics::internal::types::ModelImportData const&, unsigned int, ddknd::graphics::IRendererBackend&) (gfx_asset_loader.cpp:783)
==8851==    by 0x1CA255: ddknd::graphics::GraphicsAssetLoader::LoadModel(ddknd::asset::AssetManager&, ddknd::graphics::GraphicsAssetStore&, ddknd::animation::AnimationAssetStore&, ddknd::core::HandleID<ddknd::asset::tag::Model>) (gfx_asset_loader.cpp:210)
==8851==    by 0x1427D2: app::scene::LoadMainSceneAssets(app::scene::GameSceneAssets const&, app::scene::SceneLoadContext&) (game_scene.cpp:60)
==8851==    by 0x124ADC: app::App::Init(int, int) (app.cpp:141)
==8851==    by 0x123F7F: main (main.cpp:34)
```

このスタックトレースでは、実際のメモリ確保は `calloc` で行われていることがわかる．  
`OpenGLRendererBackend::CreateTexture2D` はその呼び出し経路上で確認できる自作コード側の最下層である．  

`calloc`との間には関数名を解決できていない複数の外部ライブラリ内のフレームが存在する．

`CreateTexture2D()`には、`glGenTextures`、`glTexImage2D`、`glGenerateMipmap`などのOpenGL関数を呼び出している．  
これらを契機として OpenGL 実装内部で確保されたメモリが未開放になっている可能性が考えられる．

スタック上の未解決フレームのアドレスは、`0x5D1CD5D <= address < 0x67BE8AD` の範囲 になっていた．

このアドレスは実行時にロードされた、次の Gallium ライブラリのアドレスの範囲内に存在していた．
```log
--8851-- Discarding syms at 0x5a77880-0x72839e2 in /usr/lib/x86_64-linux-gnu/libgallium-25.2.8-0ubuntu0.24.04.2.so (have_dinfo 1)
```

自作側に存在するリークかを確認するために次のことを確認した
- `OpenGLRendererBackend` のデストラクタが実際に実行されていること
- OpenGL Context が 破棄される前にデストラクタが実行されていること
- 作成した Texture に対して `glDeleteTextures` が呼ばれていること
- モデルおよびテクスチャのロード回数を増やしても、リーク量は増加しないこと

複数テクスチャをロードしても検出結果は次の結果であった
```log
==140279== LEAK SUMMARY:
==140279==    definitely lost: 56 bytes in 1 blocks
==140279==    indirectly lost: 56 bytes in 1 block
```

この結果から自作コード側の明確な解放漏れである可能性は低いと判断し、現時点での対応は保留とすることにした．

## Cppcheck による静的解析

## テスト状況

### 不足しているテスト

- Cppcheck による 静的解析
- `std::chrono` による 簡易的なパフォーマンス計測
- 

### テスト履歴
| Date | Category | 内容 | Status |
| --- | --- | --- | --- |
| 2026-07-03 | Memory | Valgrind によるメモリリーク確認を実施 | Done |