# asset manager の設計メモ


```cpp

// @TODO:
// LOW: 
// - asset_managerとvpath 解決の 境界と責務分離(AssetID<ModelTag> が一意に file + scene を指す)
//  課題:
//  - 現状: vpath-AssetID の直接対応(vpath内の違いを区別できない)
//  改善案:
//      AssetT::create_uri(vpath, options)
//          ↓
//      AssetID::GetOrCreateID(uri)(IR:similar to URI)
//          ↓
//      AssetT::parse_key(uri)
//          ↓
//      absPath + load options

// @TODO
// LOW:
// Editor機能との関連
// asset_id は ゲームオブジェクトの意味論(意味を持つ概念の区分)と対応するようにする
// そのために、asset_manager::create_asset_id(key)の入力である key は 事前に一意の識別子(uri)を生成する必要がある
// これはfile に含まれる sub asset のパース であり、ゲームエンジンのeditor機能に含まれる
// uriの仕様(案):
// - file + selector:
//      model key: res://foo.glb#scene=0 / res://foo.glb#animation=0 など
```