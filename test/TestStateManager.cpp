#include "../src/core/StateManager.hpp"
#include <catch2/catch_test_macros.hpp>

class StateMock : gk::IBaseState
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

TEST_CASE("Add State", "[statemanager]")
{
  gk::StateManager stateManager{nullptr};
}