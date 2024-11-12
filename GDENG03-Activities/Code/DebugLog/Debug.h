#pragma once
#include <list>
#include <string>
class Debug
{
public:
	static void initialize();
	static void destroy();
	static Debug* get();

	static void Log(std::string log);

private:
	Debug() = default;
	static Debug* sharedInstance;
	Debug(Debug const&) {}
	Debug& operator=(Debug const&) {};
	~Debug() = default;

	static std::list<std::string> m_logs;

	friend class DebugLogUI;
};

