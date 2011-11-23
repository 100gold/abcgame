#pragma once

namespace Localization
{

std::string fetch_from_xml(const pugi::xml_node& root, const std::string& xpath);

};