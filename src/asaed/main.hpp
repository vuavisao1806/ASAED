#ifndef HEADER_ASAED_ASAED_MAIN_HPP
#define HEADER_ASAED_ASAED_MAIN_HPP

#include <memory>

#include "control/input_manager.hpp"
#include "video/video_system.hpp"

class SDLSubsystem final {
public:
	SDLSubsystem();
	~SDLSubsystem();
};
class Main final {
public :
	Main();
	~Main();

private:
	Main(const Main&) = delete;
	Main& operator=(const Main&) = delete;

private:
	std::unique_ptr<SDLSubsystem> m_sdl_subsystem;
	std::unique_ptr<InputManager> m_input_manager;
	std::unique_ptr<VideoSystem> m_video_system;

public :
	int run(int argc, char** argv);
};

#endif