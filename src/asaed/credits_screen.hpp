#ifndef HEADER_ASAED_ASAED_CREDITS_SCREEN_HPP
#define HEADER_ASAED_ASAED_CREDITS_SCREEN_HPP

#include "asaed/screen.hpp"

#include <memory>

#include "util/currenton.hpp"

class ScrollerText;

class CreditsScreen : public Screen, public Currenton<CreditsScreen> {
private:
	std::unique_ptr<ScrollerText> m_text_scroller;

public:
	CreditsScreen();
	~CreditsScreen() override;

public:
	virtual void draw(Compositor& compositor) override;
	virtual void update(float dt_sec, const Controller& controller) override;

	virtual void setup() override;
	virtual void leave() override;
};
#endif