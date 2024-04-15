#include "gui/menu_manager.hpp"

#include "asaed/globals.hpp"
#include "control/input_manager.hpp"
#include "gui/menu.hpp"
#include "gui/mouse_cursor.hpp"
#include "gui/menu_storage.hpp"
#include "util/log.hpp"
#include "video/drawing_context.hpp"

const float MenuManager::m_menu_repeat_initial = 0.5f;
const float MenuManager::m_menu_repeat_rate = 0.1f;

MenuManager::MenuManager() :
	m_menu_stack(),
	m_transition(std::make_unique<MenuTransition>()),
	m_menu_repeat_time()
{}

MenuManager::~MenuManager()
{}

void MenuManager::process_event(const SDL_Event& event) {
	if (m_transition->is_active()) {
		return;
	}
	if (current_menu()) {
		current_menu()->process_event(event);
	}
}

void MenuManager::process_input(const Controller& controller) {
	if (!current_menu()) {
		return;
	}

	MenuAction action = MenuAction::NONE;

	check_input_action(Control::UP, MenuAction::UP, controller, action);
	check_input_action(Control::DOWN, MenuAction::DOWN, controller, action);
	check_input_action(Control::LEFT, MenuAction::LEFT, controller, action);
	check_input_action(Control::RIGHT, MenuAction::RIGHT, controller, action);

	if (controller.pressed(Control::ACTION) ||
	    controller.pressed(Control::SPACE)  ||
	    controller.pressed(Control::MENU_SELECT)) {
		action = MenuAction::HIT;
	}

	if (controller.pressed(Control::ESCAPE) ||
	    controller.pressed(Control::MENU_BACK)) {
		action = MenuAction::BACK;
	}

	current_menu()->process_action(action);
}

void MenuManager::draw(DrawingContext& drawing_context) {
	if (m_transition->is_active()) {
		m_transition->update();
		m_transition->draw(drawing_context);
	}
	else {
		if (current_menu()) {
			m_transition->set(to_rect(*current_menu()));
			m_transition->draw(drawing_context);
			current_menu()->draw(drawing_context);
		}
	}

	if (is_active() && MouseCursor::current()) {
		MouseCursor::current()->draw(drawing_context);
	}
}

void MenuManager::set_menu(int id, bool skip_transition) {
	set_menu(MenuStorage::current()->create(static_cast<MenuStorage::MenuID>(id)), skip_transition);
}

void MenuManager::set_menu(std::unique_ptr<Menu> menu, bool skip_transition) {
	if (menu) {
		if (!skip_transition) {
			transition(m_menu_stack.empty() ? nullptr : m_menu_stack.back().get(), menu.get());
		}
		m_menu_stack.clear();
		m_menu_stack.push_back(std::move(menu));
	}
	else {
		if (!skip_transition) {
			transition<Menu, Menu>(m_menu_stack.empty() ? nullptr : m_menu_stack.back().get(), nullptr);
		}
		m_menu_stack.clear();
	}
	// safe :>>
	InputManager::current()->reset();
}

void MenuManager::push_menu(int id, bool skip_transition) {
	push_menu(MenuStorage::current()->create(static_cast<MenuStorage::MenuID>(id)), skip_transition);
}

void MenuManager::push_menu(std::unique_ptr<Menu> menu, bool skip_transition) {
	assert(menu);
	if (!skip_transition) {
		transition(m_menu_stack.empty() ? nullptr : m_menu_stack.back().get(), menu.get());
	}
	m_menu_stack.push_back(std::move(menu));
}

void MenuManager::pop_menu(bool skip_transition) {
	if (m_menu_stack.empty()) {
		log_warning << "What are you doing?? You shouldn't be trying to pop on an empty menu_stack" << '\n';
		return;
	}
	if (!skip_transition) {
		transition(m_menu_stack.back().get(), (m_menu_stack.size() >= 2 ? m_menu_stack[m_menu_stack.size() - 2].get() : nullptr));
	}
	m_menu_stack.pop_back();
}

void MenuManager::clear_menu_stack(bool skip_transition) {
	if (!skip_transition) {
		transition<Menu, Menu>(m_menu_stack.empty() ? nullptr : m_menu_stack.back().get(), nullptr);
	}
	m_menu_stack.clear();
}

bool MenuManager::is_active() {
	return !m_menu_stack.empty();
}

Menu* MenuManager::current_menu() const {
	if (m_menu_stack.empty()) {
		return nullptr;
	}
	return m_menu_stack.back().get();
}

Menu* MenuManager::previous_menu() const {
	if (m_menu_stack.size() < 2) {
		return nullptr;
	}
	return m_menu_stack[m_menu_stack.size() - 2].get();
}

void MenuManager::check_input_action(Control control, MenuAction action, const Controller& controller, MenuAction& result) {
	if (controller.pressed(control)) {
		result = action;
		m_menu_repeat_time = g_real_time + m_menu_repeat_initial;
	}
	if (controller.hold(control) && m_menu_repeat_time != 0.0f && g_real_time > m_menu_repeat_time) {
		result = action;
		m_menu_repeat_time = g_real_time + m_menu_repeat_rate;
	}
}