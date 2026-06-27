# Lessons Learned 

## ECS
### 初期実装

初期実装では、ECSアーキテクチャを理解することを中心とした．  

#### 初期実装でできたこと
- EntityのCRUD
- EntityにComponentを付与(`Add`)する機能
- Componentを条件として、該当EntityをフィルターするView機能

#### 初期実装の課題
- `EntityManager`というクラスに、`ComponentStorage`・`EntityCRUD`・`View`などの主要機能をすべて持っていて、責務が肥大化していた
- そのため、`Storage`クラスの変更によるStorage戦略の変更が難しかった
- Storageでは `std::shared_ptr<T>` を使用しており、Component がヒープ上に分散するため、  
ECS の利点である連続データアクセスを活かしにくかった

以下の初期実装では、`EntityManager`に責務が集中し、Storageの実装によって、Componentがヒープに分散することでキャッシュの局所性を得られなくなっていた

```cpp
class EntityMgr
{
  private:
	uint32_t nextEntityID = 1;
	// コンポーネントIDごとにエンティティを紐づけ、そこに実データを紐づける
    std::unordered_map<std::type_index, std::unordered_map<uint32_t, std::shared_ptr<void>>> mComponentPools;
  public:
	// create
	Entity createEntity();
    
    // View
	template<typename... Components>
	std::vector<Entity> view();
};

```

### 改善方針
ECSの基盤の責務分離を徹底し、Storage戦略を変更できるような実装にすることを目指した．  
具体的には次のようにした．

- `id`・`registry`・`component_storage`・`view`をできるだけ分離して実装する
- `world`をインターフェースとしてユーザに機能を使いやすくする
- `component_storage`を`sparse_set`にして、キャッシュの局所性による利点を得られるようにする
- `query`を`registry`や`storage`に依存させないようにする

これにより、責務分離を明確にし、Storage 戦略を変更しやすくしつつ、Component の連続配置によってキャッシュ局所性を活かしやすい構造を目指した．
```cpp
class Registry
{
  private:
    // @TODO Change the `std::type_index` part to access and manage using name string-index mapping.
    std::vector<std::uint32_t> gens_; // current generation: gens_[i] = gen (i=Index, gen=currentGeneration)
    std::vector<std::uint32_t> free_indices_;
    std::unordered_map<std::type_index, std::unique_ptr<::ddknd::ecs::IStorage>> storages_;
  public:
    // Entity
    id_type Create();
};

template<typename T>
class Storage final : public IStorage
{
    private:
        std::vector<T> components_;
        std::vector<Entity> ents_;
        std::vector<std::size_t> sparse_;
};

template <typename Query>
class View
{
  public:
    View(Query q, Registry* regs);
    Iterator begin();
    Iterator end();
};
```

### 改善結果
この実装により以下のことが可能になった
- 責務分離が明確になったため、変更の影響を局所化できるようになった．
- `component_storage`のstorage戦略も変更しやすくなった
- `component`を`vector<T>`に連続配置できる構造になり、キャッシュ局所性を活かしやすい実装になった

### 学び
- ManagerクラスにEntity管理、Component戦略、Storage、View生成を集約すると、  
機能追加は簡単になる一方で、Storage戦略やQuery実装を変更しにくくなる
- ECSで、外側の使いやすいAPIと、内部のStorage / Query / View の実装を分けることで、  
利用側のコードを大きく変えずに内部構造を変更しやすくなる
- `std::shared_ptr<T>`でComponentを個別に保持すると、Componentがヒープ上に分散し、  
ECSの利点である連続データアクセスを活かしにくくなる

## AssetManagerの実装

### 初期実装
初期実装では、`AssetManager`などのAsset管理やロード管理の層を考えていなかった．  
初期の動作確認の段階ではほとんど問題にならなかった．

しかし、導入するAssetの種類や量が増えるにしたがって、各種のアセットをRuntimeでも効率的にアクセスできるようにしたり、  
非同期・並列なロードをできるようにしたりする仕組みが必要であることが明確になった．

#### 初期実装の課題
- 3DModel/SFX/Shader などのAsset が コード内に散らばっており、管理が大変であった
- 初期化時に全Assetを個別にImport/Loadする実装になっていて、ロードタイミングの総合的な制御が不可能であった
- 同一Assetデータの使いまわしなどの効率的な運用が難しい状態であった

