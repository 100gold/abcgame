#include "import.h"
#include "all.h"

TimerCallbacks::EventMgr EventHiveOwner<unsigned long>::m_objevents;

void TimerCallbacks::process_timers()
{
	static Ogre::Timer global_timer;
	unsigned long time = global_timer.getMillisecondsCPU();
	m_objevents.activate(time);
}
