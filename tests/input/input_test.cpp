#include <ddknd/input/input.h>

#include <iostream>

int failures = 0;

void test_check_impl(bool condition, const char* expression, const char* file, int line)
{
    if(!condition)
    {
            std::cerr   << "FAILED: " << expression
                        << " at "     << file << ":" << line
                        << '\n';

            ++failures;
    }
}

#define TEST_CHECK(condition)   \
    test_check_impl((condition), #condition, __FILE__, __LINE__)


enum class Action
{
    MoveForward,
    MoveBackward,
    MoveLeft,
    MoveRight
};

int main()
{

    // ======================= test for input action systems =======================
    using Key = ::ddknd::input::Key;
    using InputMapping = ::ddknd::input::InputMapping;
    using ActionInputSystem = ::ddknd::input::ActionInputSystem;

    InputMapping inputMapping_{};

    inputMapping_.RegisterKeyMap(Key::W, Action::MoveForward);
    inputMapping_.RegisterKeyMap(Key::A, Action::MoveLeft);
    inputMapping_.RegisterKeyMap(Key::S, Action::MoveBackward);
    inputMapping_.RegisterKeyMap(Key::D, Action::MoveRight);

    auto move_forward = inputMapping_.GetActionID(Action::MoveForward);
    auto move_left = inputMapping_.GetActionID(Action::MoveLeft);
    auto move_backward = inputMapping_.GetActionID(Action::MoveBackward);
    auto move_right = inputMapping_.GetActionID(Action::MoveRight);

    TEST_CHECK(move_forward != InputMapping::InvalidID);
    TEST_CHECK(move_left != InputMapping::InvalidID);
    TEST_CHECK(move_backward != InputMapping::InvalidID);
    TEST_CHECK(move_right != InputMapping::InvalidID);

    TEST_CHECK(inputMapping_.GetActionFromKey(Key::W) == move_forward);
    TEST_CHECK(inputMapping_.GetActionFromKey(Key::A) == move_left);
    TEST_CHECK(inputMapping_.GetActionFromKey(Key::S) == move_backward);
    TEST_CHECK(inputMapping_.GetActionFromKey(Key::D) == move_right);

    TEST_CHECK(inputMapping_.GetKey(move_forward) == Key::W);
    TEST_CHECK(inputMapping_.GetKey(move_left) == Key::A);
    TEST_CHECK(inputMapping_.GetKey(move_backward) == Key::S);
    TEST_CHECK(inputMapping_.GetKey(move_right) == Key::D);

    // 未登録
    TEST_CHECK(inputMapping_.GetActionID(static_cast<Action>(999)) == InputMapping::InvalidID);
    TEST_CHECK(inputMapping_.GetActionFromKey(Key::F20) == InputMapping::InvalidID);
    TEST_CHECK(inputMapping_.GetKey(InputMapping::InvalidID) == InputMapping::InvalidKey);
    // ======================= test for input action systems =======================

    return failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}