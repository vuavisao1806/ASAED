#include "gui/menu.hpp"

#include "asaed/resources.hpp"
#include "asaed/gameconfig.hpp"
#include "asaed/globals.hpp"
#include "control/input_manager.hpp"
#include "gui/item_action.hpp"
#include "gui/item_back.hpp"
#include "gui/item_goto.hpp"
#include "gui/item_horizontal_line.hpp"
#include "gui/item_label.hpp"
#include "gui/item_string_select.hpp"
#include "gui/menu_item.hpp"
#include "gui/menu_manager.hpp"
#include "gui/mouse_cursor.hpp"
#include "video/drawing_context.hpp"
#include "video/viewport.hpp"

Menu::Menu() :
	m_position(static_cast<float>(SCREEN_WIDTH) / 2.0f, static_cast<float>(SCREEN_HEIGHT) / 2.0f),
	m_menu_width(),
	m_menu_height(),
	m_items(),
	m_active_item(-1)
{}

Menu::~Menu()
{}

bool Menu::on_back_action() { return true; }

void Menu::draw(DrawingContext& drawing_context) {
	const float menu_height = get_height();
	float position_y = m_position.y - menu_height / 2.0f;

	for (size_t i = 0; i < m_items.size(); ++ i) {
		position_y += m_items[i]->get_distance();
		draw_item(drawing_context, i, position_y + static_cast<float>(m_items[i]->get_height() / 2.0f));
		position_y += static_cast<float>(m_items[i]->get_height()) + m_items[i]->get_distance();
	}
}

void Menu::draw_item(DrawingContext& drawing_context, int index, float position_y) {
	const float menu_width = get_width();

	MenuItem* pointer_item = m_items[index].get();

	const float position_x = m_position.x - menu_width / 2.0f;

	pointer_item->draw(drawing_context, Vector(position_x, position_y), static_cast<int>(menu_width), m_active_item == index);

	if (m_active_item == index) {
		float blink = 0.75f; // options
		drawing_context.get_canvas().draw_filled_rect(Rectf(Vector(m_position.x - menu_width / 2.0f + (10.0f - 2.0f), position_y - static_cast<float>(pointer_item->get_height()) / 2.0f - 2.0f),
		                                                    Vector(m_position.x + menu_width / 2.0f - (10.0f - 2.0f), position_y + static_cast<float>(pointer_item->get_height()) / 2.0f + 2.0f)),
		                                              Color(1.0f, 1.0f, 1.0f, blink), LAYER_GUI - 10);
		drawing_context.get_canvas().draw_filled_rect(Rectf(Vector(m_position.x - menu_width / 2.0f + 10.0f, position_y - static_cast<float>(pointer_item->get_height()) / 2.0f),
		                                                    Vector(m_position.x + menu_width / 2.0f - 10.0f, position_y + static_cast<float>(pointer_item->get_height()) / 2.0f)),
		                                              Color(1.0f, 1.0f, 1.0f, 0.5f), LAYER_GUI - 10);
	}
}

void Menu::process_event(const SDL_Event& event) {
	m_items[m_active_item]->process_event(event);
	switch (event.type) {
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT) {
				Vector mouse_position = VideoSystem::current()->get_viewport().to_logical(event.motion.x, event.motion.y);
				if (mouse_position.x > m_position.x - get_width() / 2.0f &&
				    mouse_position.x < m_position.x + get_width() / 2.0f &&
				    mouse_position.y > m_position.y - get_height() / 2.0f &&
				    mouse_position.y < m_position.y + get_height() / 2.0f) {
					process_action(MenuAction::HIT);
				}
			}
			break;
		
		case SDL_MOUSEMOTION: {
			Vector mouse_position = VideoSystem::current()->get_viewport().to_logical(event.motion.x, event.motion.y);
			float x = mouse_position.x;
			float y = mouse_position.y;

			if (x > m_position.x - get_width() / 2.0f &&
			    x < m_position.x + get_width() / 2.0f &&
			    y > m_position.y - get_height() / 2.0f &&
			    y < m_position.y + get_height() / 2.0f) {
				int new_active_item = 0;
				float item_y = m_position.y - get_height() / 2.0f;
				for (size_t i = 0; i < m_items.size(); ++ i) {
					if (y >= item_y && y <= item_y + static_cast<float>(m_items[i]->get_height())) {
						new_active_item = i;
						break;
					}
					item_y += static_cast<float>(m_items[i]->get_height());
				}

				if (!m_items[new_active_item]->skippable() && new_active_item != m_active_item) {
					if (m_active_item != -1) {
						process_action(MenuAction::UNSELECT);
					}
					m_active_item = new_active_item;
					process_action(MenuAction::SELECT);
				}

				if (MouseCursor::current()) {
					MouseCursor::current()->set_state(MouseCursorState::NORMAL);
				}
			}
			else {
				if (MouseCursor::current()) {
					MouseCursor::current()->set_state(MouseCursorState::NORMAL);
				}
			}
		}
		default:
			break;
	}
}

