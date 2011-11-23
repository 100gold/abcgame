#include "import.h"
#include "all.h"

namespace Localization 
{

std::string fetch_from_xml(const pugi::xml_node& root, const std::string& xpath)
{
	auto res = root.select_nodes(xpath.c_str());
	if (res.begin() == res.end())
	{
		throw ELocalization(root, xpath);
	}
	return res.begin()->node().child_value();
}

}
