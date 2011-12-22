#pragma once

class ERootException : public std::exception
{
protected:
	std::string m_what;

public:
	ERootException()
	{
		BOOST_ASSERT(false);
	}

	const char* what() const
	{
		return m_what.c_str();
	}
};

class ELocalization : public ERootException
{
public:
	ELocalization(const pugi::xml_node& node, const std::string& query)
	{
		std::stringstream stream;
		stream << "Localization error[node='" << node.name() << "',query='" << query << "']";
		m_what = stream.str();
	}
};

class EOgreBase : public ERootException
{
};

class ESearchItem : public ERootException
{
public:
	ESearchItem(const char* item, const char* list_name)
	{
		std::stringstream stream;
		stream << "Search in " << list_name << " failed on item " << item;
		m_what = stream.str();
	}
};

class EInconsistent : public ERootException
{
public:
	EInconsistent(const char* msg)
	{
		m_what = msg;
	}
};

class ENoViewableNode : public ERootException
{
public:
	ENoViewableNode(const char* msg)
	{
		m_what = msg;
	}
};