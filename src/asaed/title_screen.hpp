#ifndef HEADER_ASAED_ASAED_TITLE_MANAGER_HPP
#define HEADER_ASAED_ASAED_TITLE_MANAGER_HPP

#include "asaed/screen.hpp"
#include "util/currenton.hpp"
#include "video/surface_ptr.hpp"

class Compositor;
class Controller;

class TitleScreen : public Screen, public Currenton<TitleScreen> {
private:
	SurfacePtr m_theme;

public:
	TitleScreen();
	~TitleScreen() override;

public:
	virtual void draw(Compositor& compositor) override;
	virtual void update(float dt_sec, const Controller& controller) override;

	virtual void setup() override;
	virtual void leave() override;
};

#endif