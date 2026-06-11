## 基本構造
raw input -> Requested intent -> Resolver(FSM) -> ResolvedIntent -> UpdateLogic -> UpdateRendering

### IntentRequest
ユーザインプット・AI・ネットワークなどから`RequestedIntent`を作成

### State
`RequestedIntent`・`Environment`・`CurrentState`から ActorState/FSM を更新

### IntentResolve
`RequestedIntent`と`State`から`AcceptedIntent`を作成

### ロジック
`AcceptedIntent`と`Stats`から`GamePlayState`を更新する



## 確認されている問題点

### AssetStoreのコンテナであるAssetStorage::vector<T>の動的な再割り当てによって発生するアドレスの不安定性に関連する問題。
- 問題は、`AssetStore::TryGet(AssetID<T>)` で取得した `Resource*`(T*) のアドレスが、内部コンテナ (`vector<T>` など) に依存していること
- `TryGet()`で取得したアドレスは不安定であるため、取得したら即座に使用することによって一時対策を行う
- 非同期ロード・大量のロードが発生するような場合別の構造的対策が必要