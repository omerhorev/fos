#pragma once

#include <functional>
#include <queue>
#include <vector>
#include <cstdbool>


#include "fos/fos-general.h"

namespace fos
{
	class timewheel_event
	{
	public:
		timewheel_event(std::function<void(tick_t)> event, tick_t ticks) :
			_ticks(ticks), _event(event)
		{

		}

	public:

		inline tick_t get_ticks() const
		{
			return _ticks;
		}

		inline void dispach() const
		{
			_event(_ticks);
		}

	private:

		tick_t _ticks;

		std::function<void(tick_t)> _event;
	};

	class timewheel_comparer
	{
	public:
	public:
	    bool operator() (timewheel_event event1, timewheel_event event2)
	    {
	    	tick_t tick1 = event1.get_ticks();
	    	tick_t tick2 = event2.get_ticks();

	        return tick1 > tick2;
	    }
	};

	class timewheel
	{
	public:
		void push(std::function<void(tick_t)> event, tick_t ticks)
		{
			_events_queue.push(*new timewheel_event(event, ticks));
		}

		inline const timewheel_event& top() const
		{
			return _events_queue.top();
		}

		void pop()
		{
			const timewheel_event& event = top();
			event.dispach();
			_events_queue.pop();
		}

		std::priority_queue<timewheel_event, std::vector<timewheel_event>, timewheel_comparer> _events_queue;
	};
}
