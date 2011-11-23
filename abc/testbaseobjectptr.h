#pragma once

template<class Object>
class TestObjectPtr
{
	Object* m_ptr;
public:
	TestObjectPtr(Object* ptr) :
		m_ptr(ptr)
	{
	}
	operator Object*()
	{
		return m_ptr;
	}
	Object* operator->()
	{
		return m_ptr;
	}
	Object* get()
	{
		return m_ptr;
	}
	~TestObjectPtr()
	{
		m_ptr->set_dead();
		delete m_ptr;
	}
};