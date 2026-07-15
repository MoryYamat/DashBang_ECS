#pragma once

#include <bitset>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <vector>
#include <concepts>


namespace ddknd::input
{
    template <typename Action>
    concept ActionToIndexable = requires(Action action) {
        { static_cast<std::size_t>(action) } -> std::convertible_to<std::size_t>;
    };

    // Contains the GLFW-compatible keys currently used by the engine.
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

    enum class MouseButton : std::uint8_t
    {
        LEFT_CLICK,
        RIGHT_CLICK,
        MIDDLE_CLICK,
        COUNT
    };

    enum class MouseAxis : std::uint8_t
    {
        DeltaX,
        DeltaY,
        WheelY,
        COUNT
    };

    enum class InputSourceKind : std::uint8_t
    {
        Key,
        MouseAxis
    };

    struct InputSource
    {
        InputSourceKind kind = InputSourceKind::Key;
        Key key = Key::COUNT;
        MouseAxis mouseAxis = MouseAxis::COUNT;

        static InputSource FromKey(Key k)
        {
            InputSource s{};
            s.kind = InputSourceKind::Key;
            s.key = k;
            return s;
        }

        static InputSource FromMouseAxis(MouseAxis a)
        {
            InputSource s{};
            s.kind = InputSourceKind::MouseAxis;
            s.mouseAxis = a;
            return s;
        }
    };

    struct MouseState
    {
        bool first = true;
        double x = 0.0;
        double y = 0.0;
        double deltaX = 0.0;
        double deltaY = 0.0;

        double wheelY = 0.0;
    };

    /**
    * Provides platform-specific device input to the engine input layer.
    */
    class IInputBackend
    {
      public:
        virtual ~IInputBackend() = default;
        virtual void Update() = 0;
        virtual bool IsDown(Key k) const = 0;
        virtual bool IsMouseButtonDown(MouseButton mouseButton) const = 0;
        virtual const MouseState& Mouse() const = 0;
    };

    /**
    * Caches the current keyboard, mouse-button, and mouse state for one frame.
    */
    class DeviceInput
    {
      public:
        explicit DeviceInput(IInputBackend& bc) : backend_(bc) {}

        bool IsKeyDown(Key k) const
        {
            return curr_[ToIndex(k)];
        }

        bool IsMouseButtonDown(MouseButton button) const
        {
            return mouse_curr_[ToIndex(button)];
        }

        void Update()
        {
            // Poll the backend once per game frame before caching device state.
            backend_.Update();

            for (std::size_t i = 0; i < KeyCount(); i++)
            {
                curr_[i] = backend_.IsDown(static_cast<Key>(i));
            }

            for(std::size_t i = 0; i < MouseButtonCount(); i++)
            {
                mouse_curr_[i] = backend_.IsMouseButtonDown(static_cast<MouseButton>(i));
            }

            mouse_ = backend_.Mouse();
        }

        const MouseState& Mouse() const
        {
            return mouse_;
        }

        static constexpr std::size_t KeyCount()
        {
            return static_cast<std::size_t>(Key::COUNT);
        }

        static constexpr std::size_t MouseButtonCount()
        {
            return static_cast<std::size_t>(MouseButton::COUNT);
        }

      private:
        IInputBackend& backend_;
        MouseState mouse_{};

        std::bitset<static_cast<std::size_t>(Key::COUNT)> curr_;
        std::bitset<static_cast<std::size_t>(MouseButton::COUNT)> mouse_curr_;

        constexpr std::size_t ToIndex(Key k) const
        {
            return static_cast<std::size_t>(k);
        }

