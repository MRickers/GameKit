#include "GameKit/core/state_machine.hpp"
#include <catch2/catch_test_macros.hpp>

class StateMock : public gk::base_state
{
public:
  void on_create() override
  {
    m_input += "on_create";
  }
  void on_destroy() override
  {
    m_input += "on_destroy";
  }

  void activate() override
  {
    m_input += "Activate";
  }
  void deactivate() override
  {
    m_input += "Deactivate";
  }

  void update() override
  {
    m_input += "Update";
  }
  void draw(SDL_Renderer*) override
  {
    m_input += "Draw";
  }

  std::string m_input{""};
};

gk::base_state_ptr createState()
{
  return std::make_unique<StateMock>();
}

enum class StateType
{
  INIT,
  MENU,
  PLAY,
  PAUSE
};

TEST_CASE("Add/Switch State", "[statemanager]")
{
  gk::state_machine state_machine;

  SECTION("InitState")
  {
    REQUIRE(state_machine.currentState() == StateType::INIT);
  }
  SECTION("RegisterState")
  {
    state_machine.registerState(StateType::INIT,
                                []() -> gk::base_state_ptr
                                { return std::make_unique<StateMock>(); });
    REQUIRE(!state_machine.hasState(StateType::INIT));
  }
  SECTION("SwitchStateSimple")
  {

    StateMock* statePtr = nullptr;

    state_machine.registerState(StateType::MENU,
                                [&statePtr]() -> gk::base_state_ptr
                                {
                                  auto state = std::make_unique<StateMock>();
                                  statePtr = state.get();
                                  return std::move(state);
                                });
    state_machine.switchTo(StateType::MENU);
    REQUIRE(state_machine.hasState(StateType::MENU));
    REQUIRE(state_machine.currentState() == StateType::MENU);
    REQUIRE(statePtr->m_input == "on_createActivate");
  }
  SECTION("SwitchStates")
  {

    StateMock* menuStatePtr = nullptr;
    StateMock* initStatePtr = nullptr;

    state_machine.registerState(StateType::INIT,
                                [&initStatePtr]() -> gk::base_state_ptr
                                {
                                  auto state = std::make_unique<StateMock>();
                                  initStatePtr = state.get();
                                  return std::move(state);
                                });
    state_machine.registerState(StateType::MENU,
                                [&menuStatePtr]() -> gk::base_state_ptr
                                {
                                  auto state = std::make_unique<StateMock>();
                                  menuStatePtr = state.get();
                                  return std::move(state);
                                });
    state_machine.switchTo(StateType::MENU);
    state_machine.switchTo(StateType::INIT);
    REQUIRE(state_machine.hasState(StateType::MENU));
    REQUIRE(state_machine.hasState(StateType::INIT));
    REQUIRE(state_machine.currentState() == StateType::INIT);
    REQUIRE(menuStatePtr->m_input == "on_createActivateDeactivate");
    REQUIRE(initStatePtr->m_input == "on_createActivate");
  }
}

TEST_CASE("Remove", "[statemanager]")
{
  gk::state_machine state_machine;

  SECTION("Add/Remove")
  {
    StateMock* statePtr = nullptr;

    state_machine.registerState(StateType::PAUSE,
                                [&statePtr]() -> gk::base_state_ptr
                                {
                                  auto state = std::make_unique<StateMock>();
                                  statePtr = state.get();
                                  return std::move(state);
                                });
    state_machine.switchTo(StateType::PAUSE);
    state_machine.remove(StateType::PAUSE);
    REQUIRE(statePtr->m_input == "on_createActivate");
    state_machine.processRequests();
    REQUIRE(!state_machine.hasState(StateType::PAUSE));
  }
}

