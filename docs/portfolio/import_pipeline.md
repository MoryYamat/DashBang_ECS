# Import Pipeline

ここでは、外部ファイルのデータ形式の変換フローに焦点を当てた、Import Pipeline について説明する．  

なお、Asset の全般的な管理は、[こちらの資料](./asset_manager.md) に記載する．

## もくじ
- [目的](#目的)
- [実現したこと](#実現したこと)
- [現行実装の全体構造](#現行実装の全体構造)
- [主要要素](#主要要素)
- [初期実装の課題](#初期実装の課題)
- [改善方針](#改善方針)
- [改善結果](#改善結果)
- [現行実装の課題](#現行実装の課題)

## 目的
Import Pipeline は次のことを目的に設計した．

- 外部ファイル形式の違いを ImportData / Runtime Resource への変換段階で吸収し、  
runtime 側がファイル形式に直接依存しないようにすること
- 同一ファイルに含まれる SubAsset を 整合的に取り出し、識別子を付与して使えるようにすること
- Importer と Runtime Resource Builder を分離して、  
ファイル読み取りとruntime 用データ構築の責務を分けること


## 実現したこと

- `.glb` を ImportData として import する
- ImportData から、Runtime Resource と Backend Resource をビルドする
- `.glb`に含まれる`Animation` SubAsset に AssetID と vpath を発行する


## 現行実装の全体構造

```txt
Asset Loader
    ↓
External File (.glb)
    ↓
Importer
    ↓
ImportData
    ↓
Runtime Resource Builder
    ↓
Runtime Resource / Backend Resource
```

## 主要要素

### 1. Asset Loader
Asset Loader は Asset Pipeline 側から Import / Build するための入り口である．

責務:
- AsestID から 実Path を解決する
- Importer を呼び ImportData を構成する
- Builder を呼び Runtime Resource を構成する
- AssetStore への登録や load state の更新を行う

### 2. Importer
ファイルデータを ImportData に変換する処理.  

ファイル形式に対応した処理を実装する必要がある．

主な責務:
- ファイルデータを ImportData として構成する

### 3. ImportData
外部ファイルから読み取った結果を保持する中間データ表現である．  

できる限りファイル形式の内部構造を保ち、Runtime resource へ変換する前の確認・変換境界として扱う．

例えば `.glb` から構成する `ModelImportData` では、  
scene / node / mesh / skin / animation / material / texture などの参照関係を、  
glTF に近い形で保持する。

主な責務:
- ファイルデータを Engine に格納するためのデータ構造
- Runtime Resource に変換する前の中間データ表現

### 4. Runtime Resource Builder
ImportData を Runtime Resource に変換する処理．  

runtime で共通に使用することを目的として定義された、Runtime Resource へ変換する

主な責務:
- ImportData を Runtime Resource へ変換する
- ImportData から GPU Backend などに依存する、Backend Resource へ変換する

### 5. Runtime Resource
ファイル形式による差異を吸収した runtime データ構造として定義．  

Backend 固有のリソースへの識別子をIDで持つ場合がある．

主な責務:
- ファイル形式による差異を吸収する
- `GLuint`などのbackend 固有リソースへの参照IDなどを保持


### 6. Backend Resource
GPU 処理や 音声処理など各Assetのbackend 固有のリソース．  
`GLuint`など runtime コードに広範に露出したくないデータをBackend Resource として隠ぺいする.

主な責務:
- Runtime Resource に変換された際の、Backend 固有のリソース


## 初期実装の課題

次に、現行実装以前の、初期実装における課題を挙げ、どのように改善することを目指したかを説明する．

### 1. ファイル読み取りと Runtime Resource 構築の責務が混ざっていた

初期実装は、ファイル形式のパース後、Runtime Resource の構築が直接行われていた．  
これには次のような問題があった．

- Runtime Resource のデータ構造の変更の影響が Importer 処理に直接波及する構造になっていた
- ファイル形式の変更や多様化がしづらい実装になっていた
- Importer と Runtime Resouce の密結合によって、ロードタイミングの制御の導入が難しくなっていた


### 2. SubAsset に論理的識別子を付与して管理できていなかった

同一ファイルに含まれるSubAssetを明示的に管理していなかった．  

これによって次のような問題があった．

- runtime で SubAsset の動的な差し替えが難しいこと  
- Asset の 親子関係や階層構造の構築が難しいこと
- Runtime Resource のデータ構造の肥大化


## 改善方針
これらの問題に対応するために、現行実装では以下の点を重視して設計を行った．

### 1. Import / Build のフェーズ分離
外部ファイルの変換を以下のようにフェーズ分離する．

- Importer / ImportData フェーズ        :   
    ファイルデータ形式固有のデータ構造をできるだけ保つ
- ResourceBuilder/ Resource フェーズ    :  
    ファイル形式の差異をなくし、runtime で扱いやすいデータ構造を保つ


### 2. SubAsset を Asset として識別できるようにする
SubAsset に対して変換段階で明示的にAssetIDを付与する．

- AssetManager などの管理システムの機能と連携する

## 改善結果
これらの設計方針の結果以下のように改善できたと考えられる．

### 1. フェーズ分離によって、Backend や 拡張子の変更の影響波及が局所化できた

Importer / ImportData フェーズで、各ファイル形式のデータ構造を保つことによって、  
Runtime Resource データ構造の変更の影響が Importer 処理に波及しづらくなった．

また、Runtime Resource Builder の実装によって、ファイル形式の変更に対応しやすくなった．


### 2. SubAsset に AssetID を発行し、明示的に管理できるようになった

SubAsset に AssetID を発行することで、runtime で SubAsset を明示的かつ動的に扱いやすくなった．

## 現行実装の課題

現行実装で確認している課題には以下のようなものがある．

### 1. 外部 Animation Asset への対応
外部ファイル定義された SubAsset を Engine 内部で整合的に扱う仕組みが弱い．  
例えば、3Dモデルに対して、外部ファイルとして定義されたボーン Animation を  
Runtime Resource で適切に扱う実装が存在しない．

このため、SubAsset をより柔軟に切り替えることや、SubAssetを共通リソースとして使いまわすことが難しくなっている．

### 2. VFX や SFX などの各種 Asset への対応

現在実装しているのは、`.glb`のみである．  

Asset には VFX や SFX など様々なものが存在するため、  
これらの各種Assetにも同じ方式が有用かどうかは検証が必要である．

また、`.glb`ファイルは仕様が公開されており、ImportDataやRuntime Resourceへの変換が実装しやすい．  

`.fbx`などの形式では、仕様や利用可能な importer library によって実装方針が変わるため、  
ImportData / Runtime Resource への変換境界を保てるか検討が必要である。

### 3. ImportData / Runtime Resource の検証とテスト

ImportData と Runtime Resource の境界を分けたことで、  
各段階を検証しやすくなった一方で、変換結果の正しさを保証するテストは十分でない．  

今後は、mesh / skeleton / animation / material などの変換結果を確認するテストや  
debug output を整備する必要がある．