#pragma once

#include <d3d.h>
#include <set>
#include <string>

typedef void* any;
extern class gxRuntime;
extern class gxCanvas;

class gxGraphics {
public:
	IDirectDraw7* dirDraw;
	IDirectDraw* ds_dirDraw;

	IDirect3D7* dir3d;
	IDirect3DDevice7* dir3dDev;
	D3DDEVICEDESC7 dir3dDevDesc;
	DDPIXELFORMAT primFmt, zbuffFmt;

	DDPIXELFORMAT texRGBFmt[2], texAlphaFmt[2], texRGBAlphaFmt[2], texRGBMaskFmt[2];

	any ftLibrary;

	bool running_on_wine;

	gxRuntime* runtime;

private:
	gxCanvas* front_canvas, * back_canvas;
	any def_font;
	bool gfx_lost;
	any dummy_mesh;

	std::set<any> font_set;
	std::set<gxCanvas*> canvas_set;
	std::set<any> mesh_set;
	std::set<any> scene_set;
	std::set<any> movie_set;
	std::set<std::string> font_res;

	DDGAMMARAMP _gammaRamp;
	IDirectDrawGammaControl* _gamma;
};
