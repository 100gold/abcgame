#pragma once

class TimerCallbacks : public EventHiveOwner<unsigned long>
{
public:
	static void process_timers();
};