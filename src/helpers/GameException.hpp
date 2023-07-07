#pragma once
#include <filesystem>
#include <stdexcept>
#include <string>

namespace gk
{
  class GameException : public std::exception
  {
  public:
    GameException(const std::string& msg, const int error,
                  const char* file = "", const int line = 0)
        : m_error{error}
        , m_msg{msg}
    {
      m_msg += " in ";
      m_msg +=
          std::filesystem::absolute(std::filesystem::path{file}.filename());
      m_msg += " on line " + std::to_string(line);
    }

    const char* what() const noexcept override
    {
      return m_msg.c_str();
    }

  private:
    int m_error{0};
    std::string m_msg{""};
  };

  void ExceptionHandler(std::exception_ptr eptr, const char* file,
                        const int line)
  {
    try
    {
      if (eptr)
        std::rethrow_exception(eptr);
    }
    catch (const std::exception& e)
    {
      throw GameException{e.what(), 0, file, line};
    }
  }

#define HandleException(eptr) ExceptionHandler(eptr, __FILE__, __LINE__)

#define GameException(message, error)                                          \
  GameException                                                                \
  {                                                                            \
    message, error, __FILE__, __LINE__                                         \
  }
} // namespace gk
