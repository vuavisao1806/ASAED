#ifndef HEADER_ASAED_ASAED_GLOBALS_HPP
#define HEADER_ASAED_ASAED_GLOBALS_HPP

#include <memory>

// Managing global (??). Hope it's working the right way

class Config;

extern std::unique_ptr<Config> g_config;

extern float g_game_time;
extern float g_real_time;

#endif