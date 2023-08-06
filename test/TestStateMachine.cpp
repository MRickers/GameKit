#include "../src/core/StateMachine.hpp"
#include <catch2/catch_test_macros.hpp>

class StateMock : public gk::IBaseState
{
  void onCreate() override
  {
    m_input += "Create";
  }
  void onDestroy() override
  {
    m_input += "Destroy";
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

TEST_CASE("Add State", "[statemanager]")
{
  gk::StateMachine stateMachine;

  SECTION("RegisterState")
  {
    stateMachine.registerState(StateType::INIT, createState);
    REQUIRE(!stateMachine.hasState(StateType::INIT));
  }
}