### 改善方針
Asset管理のパイプラインを設計し、効率的でスケーラブルなAsset管理、ロードタイミング制御を目指した．  
具体的には以下のように考えた．
- AssetLoaderを明示ロードのインターフェースとすることで、非同期ロードの拡張を行いやすくする
- AssetManagerの責務を、AssetIDの発行とLoad状態の管理、vpathとIDの対応を保持  
に限定することで、責務の肥大化、テンプレートによるヘッダの巨大化を防ぐ
- AssetIDをAssetStorageのコンテナのIndexにすることで、Runtime解決のオーバーヘッドを対策する

```
AssetManager
責務:
    - AssetIDの発行
    - 実AssetResourceのLoad状態の管理
    - virtual path と AssetIDの対応の保持

AssetLoader
責務:
    - 明示ロードのインターフェース
    - ロードしたAsset ResourceをAssetStoreへ登録する

AssetStore
責務:
    - 各種Asset Resourceを保持する
    - AssetIDからAssetResourceへのポインタを返す
```

```cpp
class AssetManager
{
  public:
    // AssetID 発行
    template <typename Tag>
    AssetID<Tag> GetOrCreate(std::string_view vpath)
    {
        return Table<Tag>().GetOrCreate(vpath);// default scene
    }
    
    private:
      std::unordered_map<std::type_index, std::unique_ptr<IAssetTable>> tables_;
};

class GraphicsAssetLoader
{
    // ロードインターフェース
    bool LoadModel(AssetManager& assets, GraphicsAssetStore& gfxstore, AnimationAssetStore& animstore, ModelID id);
};

class GraphicsAssetStore
{
  public:
    // AssetResource ポインタのGetter
    const ModelRenderResource* TryGet(ModelID id) const
    {
        return models_.TryGet(id);
    }

    void SetLoaded(ModelID id, ModelRenderResource res)
    {
        models_.Set(id, std::move(res));
    }
  private:
    AssetStorage<ModelRenderResource, ModelTag> models_;// AssetID->Storage Index
};
```

### 改善結果
この形にしたことで、SceneやActorの初期化コードはファイルロードそのものでなく、必要なAssetIDの登録と保持に集中できるようになった．

```cpp
// PaladinActor の 関連アセットパスの集約定義
struct PaladinAssets
{
    static inline constexpr const char* kModelPath = "res://Models/paladin/base_action_animation_diago.glb";

    // 事前の命名契約に基づいたSubAsset用の論理パス
    static inline constexpr const char* kIdleClipName = "idle_default";
};

// Runtime解決用で、AssetManagerから発行されたAssetIDを保持しておくデータ構造
struct PaladinAssetIDs
{
    ModelID model;
    AnimationClipID idle;
};

// 
PaladinAssetIDs RegisterPaladinAssets(::ddknd::asset::AssetManager& assetMgr) 
{
    using AnimationClip = ::ddknd::asset::tag::AnimationClip;
    return PaladinAssetIDs{
        .model = assetMgr.GetOrCreate<::ddknd::asset::tag::Model>(PaladinAssets::kModelPath),
        .idle = assetMgr.GetOrCreate<AnimationClip>(::ddknd::asset::MakeAnimationClipKey(PaladinAssets::kModelPath, PaladinAssets::kIdleClipName)),
    };
}
```

### 学び
- Managerクラスは実リソースそのものを持つのではなく、ID、Index、状態、名前解決などの管理情報を扱う役割に限定することで、責務の肥大化を防ぎやすい
- Loaderなどの「処理を実行する層」と、Storeのような「データを保持する層」を分けることで、ロード方式の変更、非同期処理への拡張、コンテナやアロケータの差し替えを行いやすくなる
- Runtime で頻繁に参照されるデータは、ファイルパスや文字列ではなく、IDやIndexを通して解決できる形にしておくと、参照コストと依存関係を整理しやすい


## glb importer の改善
変更の核
```
Before:
.glb parser
    -> Runtime ModelData

After:
.glb parser
    -> ModelImportData
    -> Runtime Model / Animation / GPU Resource
```


### 初期実装
該当ファイル:  
- glb_importer.h: 
- model_import_types.h

初期実装ではライブラリでパースした.glb形式のファイルを直接 Engine Runtime 用データ構造へ変換していた.  
この方式により、.glb形式をロードして OpenGL で作成したRendering Pipeline で描画できることを確認した．

