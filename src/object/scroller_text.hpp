#ifndef HEADER_ASAED_OBJECT_SCROLLER_TEXT_HPP
#define HEADER_ASAED_OBJECT_SCROLLER_TEXT_HPP


#include <string>
#include <vector>
#include <tuple>

#include "asaed/resources.hpp"
#include "control/controller.hpp"
#include "object/game_object.hpp"
#include "gui/colorscheme.hpp"
#include "video/color.hpp"
#include "video/ttf_font_ptr.hpp"

class DrawingContext;
class ReaderData;

enum TextType {
	TITLE,
	HEADING,
	BLANK,
	PERSONS
};

class ScrollerText : public GameObject {
private:
	std::string m_filename;
	const Controller* m_controller;

	std::vector<std::pair<TextType, std::string>> m_lines;
	float m_speed;
	float m_scroll;
	float m_offset;
	bool m_finished;

public:
	ScrollerText(const std::string& filename);

private:
	ScrollerText(const ScrollerText&) = delete;
	ScrollerText& operator=(const ScrollerText&) = delete;

public:
	virtual void update(float dt_sec) override;
	virtual void draw(DrawingContext& drawing_context) override;

	static std::string class_name();
	virtual std::string get_class_name() const override;

	void set_speed(float speed);
	void scroll(float offset);
	bool is_finished() const;

private:
	void parse(const ReaderData* data);
};

TextType from_string_to_text_type(const std::string& name);
TTFFontPtr from_text_type_to_font(const TextType& text_type);
Color from_text_type_to_color(const TextType& text_type);

#endif