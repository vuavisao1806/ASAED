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
		static Color back_color;
		static Color front_color;
		static Color horizontal_line_color;
		static Color default_color;
		static Color active_color;
		static Color inactive_color;
		static Color label_color;
	};

	class Text {
	public:
		static Color small_color;
		static Color heading_color;
		static Color reference_color;
		static Color normal_color;
	};
};

#endif
