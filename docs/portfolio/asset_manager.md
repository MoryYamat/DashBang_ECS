# Asset Pipeline

ここでは、現行実装における、Asset Pipeline の構造について説明する．


## 目的
Asset Pipeline は次のことを目的に設計した．

- Engine 外部 から Import する File / Asset を runtime で効率的に管理・再利用できるようにすること
- コード内に Asset Import Path を 散逸させることなく、コード内で扱いやすくすること
- 非同期ロード対応などバックエンド動作の変更や拡張をあとから行いやすい構造にすること

## 実現したこと

- virtual path (vpath) から AssetID を発行する
- AssetID と virtual path の対応を保持する
- Asset の load state を管理する
- AssetLoader によって 明示的に Asset をロードする
- Import / Build した Runtime Resource を AssetStore に登録する
- Runtime 側では AssetID を使ってロード済み Resource を取得する


## 現行実装の全体構造

```
Asset Path / vpath
    ↓
AssetManager         : AssetID発行 / vpath-AssetID 対応 / load state 管理
    ↓
AssetLoader         : vpath を 実 path に解決 / file import と resource build を実行
    ↓
RuntimeResource     : ModelRenderResource などの リソースを保持
    ↓
AssetStore          : AssetID を index として Runtime Resource を保持
    ↓
Runtime Systems     : AssetID から必要な Resource を取得して使用
```

## 主要要素

ここでは Asset Pipeline における、主な構成要素の責務を説明する．

### Asset Manager
**Assetの実データを持たず**、AssetID、vpath、load state などの管理情報を扱う．

主な責務:
- vpath から AssetID を発行する
- AssetID と vpath の対応を保持する
- Asset 種別ごとのAsset Table として Asset の load state を管理する


### Asset Loader
AssetID に対応する vpath を用いて実際の load を行う．

主な責務:
- Importer / Builder / Backend を使って Runtime Resource を構成する
- 構成した Runtime Resource を AssetStore へ登録する
- Asset の load state を更新する


### Asset Store
AssetStore は、 構成済み Runtime Resource を AssetID に対応させて保持する．

主な責務:
- AssetID を index として Runtime Resource を保持する
- Runtime System から AssetID によって Runtime Resource を取得できるようにする
- AssetManager から実データの保持責務を分離する


### Runtime Resource
実ファイルデータからビルドされる、runtime 用のデータ．

- runtime で参照するためのデータ
- ファイルデータから内部表現用に変換されることで、構成される
- ModelRenderResource / AnimationClipResource/ TextureResource など
- Backend 固有 resource への ID を保持する場合がある


### Backend
Backend は、OpenGL などの backend 固有 resource を保持する

Texture や Shader などは、 runtime 側では直接 `GLuint`を扱わない．  
`GPUID`などの backend resource ID を通して参照する形にすることで、backend 実体を隠ぺいする．

主な責務:
- GPU / Audio などの backend 固有 Resource の保持


## この構造にした理由

### Asset の識別と実データ保持を分離するため
AssetManager にロード実行やリソース保持の責務を持たせると 責務が肥大化してしまう．  
そこで、AssetManager は AssetID の発行を主として、メタ情報の管理を行うようにした．  

AssetLoader や AssetStore は AssetID を共通の参照単位として、ロードやリソース保持を行う．  
これによって、Asset 識別、ロード状態、実データ保持の責務を分離しつつ、一貫したっ参照方法を維持できる．


### Runtime では文字列ではなく、ID で参照解決するようにするため
パフォーマンスや保守性の観点から、runtime では可能な限り生の文字列を使わないようにしたい．  
そこで、初期化時に AssetID を発行し、Runtime では AssetID を通して Resource を取得する構造にした．

これによって、文字列の比較コストを避け、依存関係の整理が行いやすくなった．  
また、AssetID を使いまわすことで、同一Asset を簡単に再利用できるようになった．


### 明示ロードと将来の非同期ロード対応を行いやすくするため
ロード処理を AssetLoader に分離することで、同期ロード、遅延ロード、非同期ロードなどの実行方式変更をしやすくすることを目指した．


## 現行実装の課題

### 非同期ロードへの拡張
現行実装は、同期ロードのみであり、非同期ロードは実装できていない．  
構造的にはロードの実行方式を変更しやすいため、非同期ロードを実装していくなかで、  
ロード完了通知などの仕組みを充実させていきたいと考えている．


### Asset 定義のデータ駆動化 / Editor 化
現行実装では、使用する Asset path や sub asset key を C++ コード上で定義している．  
Tool や Editor 機能を拡張していく中で、Asset 定義を data 側へ移し、  
内部データ表現の形式も安定させていくことが重要だと考えている．


### Resource 参照の寿命管理

AssetStore が返す Resource ポインタ の寿命や、再ロード・アンロード時の仕組みや扱いは今後整備していく必要がある．