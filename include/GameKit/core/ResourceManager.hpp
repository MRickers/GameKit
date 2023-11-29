#pragma once
#include <filesystem>
#include <fstream>
#include <string>
#include <unordered_map>

namespace gk
{
  template <typename Derived, typename T> class ResourceManager
  {
  public:
    ResourceManager() = default;

    ResourceManager(const std::string& loadFile)
        : m_resources{}
        , m_paths{}
    {
      loadConfig(loadFile);
    }

    virtual ~ResourceManager()
    {
    }

    T* getResource(const std::string& id)
    {
      auto res = find(id);
      return (res ? res->first : nullptr);
    }

    std::string getPath(const std::string& id)
    {
      auto path = m_paths.find(id);
      return (path != m_paths.end() ? path->second : "");
    }

    bool requireResource(const std::string& id)
    {
      if (auto res = find(id); res)
      {
        ++res->second;
        return true;
      }
      if (auto path = m_paths.find(id); path != m_paths.end())
      {
        T* resource = load(path->second);
        if (!resource)
        {
          return false;
        }
        m_resources.emplace(id, std::make_pair(resource, 1));
        return true;
      }
      return false;
    }

    bool releaseResource(const std::string& id)
    {
      if (auto res = find(id); res)
      {
        --res->second;
        if (!res->second)
        {
          unload(id);
        }
        return true;
      }
      return false;
    }

    void purgeResources()
    {
      while (m_resources.begin() != m_resources.end())
      {
        delete m_resources.begin()->second.first;
        m_resources.erase(m_resources.begin());
      }
    }

    T* load(const std::string& path)
    {
      return static_cast<Derived*>(this)->load(path);
    }

    std::pair<T*, uint16_t>* find(const std::string& id)
    {
      auto itr = m_resources.find(id);
      return (itr != m_resources.end() ? &itr->second : nullptr);
    }

    bool unload(const std::string& id)
    {
      if (auto itr = m_resources.find(id); itr != m_resources.end())
      {
        static_cast<Derived*>(this)->unload(itr->second->first);
        m_resources.erase(itr);
        return true;
      }
      return false;
    }

    bool loadConfig(const std::string& pathFile)
    {
      std::ifstream paths;
      paths.open(std::filesystem::current_path().string() + pathFile);
      if (paths.is_open())
      {
        std::string line;
        while (std::getline(paths, line))
        {
          std::stringstream keyStream(line);
          std::string name;
          std::string path;
          // grab everthing till space dilimeter
          keyStream >> name;
          keyStream >> path;
          m_paths.emplace(name, path);
        }
        paths.close();
        return true;
      }
      return false;
    }

  private:
    std::unordered_map<std::string, std::pair<T*, uint16_t>> m_resources;
    std::unordered_map<std::string, std::string> m_paths;
  };

} // namespace gk
