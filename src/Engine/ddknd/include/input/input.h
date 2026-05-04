#pragma once

#include <bitset>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <type_traits>
#include <vector>

namespace ddknd::input
{
    template <class>
    inline constexpr bool always_false_v = false;

    template<typename Action>
    concept ActionToIndexable = requires(Action action){ {static_cast<std::size_t>(action)} -> std::convertible_to<std::size_t>;};

    // TODO: add all keys of glfws
    enum class Key : std::uint16_t
    {
        NONE,
        SPACE,
        APOSTROPHE,
        COMMA,
        MINUS,
        PERIOD,
        SLASH,
        NUM0,
        NUM1,
        NUM2,
        NUM3,
        NUM4,
        NUM5,
        NUM6,
        NUM7,
        NUM8,
        NUM9,
        SEMICOLON,
        EQUAL,
        A,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
        I,
        J,
        K,
        L,
        M,
        N,
        O,
        P,
        Q,
        R,
        S,
        T,
        U,
        V,
        W,
        X,
        Y,
        Z,
        LEFT_BRACKET,
        BACKSLASH,
        RIGHT_BRACKET,
        GRAVE_ACCENT,
        ESCAPE,
        ENTER,
        TAB,
        BACKSPACE,
        INSERT,
        DEL,
        RIGHT_ARROW,
        LEFT_ARROW,
        DOWN_ARROW,
        UP_ARROW,
        PAGE_UP,
        PAGE_DOWN,
        HOME,
        END,
        CAPS_LOCK,
        SCROLL_LOCK,
        NUM_LOCK,
        PRINT_SCREEN,
        PAUSE,
        F1,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        F10,
        F11,
        F12,
        F13,
        F14,
        F15,
        F16,
        F17,
        F18,
        F19,
        F20,
        F21,
        F22,
        F23,
        F24,
        F25,
        KP_0,
        KP_1,
        KP_2,
        KP_3,
        KP_4,
        KP_5,
        KP_6,
        KP_7,
        KP_8,
        KP_9,
        KP_DECIMAL,
        KP_DIVIDE,
        KP_MULTIPLY,
        KP_SUBTRACT,
        KP_ADD,
        KP_ENTER,
        KP_EQUAL,
        LEFT_SHIFT,
        LEFT_CONTROL,
        LEFT_ALT,
        LEFT_SUPER,
        RIGHT_SHIFT,
        RIGHT_CONTROL,
        RIGHT_ALT,
        RIGHT_SUPER,
        MENU,
        COUNT
    };

    struct InputState
    {
        float value = 0.0f;
        bool down = false;
        bool pressed = false;
        bool released = false;
    };

    class IInputBackend
    {
      public:
        virtual ~IInputBackend() = default;
        virtual void Update() = 0;
        virtual bool isKeyPressed(Key k) const = 0;
    };

    class DeviceInput
    {
      public:
        DeviceInput(IInputBackend& bc) : backend_(bc) {}

        bool isPressing(Key k) const
        {
            return curr_[toIdx(k)];
        }

        void Update()
        {
            for (std::size_t i = 0; i < KeyCount(); i++)
            {
                curr_[i] = backend_.isKeyPressed(static_cast<Key>(i));
            }
        }

        static constexpr std::size_t KeyCount()
        {
            return static_cast<std::size_t>(Key::COUNT);
        }

      private:
        IInputBackend& backend_;

        std::bitset<static_cast<std::size_t>(Key::COUNT)> curr_;

        constexpr std::size_t toIdx(Key k) const
        {
            return static_cast<std::size_t>(k);
        }
    };

    struct ActionState
    {
        float value = 0.0f;
        bool down = false;
        bool pressed = false;
        bool released = false;

        void Update(float newValue)
        {
            bool wasDown = down;

            value = newValue;
            down = value != 0.0f;

            pressed = !wasDown && down;
            released = wasDown && !down;
        }
    };


    // @TODO: 疎な値群に対して、unordered_map<Action>と自動で切り替える処理の実装
    // Support sparse action values using an internal unordered_map.
    // Current implementation assumes action values can be used efficiently
    // as dense indices after conversion to std::size_t.
    // std::size_t に static_cast できる型なら Action として使える
    // Any type that can be statically cast to std::size_t can be used as an Action
    class InputMapping
    {
      public:
        using id_type = std::uint32_t;
        using key_type = Key;

        static constexpr id_type InvalidID = std::numeric_limits<id_type>::max();
        static constexpr key_type InvalidKey = Key::COUNT;

