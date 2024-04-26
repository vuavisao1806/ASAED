#include "object/scroller_text.hpp"

#include "asaed/screen_manager.hpp"
#include "control/input_manager.hpp"
#include "video/drawing_context.hpp"
#include "video/ttf_font.hpp"
#include "util/reader_machine.hpp"
#include "util/reader_data.hpp"

namespace {
	const float SPEED = 60.0f; // options
	const float SCROLL_JUMP = 60.0f; // options
	const float ITEMS_SPACE = 0.0f; // options
} // namespce

ScrollerText::ScrollerText(const std::string& filename) :
	m_filename(filename),
	m_controller(&(InputManager::current()->get_controller())),
	m_lines(),
	m_speed(SPEED),
	m_scroll(),
	m_offset(),
	m_finished()
{
	ReaderMachine reader(m_filename);
	for (size_t i = 0; i < reader.get_size(); ++ i) {
		parse(reader.get_data(i));
	}
}

void ScrollerText::update(float dt_sec) {
	float speed = m_speed;

	if (m_controller) {
		if (m_controller->hold(Control::UP) && !m_controller->hold(Control::DOWN)) {
			speed = -m_speed * 5;
		}
		else if (m_controller->hold(Control::DOWN) && !m_controller->hold(Control::UP)) {
			speed = m_speed * 5;
		}
	
		if (m_controller->pressed_any(Control::ACTION, Control::MENU_SELECT) && !m_controller->pressed(Control::UP)) {
			scroll(SCROLL_JUMP);
		}

		if (m_controller->pressed_any(Control::ESCAPE)) {
			ScreenManager::current()->pop_screen();
		}
	}

	m_scroll += speed * dt_sec;

	if (m_scroll < 0.0f) {
		m_scroll = 0.0f;
	}

	if (m_finished) {
		ScreenManager::current()->pop_screen();
	}
}

void ScrollerText::draw(DrawingContext& drawing_context) {
	drawing_context.push_transform();
	drawing_context.set_translation(Vector(0.0f, 0.0f));

	const float context_width = drawing_context.get_width();
	const float context_height = drawing_context.get_height();

	float y = std::floor(context_height - m_scroll);

	{
		for (const auto& [type, line] : m_lines) {
			auto font = from_text_type_to_font(type);
			auto color = from_text_type_to_color(type);
			float height = font->get_height() + ITEMS_SPACE;
			if (y + height >= 0 && context_height - y >= 0) {
				font->draw_text(drawing_context.get_canvas(), line, Vector(context_width / 2.0f, y), ALIGN_CENTER, LAYER_GUI, color);
			}
			y += std::floor(height);
		}
	}
	drawing_context.pop_transform();

	if (y < 0) {
		m_finished = true;
		set_speed(0.0f);
	}
}

std::string ScrollerText::class_name() { return "scroller-text"; }
std::string ScrollerText::get_class_name() const { return class_name(); }

void ScrollerText::set_speed(float speed) { m_speed = speed; }
void ScrollerText::scroll(float offset) {
	m_scroll += offset;
	if (m_scroll < 0.0f) {
		m_scroll = 0.0f;
	}
}
bool ScrollerText::is_finished() const { return m_finished; }

void ScrollerText::parse(const ReaderData* data) {
	std::string s_type;
	if (!data->get("type", s_type)) {
		throw std::runtime_error("Unknown type of text");
	}

	TextType type = from_string_to_text_type(s_type);
	
	if (type == TextType::TITLE) {
		std::vector<std::string> name;
		if (!data->get("name", name)) {
			throw std::runtime_error("Missing name of title");
		}
		assert(name.size() == 1);
		m_lines.push_back(make_pair(type, name[0]));
	}
	else if (type == TextType::HEADING) {
		std::vector<std::string> name;
		if (!data->get("name", name)) {
			throw std::runtime_error("Missing name of heading");
		}
		assert(name.size() == 1);
		m_lines.push_back(make_pair(type, name[0]));
	}
	else if (type == TextType::BLANK) {
		std::string blank = "";
		m_lines.push_back(make_pair(type, blank));
	}
	else if (type == TextType::PERSONS) {
		std::vector<std::string> name;
		if (!data->get("name", name)) {
			throw std::runtime_error("Missing names of persons");
		}
		for (const auto& person : name) {
			m_lines.push_back(make_pair(type, person));
		}
	}
	else {
		throw std::runtime_error("Unknown type of text!!");
	}
}

TextType from_string_to_text_type(const std::string& name) {
	if (name == "title") return TextType::TITLE;
	if (name == "heading") return TextType::HEADING;
	if (name == "blank") return TextType::BLANK;
	if (name == "persons") return TextType::PERSONS;
	throw std::runtime_error("Unknown type of text!!");
	return TextType::BLANK;
}

TTFFontPtr from_text_type_to_font(const TextType& text_type) {
	switch (text_type) {
		case TextType::TITLE:
			return Resources::big_font;
			break;
		
		case TextType::HEADING:
			return Resources::normal_font;
			break;
		
		case TextType::BLANK:
			return Resources::normal_font;
			break;
		
		case TextType::PERSONS:
			return Resources::small_font;
			break;
		
		default:
			throw std::runtime_error("Unknown type of text!!");
			break;
	}
	return Resources::normal_font;
}

Color from_text_type_to_color(const TextType& text_type) {
	switch (text_type) {
		case TextType::TITLE:
			return ColorScheme::Text::reference_color;
			break;
		
		case TextType::HEADING:
			return ColorScheme::Text::heading_color;
			break;
		
		case TextType::BLANK:
			return ColorScheme::Text::small_color;
			break;
		
		case TextType::PERSONS:
			return ColorScheme::Text::normal_color;
			break;
		
		default:
			throw std::runtime_error("Unknown type of text!!");
			break;
	}
	return ColorScheme::Text::normal_color;
}