#include "Debug.h"
#include <exception>
#include <chrono>
#include <ctime>   

Debug* Debug::sharedInstance = nullptr;
std::list<std::string> Debug::m_logs;
void Debug::initialize()
{
	if (Debug::sharedInstance) throw std::exception("DebugLog already created");
	Debug::sharedInstance = new Debug();
}

void Debug::destroy()
{
	if (!Debug::sharedInstance) return;
	delete Debug::sharedInstance;
}

Debug* Debug::get()
{
	return sharedInstance;
}

void Debug::Log(std::string log)
{
	time_t t; // t passed as argument in function time()
	struct tm* tt; // decalring variable for localtime()
	time(&t); //passing argument to time()
	tt = localtime(&t);

	std::string dateTime = asctime(tt);
	//dateTime.erase('\n');
	std::string l = dateTime + log;
	m_logs.push_back(l);
}