      private:
        template <typename Action>
        id_type ActionToValue(Action action) const
        {
            return static_cast<id_type>(action);
        }

        std::size_t KeyToIndex(key_type k) const
        {
            return static_cast<std::size_t>(k);
        }

        template <typename Action>
        std::size_t ActionToIndex(Action action) const
        {
            return static_cast<std::size_t>(action);
        }

        template <typename Action>
        bool IsValidActionID(Action action) const
        {
            auto idx = ActionToIndex(action);
            return idx < action_to_id_.size();
        }

        bool IsValidKey(key_type key) const
        {
            auto idx = KeyToIndex(key);
            return idx < key_to_action_.size();
        }

      public:
        InputMapping() : key_to_action_(static_cast<std::size_t>(key_type::COUNT), InvalidID) {}

        // @NOTE
        // Actions should be arranged as densely as possible.
        // Using unnecessarily large values ​​increases the internal data size and impairs cache locality.
        template <ActionToIndexable Action>
        bool RegisterKeyMap(const key_type key, const Action action)
        {
            auto action_to_index = ActionToIndex(action);
            if(action_to_index >= action_to_id_.size() )
                action_to_id_.resize(action_to_index + 1, InvalidID);

            id_type id = action_to_id_[action_to_index];
            
            if(id == InvalidID)
            {
                id = static_cast<id_type>(id_to_key_.size());
                action_to_id_[action_to_index] = id;
                id_to_key_.push_back(InvalidKey);
            }

            key_to_action_[KeyToIndex(key)] = id;
            id_to_key_[id] = key;

            return true;
        }

        template<ActionToIndexable Action>
        id_type GetActionID(Action action) const
        {
            const auto actionValue = ActionToIndex(action);

            if(actionValue >= action_to_id_.size())
                return InvalidID;

            return action_to_id_[actionValue];
        }

        key_type GetKey(id_type id) const
        {
            if(static_cast<std::size_t>(id) >= id_to_key_.size())
                return InvalidKey;

            return id_to_key_[id];
        }

        id_type GetActionFromKey(key_type key) const
        {
            if(!IsValidKey(key))
                return InvalidID;

            return key_to_action_[KeyToIndex(key)];
        }

        std::size_t GetActionCount()const
        {
            return id_to_key_.size();
        }

      private:
        std::vector<id_type> action_to_id_;         //  action -> inetrnal action id        (idx: action_value, value: internal action id)
        std::vector<key_type> id_to_key_;           // internal action id -> key            (idx: internal action id          , value: key)
        std::vector<id_type> key_to_action_;        // key -> internal action id            (idx: key         , value: internal action id)
    };

    // system (update state)
    class ActionInputSystem
    {
        // responsibility
        // dead zone
        // sensitivity
        // invert Y
        // hold / pressed / released
        // composite input
        // 複数デバイス統合
        // context切り替えa

        using Mapping = InputMapping;
        using id_type = Mapping::id_type;
        using key_type = Mapping::key_type;

        static constexpr id_type InvalidID = Mapping::InvalidID;
        static constexpr key_type InvalidKey = Mapping::InvalidKey;

      private:
        template <ActionToIndexable Action>
        std::size_t ActionToIndex(Action action) const
        {
            auto id = mappings_.GetActionID(action);
            assert(id != InvalidID);
            return static_cast<std::size_t>(id);
        }

        template <ActionToIndexable Action>
        bool IsValidAction(const Action action) const
        {
            auto idx = mappings_.GetActionID(action);
            return idx != InvalidID && static_cast<std::size_t>(idx) < actions_.size();
        }

      public:
        ActionInputSystem(Mapping* mapping) : mappings_(*mapping) {}

        void Update(const DeviceInput& input);

        template <ActionToIndexable Action>
        bool IsDown(Action action) const
        {
            assert(IsValidAction(action));
        }

        template <ActionToIndexable Action>
        bool IsPressed(Action action) const
        {            
            assert(IsValidAction(action));

        }

        template <ActionToIndexable Action>
        bool IsReleased(Action action) const
        {
            assert(IsValidAction(action));

        }

        template <ActionToIndexable Action>
        float GetValue(Action action) const
        {
            assert(IsValidAction(action));

        }

      private:
        Mapping& mappings_;
        std::vector<ActionState> actions_;
        std::vector<float> action_values_;
    };

    // std::unique_ptr<ddknd::input::IInputBackend>
    // CreateGlfwInputBackend(const ddknd::window::Window& w);
} // namespace ddknd::input

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
