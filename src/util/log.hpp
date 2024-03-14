#ifndef HEADER_ASAED_UNIT_LOG_HPP
#define HEADER_ASAED_UNIT_LOG_HPP

#include <iostream>

enum LogLevel {
	LOG_NONE,
	LOG_FATAL,
	LOG_WARNING,
	LOG_INFO,
	LOG_DEBUG,
};

extern LogLevel g_log_level;

std::ostream& get_logging_instance();

/**
 * usage: log_debug << "debug";
*/
std::ostream& log_debug_f(const char* file, int line);
#define log_debug if (g_log_level >= LOG_DEBUG) log_debug_f(__FILE__, __LINE__)

/**
 * usage: log_info << "info";
*/
std::ostream& log_info_f(const char* file, int line);
#define log_info if (g_log_level >= LOG_DEBUG) log_info_f(__FILE__, __LINE__)

/**
 * usage: log_warning << "warning";
*/
std::ostream& log_warning_f(const char* file, int line);
#define log_warning if (g_log_level >= LOG_DEBUG) log_warning_f(__FILE__, __LINE__)

/**
 * usage: log_fatal << "fatal";
*/
std::ostream& log_fatal_f(const char* file, int line);
#define log_fatal if (g_log_level >= LOG_DEBUG) log_fatal_f(__FILE__, __LINE__)

#endif