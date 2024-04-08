#ifndef HEADER_ASAED_ASAED_GLOBALS_HPP
#define HEADER_ASAED_ASAED_GLOBALS_HPP

#include <memory>

#include <video/video_system.hpp>
#include <video/viewport.hpp>

// Managing global (??). Hope it's working the right way

// little bit of trolling! DON'T TOUCH UP
#define SCREEN_WIDTH (VideoSystem::current()->get_viewport().get_screen_width())
// little bit of trolling! DON'T TOUCH UP
#define SCREEN_HEIGHT (VideoSystem::current()->get_viewport().get_screen_height())

class Config;

extern std::unique_ptr<Config> g_config;

extern float g_game_time;
extern float g_real_time;

#endif