TEST_CASE("Update", "[statemanager]")
{
  gk::state_machine state_machine;

  SECTION("Simple")
  {
    StateMock* statePtr = nullptr;

    state_machine.registerState(StateType::PAUSE,
                                [&statePtr]() -> gk::base_state_ptr
                                {
                                  auto state = std::make_unique<StateMock>();
                                  statePtr = state.get();
                                  return std::move(state);
                                });
    state_machine.switchTo(StateType::PAUSE);
    state_machine.update();
    REQUIRE(statePtr->m_input == "on_createActivateUpdate");
  }
  SECTION("Transcendent")
  {
    StateMock* pauseStatePtr = nullptr;
    StateMock* playStatePtr = nullptr;
    StateMock* menuStatePtr = nullptr;

    state_machine.registerState(StateType::PAUSE,
                                [&pauseStatePtr]() -> gk::base_state_ptr
                                {
                                  auto state = std::make_unique<StateMock>();
                                  pauseStatePtr = state.get();
                                  state->set_transcendent(true);
                                  return std::move(state);
                                });
    state_machine.registerState(StateType::PLAY,
                                [&playStatePtr]() -> gk::base_state_ptr
                                {
                                  auto state = std::make_unique<StateMock>();
                                  playStatePtr = state.get();
                                  return std::move(state);
                                });
    state_machine.registerState(StateType::MENU,
                                [&menuStatePtr]() -> gk::base_state_ptr
                                {
                                  auto state = std::make_unique<StateMock>();
                                  menuStatePtr = state.get();
                                  return std::move(state);
                                });

    state_machine.switchTo(StateType::MENU);
    state_machine.switchTo(StateType::PLAY);
    state_machine.switchTo(StateType::PAUSE);
    REQUIRE(menuStatePtr->m_input == "on_createActivateDeactivate");
    REQUIRE(playStatePtr->m_input == "on_createActivateDeactivate");
    REQUIRE(pauseStatePtr->m_input == "on_createActivate");
    state_machine.update();

    REQUIRE(menuStatePtr->m_input == "on_createActivateDeactivate");
    REQUIRE(playStatePtr->m_input == "on_createActivateDeactivateUpdate");
    REQUIRE(pauseStatePtr->m_input == "on_createActivateUpdate");
  }
}

TEST_CASE("Draw", "[statemanager]")
{
  gk::state_machine state_machine;

  SECTION("Simple")
  {
    StateMock* statePtr = nullptr;

    state_machine.registerState(StateType::PAUSE,
                                [&statePtr]() -> gk::base_state_ptr
                                {
                                  auto state = std::make_unique<StateMock>();
                                  statePtr = state.get();
                                  return std::move(state);
                                });
    state_machine.switchTo(StateType::PAUSE);
    state_machine.draw(nullptr);
    REQUIRE(statePtr->m_input == "on_createActivateDraw");
  }
  SECTION("Transparent")
  {
    StateMock* pauseStatePtr = nullptr;
    StateMock* playStatePtr = nullptr;
    StateMock* menuStatePtr = nullptr;

    state_machine.registerState(StateType::PAUSE,
                                [&pauseStatePtr]() -> gk::base_state_ptr
                                {
                                  auto state = std::make_unique<StateMock>();
                                  pauseStatePtr = state.get();
                                  state->set_transparent(true);
                                  return std::move(state);
                                });
    state_machine.registerState(StateType::PLAY,
                                [&playStatePtr]() -> gk::base_state_ptr
                                {
                                  auto state = std::make_unique<StateMock>();
                                  playStatePtr = state.get();
                                  return std::move(state);
                                });
    state_machine.registerState(StateType::MENU,
                                [&menuStatePtr]() -> gk::base_state_ptr
                                {
                                  auto state = std::make_unique<StateMock>();
                                  menuStatePtr = state.get();
                                  return std::move(state);
                                });

    state_machine.switchTo(StateType::MENU);
    state_machine.switchTo(StateType::PLAY);
    state_machine.switchTo(StateType::PAUSE);
    REQUIRE(menuStatePtr->m_input == "on_createActivateDeactivate");
    REQUIRE(playStatePtr->m_input == "on_createActivateDeactivate");
    REQUIRE(pauseStatePtr->m_input == "on_createActivate");
    state_machine.draw(nullptr);

    REQUIRE(menuStatePtr->m_input == "on_createActivateDeactivate");
    REQUIRE(playStatePtr->m_input == "on_createActivateDeactivateDraw");
    REQUIRE(pauseStatePtr->m_input == "on_createActivateDraw");
  }
}