#pragma once

template<class Context>
class EventHive
{
public:
	class Event
	{
	public:
		virtual void activate(Context& ctx) = 0;
	};
	typedef boost::shared_ptr<Event> EventPtr;

private:
	std::vector<EventPtr> m_events;

public:
	void activate(Context& ctx)
	{
		BOOST_FOREACH(EventPtr& x, m_events)
		{
			x->activate(ctx);
		}
	}

	void delete_event(EventPtr ptr)
	{
		for (auto i=m_events.begin(); i!=m_events.end(); ++i)
		{
			if (*i == ptr)
			{
				m_events.erase(i);
				return;
			}
		}
	}

	template<class Requestor>
	EventPtr make_new_event(Requestor& req)
	{
		class RealEvent : public Event
		{
			Requestor& m_requestor;
		public:
			RealEvent(Requestor& req) :
				m_requestor(req)
			{
			}
			void activate(Context& ctx)
			{
				m_requestor(ctx);
			}
		};
		EventPtr ptr(new RealEvent(req));
		m_events.push_back(ptr);
		return ptr;
	}
};

template<class Context>
class EventHiveOwner
{
public:
	typedef EventHive<Context> EventMgr;
	typedef typename EventMgr::EventPtr EventHandle;

protected:
	static EventMgr m_objevents;

public:
	template<class Requestor>
	static EventHandle subscribe(Requestor& req)
	{
		return m_objevents.make_new_event(req);
	}
	static void unsubscribe(EventHandle handle)
	{
		m_objevents.delete_event(handle);
	}
};