void Menu::process_action(const MenuAction& action) {
	if (m_items.empty()) {
		return;
	}

	const int last_active_item = m_active_item;
	const int cnt_items = static_cast<int>(m_items.size());

	switch (action) {
		case MenuAction::UP:
			do {
				m_active_item = (m_active_item - 1 + cnt_items) % cnt_items;
			} while (m_items[m_active_item]->skippable() && (m_active_item != last_active_item));
			break;
		
		case MenuAction::DOWN:
			do {
				m_active_item = (m_active_item + 1) % cnt_items;
			} while (m_items[m_active_item]->skippable() && (m_active_item != last_active_item));
			break;
		
		case MenuAction::BACK:
			if (on_back_action()) {
				MenuManager::current()->pop_menu();
			}
			return;
		default:
			break;
	}

	if (last_active_item != m_active_item) {
		if (last_active_item != -1) {
			m_items[last_active_item]->process_action(MenuAction::UNSELECT);
		}
		m_items[m_active_item]->process_action(MenuAction::SELECT);
	}
	m_items[m_active_item]->process_action(action);

	if (MenuManager::current()->current_menu() != this) {
		return;
	}
	if (action == MenuAction::HIT) {
		menu_action(*m_items[m_active_item]);
	}
}

ItemHorizontalLine& Menu::add_horizontal_line() {
	auto item = std::make_unique<ItemHorizontalLine>();
	auto item_ptr = item.get();
	add_item(std::move(item));
	return *item_ptr;
}

ItemLabel& Menu::add_label(const std::string& text) {
	auto item = std::make_unique<ItemLabel>(text);
	auto item_ptr = item.get();
	add_item(std::move(item));
	return *item_ptr;
}

ItemAction& Menu::add_entry(int id, const std::string& text) {
	auto item = std::make_unique<ItemAction>(text, id);
	auto item_ptr = item.get();
	add_item(std::move(item));
	return *item_ptr;
}

ItemAction& Menu::add_entry(const std::string& text, const std::function<void()>& callback) {
	auto item = std::make_unique<ItemAction>(text, -1, callback);
	auto item_ptr = item.get();
	add_item(std::move(item));
	return *item_ptr;
}

ItemBack& Menu::add_back(const std::string& text, int id) {
	auto item = std::make_unique<ItemBack>(text, id);
	auto item_ptr = item.get();
	add_item(std::move(item));
	return *item_ptr;
}

ItemGoto& Menu::add_submenu(const std::string& text, int submenu, int id) {
	auto item = std::make_unique<ItemGoto>(text, submenu, id);
	auto item_ptr = item.get();
	add_item(std::move(item));
	return *item_ptr;
}

ItemStringSelect& Menu::add_string_select(int id, const std::string& text, int* selected, const std::vector<std::string>& items) {
	auto item = std::make_unique<ItemStringSelect>(text, items, selected, id);
	auto item_ptr = item.get();
	add_item(std::move(item));
	return *item_ptr;
}

MenuItem& Menu::add_item(std::unique_ptr<MenuItem> menu_item) {
	m_items.push_back(std::move(menu_item));
	MenuItem& item = *m_items.back();

	if (m_active_item == -1 && !item.skippable()) {
		m_active_item = static_cast<int>(m_items.size()) - 1;
	}

	re_calculate_width();
	re_calculate_height();

	return item;
}

MenuItem& Menu::add_item(std::unique_ptr<MenuItem> menu_item, int position) {
	m_items.insert(m_items.begin() + position, std::move(menu_item));
	MenuItem& item = *m_items[position];

	if (m_active_item >= position) {
		++ m_active_item;
	}

	re_calculate_width();
	re_calculate_height();

	return item;
}

void Menu::delete_item(int position) {
	m_menu_height -= static_cast<float>(m_items[position]->get_height()) + m_items[position]->get_distance() * 2.0f;
	m_items.erase(m_items.begin() + position);
}

void Menu::clear() {
	m_items.clear();
	m_active_item = -1;
}

MenuItem& Menu::get_item(int index) { return *m_items[index]; }

MenuItem& Menu::get_item_by_id(int id) {
	auto item = std::find_if(m_items.begin(), m_items.end(), [id] (const std::unique_ptr<MenuItem>& cur) {
		return cur->get_id() == id;
	});

	if (item != m_items.end()) {
		return *item->get();
	}
	throw std::runtime_error("MenuItem not found: " + std::to_string(id));
}

const MenuItem& Menu::get_item_by_id(int id) const {
	auto item = std::find_if(m_items.begin(), m_items.end(), [id] (const std::unique_ptr<MenuItem>& cur) {
		return cur->get_id() == id;
	});

	if (item != m_items.end()) {
		return *item->get();
	}
	throw std::runtime_error("MenuItem not found: " + std::to_string(id));
}

int Menu::get_active_item_id() const {
	return m_items[m_active_item]->get_id();
}

void Menu::set_active_item(int id) {
	for (size_t i = 0; i < m_items.size(); ++ i) {
		if (m_items[i]->get_id() == id) {
			m_active_item = static_cast<int>(i);
			break;
		}
	}
}

Vector Menu::get_center_position() const { return m_position; }
void Menu::set_center_position(float x, float y) {
	m_position.x = x;
	m_position.y = y;
}

float Menu::get_width() const { return m_menu_width + 24.0f; }
float Menu::get_height() const { return m_menu_height; }

void Menu::re_calculate_width() {
	float max_width = 0.0f;
	for (const auto& item : m_items) {
		max_width = std::max(max_width, static_cast<float>(item->get_width()));
	}
	m_menu_width = max_width;
}

void Menu::re_calculate_height() {
	float height = 0.0f;
	for (const auto& item : m_items) {
		height += static_cast<float>(item->get_height()) + item->get_distance() * 2.0f;
	}
	m_menu_height = height;
}