        constexpr std::size_t ToIndex(MouseButton button) const
        {
            return static_cast<std::size_t>(button);
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


    /**
    * Maps device inputs to compact internal action IDs.
    *
    * Gameplay action values are used as vector indices and should therefore
    * be dense, non-negative enum values.
    */
    class InputMapping
    {
      public:
        using id_type = std::uint32_t;
        using key_type = Key;
        using mouse_button_type = MouseButton;

        static constexpr id_type InvalidID = std::numeric_limits<id_type>::max();
        static constexpr key_type InvalidKey = Key::COUNT;

      private:
        std::size_t KeyToIndex(key_type k) const
        {
            return static_cast<std::size_t>(k);
        }

        template <typename Action>
        std::size_t ActionToIndex(Action action) const
        {
            return static_cast<std::size_t>(action);
        }

        bool IsValidKey(key_type key) const
        {
            auto idx = KeyToIndex(key);
            return idx < key_to_action_.size();
        }

      public:
        InputMapping() : key_to_action_(static_cast<std::size_t>(key_type::COUNT), InvalidID) {}

        /**
        * Registers a key for a gameplay action.
        * Action values are used as vector indices and should therefore be dense,
        * non-negative enum values.
        */
        template <ActionToIndexable Action>
        bool RegisterKeyMap(const key_type key, const Action action)
        {
            auto action_to_index = ActionToIndex(action);
            if (action_to_index >= action_to_id_.size())
            {
                action_to_id_.resize(action_to_index + 1, InvalidID);
            }

            id_type id = action_to_id_[action_to_index];

            if (id == InvalidID)
            {
                id = static_cast<id_type>(id_to_key_.size());
                action_to_id_[action_to_index] = id;
                id_to_key_.push_back(InvalidKey);
            }

            key_to_action_[KeyToIndex(key)] = id;
            id_to_key_[id] = key;

            return true;
        }

        template <ActionToIndexable Action>
        bool RegisterMouseAxisMap(const MouseAxis axis, const Action action)
        {
            auto action_to_index = ActionToIndex(action);
            if (action_to_index >= action_to_id_.size())
            {
                action_to_id_.resize(action_to_index + 1, InvalidID);
            }

            id_type id = action_to_id_[action_to_index];

            if (id == InvalidID)
            {
                id = static_cast<id_type>(id_to_key_.size());
                action_to_id_[action_to_index] = id;
                id_to_key_.push_back(InvalidKey);
            }

            const auto axisIdx = static_cast<std::size_t>(axis);
            if (axisIdx >= mouse_axis_to_action_.size())
            {
                mouse_axis_to_action_.resize(axisIdx + 1, InvalidID);
            }
            mouse_axis_to_action_[axisIdx] = id;

            return true;
        }

        template<ActionToIndexable Action>
        bool RegisterMouseButtonMap(const mouse_button_type mouseButton, const Action action)
        {
            auto action_to_index = ActionToIndex(action);
            if (action_to_index >= action_to_id_.size())
            {
                action_to_id_.resize(action_to_index + 1, InvalidID);
            }

            id_type id = action_to_id_[action_to_index];

            if (id == InvalidID)
            {
                id = static_cast<id_type>(id_to_key_.size());
                action_to_id_[action_to_index] = id;
                id_to_key_.push_back(InvalidKey);
            }

            const auto mouseButtonIdx = static_cast<std::size_t>(mouseButton);
            if (mouseButtonIdx >= mouse_button_to_action_.size())
            {
                mouse_button_to_action_.resize(mouseButtonIdx + 1, InvalidID);
            }
            mouse_button_to_action_[mouseButtonIdx] = id;

            return true;
        }

        template <ActionToIndexable Action>
        id_type GetActionID(Action action) const
        {
            const auto actionValue = ActionToIndex(action);

            if (actionValue >= action_to_id_.size())
            {
                return InvalidID;
            }

            return action_to_id_[actionValue];
        }

        key_type GetKey(id_type id) const
        {
            if (static_cast<std::size_t>(id) >= id_to_key_.size())
            {
                return InvalidKey;
            }

            return id_to_key_[id];
        }

        id_type GetActionFromKey(key_type key) const
        {
            if (!IsValidKey(key))
            {
                return InvalidID;
            }

            return key_to_action_[KeyToIndex(key)];
        }

        std::size_t GetActionCount() const
        {
            return id_to_key_.size();
        }

        id_type GetActionFromMouseAxis(MouseAxis axis) const
        {
            const auto idx = static_cast<std::size_t>(axis);
            if (idx >= mouse_axis_to_action_.size())
            {
                return InvalidID;
            }

            return mouse_axis_to_action_[idx];
        }

        id_type GetActionFromMouseButton(MouseButton button) const
        {
            const auto idx = static_cast<std::size_t>(button);
            if(idx >= mouse_button_to_action_.size())
            {
                return InvalidID;
            }
            return mouse_button_to_action_[idx];
        }

      private:
        // Indexed by gameplay action value; stores the internal action ID.
        std::vector<id_type> action_to_id_; 

        // Indexed by internal action ID; stores the keyboard binding, if any.
        std::vector<key_type> id_to_key_; 

        // Indexed by key; stores the internal action ID.
        std::vector<id_type> key_to_action_; 

        std::vector<id_type> mouse_axis_to_action_;
        std::vector<id_type> mouse_button_to_action_;
    };

    /**
    * Resolve mapped device inputs into per-frame gameplay action states.
    * 
    * Produces value, down, pressed, and released states for registered actions.
    */
    class ActionInputSystem
    {
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
        ActionInputSystem(Mapping& mapping) : mappings_(mapping) {}

        void Update(const DeviceInput& input);

        template <ActionToIndexable Action>
        bool IsDown(Action action) const
        {
            assert(IsValidAction(action));
            auto idx = ActionToIndex(action);
            return actions_[idx].down;
        }

        template <ActionToIndexable Action>
        bool IsPressed(Action action) const
        {
            assert(IsValidAction(action));
            const auto idx = ActionToIndex(action);
            return actions_[idx].pressed;
        }

        template <ActionToIndexable Action>
        bool IsReleased(Action action) const
        {
            assert(IsValidAction(action));
            const auto idx = ActionToIndex(action);
            return actions_[idx].released;
        }

        /**
        * Returns the current value of a registered gameplay action.
        * 
        * The argument must be a gameplay action, not key or MouseAxis.
        */
        template <ActionToIndexable Action>
        float GetValue(Action action) const
        {
            assert(IsValidAction(action));
            const auto idx = ActionToIndex(action);
            return actions_[idx].value;
        }

      private:
        Mapping& mappings_;
        std::vector<ActionState> actions_;
        std::vector<float> action_values_;
    };


} // namespace ddknd::input