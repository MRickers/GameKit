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
  void draw(SDL_Renderer*) override
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

TEST_CASE("Remove", "[statemanager]")
{
  gk::StateMachine stateMachine;

  SECTION("Add/Remove")
  {
    StateMock* statePtr = nullptr;

    stateMachine.registerState(StateType::PAUSE,
                               [&statePtr]() -> gk::BaseStatePtr
                               {
                                 auto state = std::make_unique<StateMock>();
                                 statePtr = state.get();
                                 return std::move(state);
                               });
    stateMachine.switchTo(StateType::PAUSE);
    stateMachine.remove(StateType::PAUSE);
    REQUIRE(statePtr->m_input == "OnCreateActivate");
    stateMachine.processRequests();
    REQUIRE(!stateMachine.hasState(StateType::PAUSE));
  }
}

TEST_CASE("Update", "[statemanager]")
{
  gk::StateMachine stateMachine;

  SECTION("Simple")
  {
    StateMock* statePtr = nullptr;

    stateMachine.registerState(StateType::PAUSE,
                               [&statePtr]() -> gk::BaseStatePtr
                               {
                                 auto state = std::make_unique<StateMock>();
                                 statePtr = state.get();
                                 return std::move(state);
                               });
    stateMachine.switchTo(StateType::PAUSE);
    stateMachine.update();
    REQUIRE(statePtr->m_input == "OnCreateActivateUpdate");
  }
  SECTION("Transcendent")
  {
    StateMock* pauseStatePtr = nullptr;
    StateMock* playStatePtr = nullptr;
    StateMock* menuStatePtr = nullptr;

    stateMachine.registerState(StateType::PAUSE,
                               [&pauseStatePtr]() -> gk::BaseStatePtr
                               {
                                 auto state = std::make_unique<StateMock>();
                                 pauseStatePtr = state.get();
                                 state->setTranscendent(true);
                                 return std::move(state);
                               });
    stateMachine.registerState(StateType::PLAY,
                               [&playStatePtr]() -> gk::BaseStatePtr
                               {
                                 auto state = std::make_unique<StateMock>();
                                 playStatePtr = state.get();
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
    stateMachine.switchTo(StateType::PLAY);
    stateMachine.switchTo(StateType::PAUSE);
    REQUIRE(menuStatePtr->m_input == "OnCreateActivateDeactivate");
    REQUIRE(playStatePtr->m_input == "OnCreateActivateDeactivate");
    REQUIRE(pauseStatePtr->m_input == "OnCreateActivate");
    stateMachine.update();

    REQUIRE(menuStatePtr->m_input == "OnCreateActivateDeactivate");
    REQUIRE(playStatePtr->m_input == "OnCreateActivateDeactivateUpdate");
    REQUIRE(pauseStatePtr->m_input == "OnCreateActivateUpdate");
  }
}

TEST_CASE("Draw", "[statemanager]")
{
  gk::StateMachine stateMachine;

  SECTION("Simple")
  {
    StateMock* statePtr = nullptr;

    stateMachine.registerState(StateType::PAUSE,
                               [&statePtr]() -> gk::BaseStatePtr
                               {
                                 auto state = std::make_unique<StateMock>();
                                 statePtr = state.get();
                                 return std::move(state);
                               });
    stateMachine.switchTo(StateType::PAUSE);
    stateMachine.draw(nullptr);
    REQUIRE(statePtr->m_input == "OnCreateActivateDraw");
  }
  SECTION("Transparent")
  {
    StateMock* pauseStatePtr = nullptr;
    StateMock* playStatePtr = nullptr;
    StateMock* menuStatePtr = nullptr;

    stateMachine.registerState(StateType::PAUSE,
                               [&pauseStatePtr]() -> gk::BaseStatePtr
                               {
                                 auto state = std::make_unique<StateMock>();
                                 pauseStatePtr = state.get();
                                 state->setTransparent(true);
                                 return std::move(state);
                               });
    stateMachine.registerState(StateType::PLAY,
                               [&playStatePtr]() -> gk::BaseStatePtr
                               {
                                 auto state = std::make_unique<StateMock>();
                                 playStatePtr = state.get();
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
    stateMachine.switchTo(StateType::PLAY);
    stateMachine.switchTo(StateType::PAUSE);
    REQUIRE(menuStatePtr->m_input == "OnCreateActivateDeactivate");
    REQUIRE(playStatePtr->m_input == "OnCreateActivateDeactivate");
    REQUIRE(pauseStatePtr->m_input == "OnCreateActivate");
    stateMachine.draw(nullptr);

    REQUIRE(menuStatePtr->m_input == "OnCreateActivateDeactivate");
    REQUIRE(playStatePtr->m_input == "OnCreateActivateDeactivateDraw");
    REQUIRE(pauseStatePtr->m_input == "OnCreateActivateDraw");
  }
}