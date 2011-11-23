#include "import.h"
#include "all.h"
#include <boost/test/unit_test.hpp>

namespace
{

class ReqA
{
public:
	int m_value;
	ReqA()
	{
		m_value = 0;
	}
	void operator()(int v)
	{
		m_value = v;
	}
};

class ReqB
{
public:
	int m_value;
	ReqB()
	{
		m_value = 0;
	}
	void operator()(int v)
	{
		m_value = v;
	}
};

BOOST_AUTO_TEST_SUITE(eventhandle_suite)

BOOST_AUTO_TEST_CASE(eventhandle_test0)
{
	EventHive<int> hive;
	ReqA a;
	ReqB b;
	auto evta = hive.make_new_event(a);
	auto evtb = hive.make_new_event(b);
	int y = 1;
	hive.activate(y);
	BOOST_CHECK(a.m_value == y);

	hive.delete_event(evtb);
	y=2;
	hive.activate(y);
	BOOST_CHECK(b.m_value != y);
	BOOST_CHECK(a.m_value == y);
}

BOOST_AUTO_TEST_SUITE_END()

}
