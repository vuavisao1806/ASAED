#include "util/log.hpp"

LogLevel g_log_level = LOG_DEBUG;

std::ostream& get_logging_instance() {
	return (std::cerr);
}

static std::ostream& log_generic_f(const char* prefix, const char* file, int line) {
	get_logging_instance() << prefix << ' ' << file << ":" << line << " ";
	return (get_logging_instance());
}

std::ostream& log_debug_f(const char* file, int line) {
	return log_generic_f("[DEBUG]", file, line);
}

std::ostream& log_info_f(const char* file, int line) {
	return log_generic_f("[INFO]", file, line);
}

std::ostream& log_warning_f(const char* file, int line) {
	return log_generic_f("[WARNING]", file, line);
}

std::ostream& log_fatal_f(const char* file, int line) {
	return log_generic_f("[FATAL]", file, line);
}