#### 初期実装でできたこと
- .glbを読み込めた
- OpenGL Rendering Pipeline で描画できた
- Skeleton / AnimationClip を Runtime Data として扱えた

#### 初期実装の課題
- Importer が Runtime 用データ構造に直接依存していた
- glTF のパース処理と Runtime Resource への変換処理が混ざっていた
- .glb内部のAnimationClip などの SubAsset を個別の Asset として扱いにくかった
- Importer 単体のテスト・デバッグがしにくかった


以下では、`CgltfImporter::Import(const std::string& path)` で パースとEngineRuntimeデータへの変換が同時に行われている点が問題だった．
```cpp
// =============== Model data structure used in Runtime ===============
struct ModelData
{
    // Meshes
	std::vector<MeshData> meshes;

    // Skeletal Animation
	Skeleton skeleton;
	std::vector<AnimationClip> clips;
};

ModelData CgltfImporter::Import(const std::string& path)
{
    // Runtime Data 
    ModelData model;

    for(const auto& mesh : gltf.meshes)
    {
        for(const auto& primitive : mesh.primitives)
        {
            // glTF の attribute / accessor を読み取り、
            // Runtime 用 の MeshData に直接変換する
            model.meshes.push_back(BuildRuntimeMesh(primitive));
        }
    }
    model.skeleton = BuildRuntimeSkeleton(gltf);
    model.clips = BuildRuntimeAnimationClips(gltf);

    return model;
}
```

### 改善方針

初期実装では、`.glb` のパースと Runtime 用データ構造への変換が同じ関数内に存在していた．  
そのため、改善後は Importer と Runtime Resource Builder の責務を分離した．

具体的には、以下の方針にした.

- `.glb` の読み取り結果を、まず `ModelImportData` に変換する
- `ModelImportData` は glTF に近い構造を保つ
- Importer は Runtime 用データ構造を直接作らない
- Runtime Resource への変換は `GraphicsAssetLoader` / Builder 側で行う
- `.glb` 内の AnimationClip は `AnimTag` Asset として登録する

これにより、変更の影響波及を局所化し、描画バックエンド固有の変換処理を分離しやすい構造にすることを目指した
```cpp
// =============== model_import_types.h ===============
struct ModelImportData
{
    int defaultScene = -1;
    
    std::vector<ImportScene> scenes;
    std::vector<ImportNode> nodes;
    std::vector<ImportMesh> meshes;
    std::vector<ImportPrimitive> primitives;
    std::vector<ImportSkin> skins;
    std::vector<ImportAnimation> animations;
};

// =============== glb_importer.h ===============
// 1. Importer: .glbを中間表現へ変換する
std::optional<::ddknd::graphics::internal::types::ModelImportData> ImportModel(const std::string& path)
{
    ModelImportData model{};

    ReadNodes(g, model);
    ReadScenes(g, model);
    ReadMeshes(g, model);
    ReadSkins(g, model);
    ReadAnimations(g, model);

    return model;
}

// =============== gfx_asset_loader.h ===============
bool GraphicsAssetLoader::LoadModel(...)
{
    // 2. AssetLoader: 
    auto imported = ddknd::graphics::internal::ImportModel(absPath);

    // 3. AssetLoader: 中間表現をつかって SubAssetを登録する 
    RegisterAnimationClips(assets, *imported, modelVPath);

    // 4. Builder: 中間表現から Runtime Resource を作る
    auto res =
        BuildModelRenderResource(*imported, backend_);

    // 5. AssetStore へ 追加する
    gfxstore.SetLoaded(id, std::move(res.model));

    return true;
}
```

### 改善結果
この分離により、以下が可能になった
- Importer を ファイル形式の読み取りに限定できる
- Runtime用データ構造の変更がImporterへ波及しにくくなる
- .glb内部のAnimationClipをSubAssetとしてAssetManagerに登録できる
- 将来的な非同期ロードや遅延変換の入り口を作れる
- Importer の出力である `ModelImportData` を確認すれば、 glTF の読み取り段階と Runtime 変換段階 を分けてデバッグできる

### 学び
- 中間表現を導入することで、責務分離とテスト境界を明確にできる
- 拡張に耐える設計にするためには、責務分離を徹底する必要がある

## FSMEngine設計の改善
以下2つのFSM基盤実装は現在のEngine本体に統合しているものではなく、過去の実装と設計実験から得た学びを整理したものである．

