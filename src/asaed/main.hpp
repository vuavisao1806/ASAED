#ifndef HEADER_ASAED_ASAED_MAIN_HPP
#define HEADER_ASAED_ASAED_MAIN_HPP

#include <memory>

#include "control/input_manager.hpp"
#include "video/video_system.hpp"

/** 
 * Class to manage g_config to avoid global (!!!) 
 * It has only one unique function
*/

class ConfigSubsystem {
public:
	ConfigSubsystem();
	~ConfigSubsystem();
};

/** Class to init and quit SDL automatic */
class SDLSubsystem final {
public:
	SDLSubsystem();
	~SDLSubsystem();
};

/** Class to hold everything to run game */
class Main final {
public :
	Main();
	~Main();

private:
	Main(const Main&) = delete;
	Main& operator=(const Main&) = delete;

private:
	std::unique_ptr<ConfigSubsystem> m_config_subsystem;
	std::unique_ptr<SDLSubsystem> m_sdl_subsystem;
	std::unique_ptr<InputManager> m_input_manager;
	std::unique_ptr<VideoSystem> m_video_system;

public :
	int run(int argc, char** argv);
};

#endif