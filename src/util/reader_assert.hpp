#ifndef HEADER_ASAED_UTIL_READER_ASSERT_HPP
#define HEADER_ASAED_UTIL_READER_ASSERT_HPP

#include <fstream>

#include "util/json.hpp"

using json = nlohmann::json;

void assert_isn_array(const json& data);
void assert_isn_object(const json& data);
void assert_is_array(const json& data);

#endif