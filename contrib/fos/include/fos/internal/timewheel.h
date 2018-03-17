#pragma once

#include <functional>
#include <queue>
#include <vector>
#include <cstdbool>


#include "fos/fos-general.h"

namespace fos
{
namespace internal
{
	struct timewheel_event
	{

		timewheel_event(std::function<void(tick_t)> event, tick_t ticks) :
			ticks(ticks), event(event) {}

		inline void dispach() const
		{
			event(ticks);
		}

		bool operator < (const timewheel_event& event) const
		{
			return ticks < event.ticks;
		}

		bool operator > (const timewheel_event& event) const
		{
			return ticks > event.ticks;
		}

		tick_t ticks;

		std::function<void(tick_t)> event;
	};


	class timewheel
	{
	public:
		void push(std::function<void(tick_t)> event, tick_t ticks)
		{
			_events_queue.push(timewheel_event(event, ticks));
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

		inline size_t size() const
		{
			return _events_queue.size();
		}

		inline size_t empty() const
		{
			return _events_queue.size() == 0;
		}

		std::priority_queue<timewheel_event> _events_queue;
	};
}
}
