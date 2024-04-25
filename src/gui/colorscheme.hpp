#ifndef HEADER_ASAED_GUI_COLORSCHEME_HPP
#define HEADER_ASAED_GUI_COLORSCHEME_HPP

#include "video/color.hpp"

/** Class to holds color scheme most use, everything in here is options */

/**
 * I used color schemes like Supertux
 * https://github.com/SuperTux/supertux/blob/master/src/supertux/colorscheme.hpp for more details
*/

class ColorScheme final {
public:
	class Menu {
	public:
		static Color back_color; // blue
		static Color front_color; // gray
		static Color horizontal_line_color;
		static Color default_color; // white
		static Color active_color; // blue
		static Color inactive_color; // gray
		static Color label_color; // light blue
	};

	class HUD {
	public:
		static Color frame_back;
		static Color frame_front;
		static Color space_back;
		static Color heart_back;
		static Color heart_front;
		static Color shield_back;
		static Color shield_front;
		static Color normal_color;
	};

	class FloatingText {
	public:
		static Color text_color;
	};

	class Text {
	public:
		static Color small_color; // white
		static Color heading_color; // yellow
		static Color reference_color; // blue
		static Color normal_color; // white
	};
};

#endif
