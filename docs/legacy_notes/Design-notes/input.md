# raw input abstracted input の接続とAPI設計のメモ

TODOメモ
```cpp
// @TODO: 疎な値群に対して、unordered_map<Action>と自動で切り替える処理の実装
// TODO:
// Reconsider the Action API.
// Current template-based API accepts any type convertible to std::size_t,
// which is flexible but too permissive.
// For example, Key / MouseAxis can accidentally be passed as an Action.
// Consider introducing a stronger ActionID type or requiring user-defined
// action traits/concepts.
// Action API の設計を再検討する。
// 現在は std::size_t に static_cast できる型を Action として受け入れているため柔軟だが、
// Key / MouseAxis など Action ではない enum も誤って渡せてしまう。
// 将来的には ActionID 型の導入、または ActionTraits / concept による制約強化を検討する。
```

設計メモ
```cpp

// memo
// 普通
// 1. Register 方式
// auto id = reg.Register("move_foward");
// メリット:
// - IDが一意
// - 内部は整数で高速
// - Engine主導で管理
// デメリット:
// - 初期化が必要(Register呼び出し)
// - IDをどこかで保持する必要がある
// - 所有者問題が発生

// 2. 文字列方式
// input.Pressed("move_forward");
// - シンプル
// - 初期化不要・所有者不要・柔軟
// デメリット
// - 比較コストが高い
// - typo / コンパイルチェックが効かない

// 3. ハッシュ方式
// ActionID("move_foward")// -> hash  // struct ActionID{ std::uint32_t v; explicit ActionID(std::string_view name):
// v(hash(name)){}}; メリット
// - register不要
// - 所有者不要・比較は高速・ユーザ文字列で定義できる
// デメリット:
// (理論上)衝突の可能性
// デバッグしづらい
// typo検出不可

// 案 (DI)
// User -> Actionの意味とID変換規則を持つ
// Engine -> その規則だけを呼ぶ
// API契約: typename UserAction -> std::uint32_t
// 例:
// USER:
//      enum class Action : std::uint32_t { move_fwd, move_back, ...}; or struct Action{ std::uint32_t v;} inline
//      constexpr Action MoveFoward{0};... if(input.Pressed(Action::move_fwd)){...} or
//      if(input.Pressed(MoveForward)){...}
// ENGINE:
//      template<typename Action>
//      std::uint32_t ToActionID(Action Action)
//      { return static_cast<std::uitn32_t>(action.value);  }// API契約: 変換可能であること(UserAction -> std::uint32_t)
// メリット:
// - **所有者不要**
// - Register()不要
// - **型安全**
// - **高速(整数比較)**
// - **ユーザ定義が自由(API契約を守れば)**
// - **Engineが意味を知らない**
// - 拡張可能
// デメリット
// - ID管理がユーザ責任
// - 名前情報がない(デバッグ性)
// - 動的追加が難しい(enum では 不可)
```

