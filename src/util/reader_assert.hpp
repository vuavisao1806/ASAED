#ifndef HEADER_ASAED_UTIL_READER_ASSERT_HPP
#define HEADER_ASAED_UTIL_READER_ASSERT_HPP

#include <fstream>
#include <string>
#include <tuple>

#include "util/json.hpp"

using json = nlohmann::json;

/** check error when parsing item in reader_data */

void assert_isn_array(const std::string parent_path, const std::pair<std::string, json>* data);
void assert_isn_object(const std::string parent_path, const std::pair<std::string, json>* data);
void assert_is_array(const std::string parent_path, const std::pair<std::string, json>* data);

#endif