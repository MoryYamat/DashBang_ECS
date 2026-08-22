# Appendix: Quality Checks and Improvements

!このページは執筆中です．

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
未実施

## 簡易的な1000体分のアニメーション更新のパフォーマンス計測

- date: 2026/07/08
- Release Build
- 1,000 Animated Characters
- 約14 ms/frame
- P90 約15 ms

### specs
```txt
OS: Windows11
cpu: INTEL core i7 9700
gpu: NVIDIA RTX 2070 Super
```

## RenderDoc の導入
未実施

## VisualStudio Profiler によるホットパス分析
- date: 2026/07/08
- Release Build
- 1,000 Animated Characters

```cpp
void AnimatorSystem::UpdateGlobalPose(skeleton, pose)
{
    // ...
    // 12.73 %
    std::vector<bool> computed(boneCount, false);

    // 5.56 %
    for (std::size_t i = 0; i < boneCount; ++i)
    {
        ComputeBoneGlobal(skeleton, pose, i, computed);
    }

    for (std::size_t i = 0; i < boneCount; ++i)
    {
        // 2.67 %
        pose.skinMatrices[i] = pose.globalMatrices[i] * skeleton.bones[i].inverseBindMatrix;
    }
}
```

初回計測時は `computed` バッファ周辺が大きな割合を占めていた.  


### リファクタリング 後
`SkeletonResource::bones` の配列順は、glTF の `Skins.joints`順を維持しており、hierarchyの評価順を保証しない。  
このため、リファクタリング前は、`computed`バッファを用いて parent の階層関係を再帰的に解決していた。  
そこで、以下のように、ロード時に bone hierarchy から parent-first の `evaluationOrder`を事前計算し、  
Runtime ではその順序に従って線形走査する方式へ変更した。

```cpp
struct SkeletonResource
{
    std::vector<Bone> bones;
    std::vector<std::uint32_t> evaluationOrder;
};
```

同条件の1000体描画のプロファイリング結果は以下のようになった

```cpp
void AnimatorSystem::UpdateGlobalPose(types::Pose& pose, const types::SkeletonResource& skeleton)
{
    ...

    for(const std::uint32_t boneIndex : skeleton.evaluationOrder)
    {
        const auto& bone = skeleton.bones[boneIndex];

        if(bone.parent < 0)
        {
            pose.globalMatrices[boneIndex] = pose.localMatrices[boneIndex];
        }
        else
        {
            // 4.06 %
            pose.globalMatrices[boneIndex] = pose.globalMatrices[bone.parent] * pose.localMatrices[boneIndex];
        }

        // 4.83 %
        pose.skinMatrices[boneIndex] = pose.globalMatrices[boneIndex] * bone.inverseBindMatrix;
    }
}
```

ロード時に用意した `evaluationOrder` からboneIndexを解決することで、  
**毎フレームの `std::vector<bool> computed` の 生成・初期化と、`ComputeBoneGlobal`の再帰的な親子関係の解決を廃止した。

その結果、Visual Studio Profiler上で、`UpdateGlobalPose` の合計CPU time 比率は 12.71% から 9.22% へと低下し、相対的に **約27.5%** 削減された。

改善後は主なコストが行列演算へ集約されており、毎フレームの一時バッファ生成と再帰的な依存解決はホットパスから除去できた。


## 単体テスト

- tests/ecs に CTest による `ddknd::ecs`実装の単体テスト追加
- 

### 不足しているテスト

- Cppcheck による 静的解析
- `std::chrono` による 簡易的なパフォーマンス計測
- RenderDoc の導入


### テスト履歴
| Date | Category | 内容 | Status |
| --- | --- | --- | --- |
| 2026-07-03 | Memory | Valgrind によるメモリリーク確認を実施 | Done |