初期実装では、MovementFSM の状態を`std::type_index`で識別し、Conditionをvirtual interface として実装していた．  
この方式により、`Idle`/`Moving` の 状態遷移や、CCによる移動受付停止などの基本的な挙動の確認はできた．

#### 初期実装でできたこと
- 入力に基づくCharacterの状態遷移
- CC(行動阻害)による移動状態遷移の停止

#### 初期実装の課題
- 状態識別がC++型に依存していたため、FSM定義と実装が密結合になった．(拡張性に乏しい)
- C++型に依存するため、状態一覧の検証、デバッグ表示、データ駆動化、ツール化が難しくなった．(プログラマしか扱えない)
- あとから状態や条件を追加するたびに依存関係が複雑になった

以下の例では、状態・遷移要求・強制遷移がすべて`std::type_index`に依存している点が問題だった
```cpp
// =============== StateDefinition ===============
inline const std::type_index IDLE = typeid(MovementState::Idle);
inline const std::type_index MOVING = typeid(MovementState::Moving);

// =============== StateComponent ===============
struct MovementStateComponent
{
    std::type_index previous;
    std::type_index current; // 現在の状態を表す型のインデックス
};

// =============== Transition Request ===============
struct MovementFSMTransitionRequest 
{ 
    std::type_index requestedTo;
    uint32_t priority = 0; 
};

// =============== LeaseComponent ===============
struct MovementFSMLeaseComponent
{
	std::optional<std::type_index> forcedState;// 強制遷移先の状態型
	float remainingDurationSec = 0.0f;// 残り干渉時間
};
```

### 改善の方針
後のFSMEngineでは、FSM定義をC++型そのものではなく、Authoring用DTOとして記述するようにした．  

具体的には以下のようにした.

- 状態名・条件名・スロット名・Profile名を文字列DTOとして記述する
- 初期化(Build)時に検証したうえでRuntime用IDへ変換する
- ComponentではRuntime用IDを保持する

これにより、FSMの構造をC++型から切り離し、状態一覧の検証、デバッグ表示、データ駆動化、Profile差し替え  
を行いやすくすることを目指した．
```cpp
// =============== FSM Axis Definition ===============
AxisDTO axis;
axis.axis = "Movement";
axis.stateU = {"Idle", "Moving" };
axis.condU = { "canMove", "shouldStop", "AlwaysTrue" };
axis.slotU = { "Transition", "Stop" };
axis.profileU = { "Default" };

// =============== FSM Definition =============== 
FSMDTO fsm;
fsm.axis = "Movement";
fsm.states = { "Idle", "Moving" };
fsm.profiles = { "Default" };

// =============== transitions ===============
fsm.transitions = { { "Idle",   "Moving",   "Transition",   0 }, 
                    { "Moving", "Idle",     "Stop",         0 }
                  };

// =============== State Component ===============
struct MovementStateComp
{
	StateID curState =  kInvalidState;
	StateID prevState = kInvalidState;

	ProfileID curProf = Engine::FSM::Core::kInvalidProfile;	
    bool changedThisFrame = false;
};

```
Build時には、これらの文字列参照を検証し、RuntimeIDへ正規化する


### 改善結果
この変更により以下のことが可能になった
- `std::type_index`への依存を避けられるようになった
- FSMの定義を、C++型を追加せずに文字列DTO上で拡張・変更しやすくなった
- FSMBuild時に文字列の比較・参照の検証を行うことができるようになった

また、TransitionRequestやLeaseによって状態遷移要求や外部干渉を表現する発想自体は有効だったため、  
あとの設計でも「入力や外部要因を一度Requestとして集め、FSM が解決する」という方向に発展させた

Condition、EffectHook、Axis / Profile による差し替えについては、別セクションで整理する．

### 学び
- 動作確認を優先する初期段階では、C++型やvirtual interface による実装でも有効だった
- virtual interface による条件定義は柔軟性はある程度あるが、条件一覧の検証・データ化・デバッグ表示が難しい
- Runtimeの効率性と拡張性を両立するためには、Authoring用の表現と Runtime用の表現を分け、Build時に検証・正規化する構造が有効である
- 入力や外部干渉を直接状態変更に結び付けるのではなく、Requestとして集めてからFSMが解決する構造にすると、制御の優先度や責務を整理しやすい
