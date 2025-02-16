#pragma once

#include <pango/pangocairo.h>
#include <tuple>

class gxPangoFont {
public:
	PangoFontDescription* desc;

    gxPangoFont(const std::string& d) {
        desc = pango_font_description_from_string(d.c_str());
    }

    ~gxPangoFont() {
        pango_font_description_free(desc);
    }

	std::pair<int, int> stringWidthHeight(const std::string& text) const {
		cairo_surface_t* surface = cairo_image_surface_create(CAIRO_FORMAT_A1, 0, 0);
		cairo_t* cr = cairo_create(surface);
		PangoLayout* layout = pango_cairo_create_layout(cr);
		pango_layout_set_text(layout, text.c_str(), text.length());
		pango_layout_set_font_description(layout, this->desc);

		int width, height;
		pango_layout_get_size(layout, &width, &height);
		width = PANGO_PIXELS(width);
		height = PANGO_PIXELS(height);

		g_object_unref(layout);
		cairo_destroy(cr);
		cairo_surface_destroy(surface);

		return std::make_pair(width, height);
	}
};