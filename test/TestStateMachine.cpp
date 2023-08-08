#include "../src/core/StateMachine.hpp"
#include <catch2/catch_test_macros.hpp>

class StateMock : public gk::IBaseState
{
public:
  void onCreate() override
  {
    m_input += "OnCreate";
  }
  void onDestroy() override
  {
    m_input += "OnDestroy";
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
  void draw(const SDL_Renderer*) override
  {
    m_input += "Draw";
  }

  std::string m_input{""};
};

gk::BaseStatePtr createState()
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
  gk::StateMachine stateMachine;

  SECTION("InitState")
  {
    REQUIRE(stateMachine.currentState() == StateType::INIT);
  }
  SECTION("RegisterState")
  {
    stateMachine.registerState(StateType::INIT,
                               []() -> gk::BaseStatePtr
                               { return std::make_unique<StateMock>(); });
    REQUIRE(!stateMachine.hasState(StateType::INIT));
  }
  SECTION("SwitchStateSimple")
  {

    StateMock* statePtr = nullptr;

    stateMachine.registerState(StateType::MENU,
                               [&statePtr]() -> gk::BaseStatePtr
                               {
                                 auto state = std::make_unique<StateMock>();
                                 statePtr = state.get();
                                 return std::move(state);
                               });
    stateMachine.switchTo(StateType::MENU);
    REQUIRE(stateMachine.hasState(StateType::MENU));
    REQUIRE(stateMachine.currentState() == StateType::MENU);
    REQUIRE(statePtr->m_input == "OnCreateActivate");
  }
  SECTION("SwitchStates")
  {

    StateMock* menuStatePtr = nullptr;
    StateMock* initStatePtr = nullptr;

    stateMachine.registerState(StateType::INIT,
                               [&initStatePtr]() -> gk::BaseStatePtr
                               {
                                 auto state = std::make_unique<StateMock>();
                                 initStatePtr = state.get();
                                 return std::move(state);
                               });
    stateMachine.registerState(StateType::MENU,
                               [&menuStatePtr]() -> gk::BaseStatePtr
                               {
                                 auto state = std::make_unique<StateMock>();
                                 menuStatePtr = state.get();
                                 return std::move(state);
                               });
    stateMachine.switchTo(StateType::MENU);
    stateMachine.switchTo(StateType::INIT);
    REQUIRE(stateMachine.hasState(StateType::MENU));
    REQUIRE(stateMachine.hasState(StateType::INIT));
    REQUIRE(stateMachine.currentState() == StateType::INIT);
    REQUIRE(menuStatePtr->m_input == "OnCreateActivateDeactivate");
    REQUIRE(initStatePtr->m_input == "OnCreateActivate");
  }
}