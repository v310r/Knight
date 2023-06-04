#pragma once

#include <unordered_map>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <fstream>

template<typename Derived, typename T>
class ResourceManager
{
public:
	ResourceManager(const std::string& fileContainingPaths)
	{
		LoadPaths(fileContainingPaths);
	}

	~ResourceManager() 
	{ 
		//PurgeResources(); 
	}

	bool RequireResource(const std::string& id)
	{
		auto res = Find(id);
		if (res)
		{
			auto& [resource, referenceCounter] = *res;
			++referenceCounter;
			return true;
		}
		

		std::string pathToResourse = GetPath(id);
		if (pathToResourse.empty())
		{
			return false;
		}

		T* resource = Load(pathToResourse);
		if (!resource)
		{
			return false;
		}

		// first time the resource is needed
		m_Resources.emplace(id, std::make_pair(resource, 1)); 
		return true;
	}

	bool ReleaseResource(const std::string& id)
	{
		auto res = Find(id);
		if (!res)
		{
			return false;
		}

		auto& [resource, referenceCounter] = *res;

		// if the resource is no longer used by anything, then unload it
		--referenceCounter;

		if (referenceCounter == 0)
		{
			Unload(id);
		}

		return true;
	}

	void PurgeResources()
	{
		while (m_Resources.begin() != m_Resources.end())
		{
			auto& [resourceName, res] = *m_Resources.begin();
			auto& [resource, referenceCounter] = res;

			delete resource;
			resource == nullptr;
			m_Resources.erase(m_Resources.begin());
		}
	}

	T* Load(const std::string& pathToFile)
	{
		return static_cast<Derived*>(this)->Load(pathToFile);
	}

	T* GetResource(const std::string& id)
	{
		auto res = Find(id);
		return (res ? res->first : nullptr);
	}

	std::string GetPath(const std::string& id)
	{
		auto path = m_Paths.find(id);
		return (path != m_Paths.end() ? path->second : "");
	}

private:
	std::pair<T*, unsigned int>* Find(const std::string& id)
	{
		auto iter = m_Resources.find(id);
		return (iter != m_Resources.end() ? &iter->second : nullptr);
	}

	bool Unload(const std::string& id)
	{
		auto iter = m_Resources.find(id);
		if (iter == m_Resources.end())
		{
			return false;
		}

		auto& [resourceName, res] = *iter;
		auto& [resource, referenceCounter] = res;

		delete resource;
		resource = nullptr;
		m_Resources.erase(iter);

		return true;
	}

	void LoadPaths(const std::string& fileContainingPaths)
	{
		std::ifstream paths;
		paths.open(std::filesystem::current_path() / fileContainingPaths);

		if (paths.is_open())
		{
			std::string line;
			while (std::getline(paths, line))
			{
				std::stringstream keystream(line);
				std::string pathName;
				std::string path;

				keystream >> pathName;
				keystream >> path;
				m_Paths.emplace(pathName, path);
			}
			paths.close();
			return;
		}
		
		std::cerr << "Failed to load the file which contains paths: " << fileContainingPaths << ", src: " << __FILE__ << std::endl;
	}

	std::unordered_map<std::string, std::pair<T*, unsigned int>> m_Resources;
	std::unordered_map<std::string, std::string> m_Paths;
};
