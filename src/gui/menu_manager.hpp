#ifndef HEADER_ASAED_GUI_MENU_MANAGER_HPP
#define HEADER_ASAED_GUI_MENU_MANAGER_HPP

#include "util/currenton.hpp"

#include <memory>
#include <vector>

#include "control/controller.hpp"
#include "gui/menu_action.hpp"
#include "gui/menu_transition.hpp"
#include "math/rectf.hpp"

class Menu;
class DrawingContext;
union SDL_Event;

class MenuManager final : public Currenton<MenuManager> {
public:
	static const float m_menu_repeat_initial;
	static const float m_menu_repeat_rate;
	
private:
	std::vector<std::unique_ptr<Menu>> m_menu_stack;
	std::unique_ptr<MenuTransition> m_transition;

	float m_menu_repeat_time;

public:
	MenuManager();
	~MenuManager() override;

private:
	MenuManager(const MenuManager&) = delete;
	MenuManager& operator=(const MenuManager&) = delete;

public:
	void process_event(const SDL_Event& event);
	void process_input(const Controller& controller);
	void draw(DrawingContext& drawing_context);

	void set_menu(int id, bool skip_transition = false);
	void set_menu(std::unique_ptr<Menu> menu, bool skip_transition = false);
	void push_menu(int id, bool skip_transition = false);
	void push_menu(std::unique_ptr<Menu> menu, bool skip_transition = false);
	void pop_menu(bool skip_transition = false);
	void clear_menu_stack(bool skip_transition = false);

	bool is_active();
	
	Menu* current_menu() const;
	Menu* previous_menu() const;

private:
	void check_input_action(Control control, MenuAction action, const Controller& controller, MenuAction& result);

	template<typename T>
	Rectf to_rect(T& menu) {
		return Rectf(menu.get_center_position().x - menu.get_width() / 2.0f,
		             menu.get_center_position().y - menu.get_width() / 2.0f,
		             menu.get_center_position().x + menu.get_width() / 2.0f,
		             menu.get_center_position().y + menu.get_width() / 2.0f);
	}

	template<typename S, typename T>
	Rectf to_transition_rect(S* from, T* to) {
		if (from) {
			return to_rect(*from);
		}
		else {
			return Rectf(to->get_center_position(), Sizef(0.0f, 0.0f));
		}
	}

	template<typename S, typename T>
	void transition(S* from, T* to) {
		if (!from && !to) {
			return;
		}
		m_transition->start(to_transition_rect(from, to), to_transition_rect(to, from));
	}
};


#endif