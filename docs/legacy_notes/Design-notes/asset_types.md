# graphics asset type の定義・設計のためのメモを残す

```
// @TODO: 3Dmodel実行時データ構造の設計とIR->Runtime変換の実装

// GPUID<PrimTag>

// static
// struct ModelResource
// {
//     std::vector<GPU<PrimTag>> meshes;
//     Skeleton skeleton;
//     std::vector<AnimationClip> clips;
// };

// 問題:
// - ModelResource{}自体を保持するStoreを作るか．それとも各リソースを保持するストアにするか
// - ModelResource{}を保持するStoreを作る場合:
//          - 各ResourceとModelResourceの対応関係が必要
//          - 使用側はload(model_id)で簡単
// - ModelResource{}を保持しない場合
//          - 使用側がload(each_id) が必要で大変
//          - ModelResourceの対応関係は不要
// 結論: Resourceを保持するStoreが必要だと思われる
// 懸念点: 例えば、実行時のメッシュやprimitives・pose関係の動的な変化をどう扱うか
// 例 - キャラクターA が Prob_B を 持つ
// というようなものをどうデータの論理で表現するか．ModelResource_Storeを導入するとその管理が大変になるのではないか 予測:
// ModelResource_Storeを導入しなくても、その管理は大変．むしろ論理的には導入したほうが正しい上に扱いやすいと予想される(Instance層で、Model単位の階層関係(もしくは所有関係)を論理的に構築できる)

// 問題2:
// PrimitivesをまとめるMesh層は必要か
//

// runtime
// struct ModelInstance
// {
//     ModelID model;
//     Pose pose;
//     AnimationState animState;
// };

// @TODO
// LOW:
// - Editor 機能: glTF->Scene(Model分割)->sceneIndex を 表示
```