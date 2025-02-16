/*
* Alamchu - A part of BlitzToolbox
* A text layout engine library for displaying multi-language text.
*
* v1.00 2025.2.16
*/

#define BLITZ3DTSS

#include "gxCanvas.h"
#include "gxPangoFont.h"
#include "../BlitzToolbox.hpp"
#include <format>

std::set<gxPangoFont*> pango_font_set; 
constexpr int transparentPixel = 0x4A412A;

gxPangoFont* __new_pango_font(const std::string& font_conf, int size) {
    gxPangoFont* font = new gxPangoFont(std::format("{0} {1}", font_conf, size - 5).c_str());
    pango_font_set.insert(font);
    return font;
}

gxPangoFont* def_font = __new_pango_font("", 17);
gxPangoFont* curr_font = def_font;

BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_DETACH)
    {
        for (gxPangoFont* font : pango_font_set) {
            delete font;
        }
    }
    return TRUE;
}

BLITZ3D(gxPangoFont*) Pango_DefaultFont() {
    return def_font;
}

BLITZ3D(gxPangoFont*) Pango_LoadFont(BBStr font_conf, int size) {
    return __new_pango_font(font_conf, size);
}

BLITZ3D(gxPangoFont*) Pango_LoadFontEx(BBStr font_desc) {
	gxPangoFont* font = new gxPangoFont(font_desc);
	pango_font_set.insert(font);
	return font;
}

BLITZ3D(void) Pango_SetFont(gxPangoFont* font) {
	if (!pango_font_set.contains(font)) {
		BlitzToolbox::runtime_exception("Pango_SetFont", "Invalid Pango font instance!");
		return;
	}
	curr_font = font;
}

BLITZ3D(int) Pango_StringWidth(BBStr str) {
    return curr_font->stringWidthHeight(str).first;
}

BLITZ3D(int) Pango_StringHeight(BBStr str) {
    return curr_font->stringWidthHeight(str).second;
}

BLITZ3D(void) Pango_Text(gxCanvas* canvas, int x, int y, BBStr text, int align_x, int align_y) {
    auto [width, height] = curr_font->stringWidthHeight(text);
    if (width <= 0 || height <= 0) return;

	static ddSurf* surf = nullptr;
	static DDSURFACEDESC2 desc = { sizeof(DDSURFACEDESC2) };
	if (!surf || desc.dwWidth < width || desc.dwHeight < height) {
		desc.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
		desc.dwHeight = height; desc.dwWidth = width;
		desc.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
		HRESULT result = canvas->graphics->dirDraw->CreateSurface(&desc, &surf, NULL);
		if (FAILED(result)) {
			BlitzToolbox::runtime_exception("Pango_Text", "Failed to create surface!");
			return;
		}
	}

	cairo_surface_t* surface = cairo_image_surface_create(CAIRO_FORMAT_A1, width, height);
	cairo_t* cr = cairo_create(surface);
	PangoLayout* layout = pango_cairo_create_layout(cr);
	pango_layout_set_text(layout, text, -1);
	pango_layout_set_font_description(layout, curr_font->desc);
	pango_cairo_show_layout(cr, layout);
	unsigned char* data = cairo_image_surface_get_data(surface);
	int stride = cairo_image_surface_get_stride(surface);

	if (align_x == 2) x -= width;
	if (align_x == 1) x -= width / 2;
	if (align_y == 2) y -= height;
	if (align_y == 1) y -= height / 2;
	int color_argb = canvas->format.toARGB(canvas->color_surf);
	surf->Lock(0, &desc, DDLOCK_WAIT | DDLOCK_NOSYSLOCK, NULL);
	for (int y = 0; y < height; y++) {
		int line = y * stride;
		for (int x = 0; x < width; x++) {
			unsigned char pixel = (data[line + x / 8] >> (x % 8)) & 1;
			((DWORD*)desc.lpSurface)[(y * desc.lPitch / 4) + x] = pixel ? color_argb : transparentPixel;
		}
	}
	surf->Unlock(NULL);
	canvas->blit(x, y, surf, transparentPixel, 0, 0, width, height);

	g_object_unref(layout);
	cairo_destroy(cr);
	cairo_surface_destroy(surface);
}

BLITZ3D(void) Pango_FreeFont(gxPangoFont* font) {
    if (font == def_font) {
        BlitzToolbox::runtime_exception("Pango_FreeFont", "Default font cannot be free!");
        return;
    }
    if (pango_font_set.erase(font)) delete font;
}