#pragma once

// Magic!
// Do not modify them except it is not compatible with the latest Blitz3D TSS.

#include "gxGraphics.h"

typedef IDirectDrawSurface7 ddSurf;

class PixelFormat {
public:
    int depth, pitch;
    unsigned amask, rmask, gmask, bmask, argbfill;
    unsigned char ashr, ashl, rshr, rshl, gshr, gshl, bshr, bshl;
    typedef void(_fastcall* Plot)(void* pix, unsigned argb);
    typedef unsigned(_fastcall* Point)(void* pix);
    Plot plot;
    Point point;
    char* plot_code, * point_code;

    unsigned __stdcall fromARGB(unsigned n)const {
        return ((n >> ashr << ashl) & amask) | ((n >> rshr << rshl) & rmask) | ((n >> gshr << gshl) & gmask) | ((n >> bshr << bshl) & bmask);
    }

    unsigned __stdcall toARGB(unsigned n) const {
        return ((n & amask) >> ashl << ashr) | ((n & rmask) >> rshl << rshr) | ((n & gmask) >> gshl << gshr) | ((n & bmask) >> bshl << bshr) | argbfill;
    }
};

struct Rect : public RECT {
    Rect() {
    }
    Rect(int x, int y, int w, int h) {
        left = x; top = y; right = x + w; bottom = y + h;
    }
};

static bool __stdcall clip(const RECT& viewport, RECT* d, RECT* s) {
    if (d->right <= d->left ||
        d->bottom <= d->top ||
        d->left >= viewport.right ||
        d->right <= viewport.left ||
        d->top >= viewport.bottom ||
        d->bottom <= viewport.top) return false;
    int dx, dy;
    if ((dx = viewport.left - d->left) > 0) { d->left += dx; s->left += dx; }
    if ((dx = viewport.right - d->right) < 0) { d->right += dx; s->right += dx; }
    if ((dy = viewport.top - d->top) > 0) { d->top += dy; s->top += dy; }
    if ((dy = viewport.bottom - d->bottom) < 0) { d->bottom += dy; s->bottom += dy; }
    return true;
}

class gxCanvas {
public:
    int flags, cube_mode;
    gxGraphics* graphics;

    ddSurf* main_surf, * surf, * z_surf, * cube_surfs[6];

    mutable int mod_cnt;
    mutable ddSurf* t_surf;

    mutable int locked_pitch, locked_cnt, lock_mod_cnt, remip_cnt;
    mutable unsigned char* locked_surf;

    mutable int cm_pitch;
    mutable unsigned* cm_mask;

    RECT clip_rect;

    PixelFormat format;

    any font;
    RECT viewport;
    int origin_x, origin_y, handle_x, handle_y;
    unsigned mask_surf, color_surf, color_argb, clsColor_surf;

    bool __stdcall lock() const {
        if (!locked_cnt++) {
            DDSURFACEDESC2 desc = { sizeof(desc) };
            if (surf->Lock(0, &desc, DDLOCK_WAIT | DDLOCK_NOSYSLOCK, 0) < 0) {
                --locked_cnt;
                return false;
            }
            locked_pitch = desc.lPitch;
            locked_surf = (unsigned char*)desc.lpSurface;
            lock_mod_cnt = mod_cnt;
        }
        return true;
    }

    void __stdcall unlock() const {
        if (locked_cnt == 1) {
            if (lock_mod_cnt != mod_cnt && cm_mask) updateBitMask(clip_rect);
            surf->Unlock(0);
        }
        --locked_cnt;
    }

    void __stdcall updateBitMask(const RECT& r) const {
        int w = r.right - r.left; if (w <= 0) return;
        int h = r.bottom - r.top; if (h <= 0) return;

        lock();
        RECT t = r;
        t.left &= ~31;
        t.right = (t.right + 31) & ~31;
        w = (t.right - t.left) / 32;
        unsigned char* src_row = locked_surf + t.top * locked_pitch + t.left * format.pitch;
        unsigned* dest_row = cm_mask + t.top * cm_pitch + t.left / 32;
        unsigned mask_argb = format.toARGB(mask_surf) & 0xffffff;

        while (h--) {
            unsigned* dest = dest_row;
            unsigned char* src = src_row;
            for (int c = 0; c < w; ++c) {
                unsigned mask = 0;
                for (int x = 0; x < 32; ++x) {
                    unsigned pix = format.point(src) & 0xffffff;
                    mask = (mask << 1) | (pix != mask_argb);
                    src += format.pitch;
                }
                *dest++ = mask;
            }
            dest_row += cm_pitch;
            src_row += locked_pitch;
        }
        unlock();
    }

    void __stdcall damage(const RECT& r) const {
        ++mod_cnt; if (cm_mask) updateBitMask(r);
    }

    void __stdcall blit(int x, int y, ddSurf* src_surf, int mask, int src_x, int src_y, int src_w, int src_h) {
        x += origin_x;
        y += origin_y;
        Rect dest_r(x, y, src_w, src_h), src_r(src_x, src_y, src_w, src_h);
        if (!::clip(viewport, &dest_r, &src_r)) return;
        RECT clip_rect = { 0, 0, src_w, src_h };
        if (!::clip(clip_rect, &src_r, &dest_r)) return;

        static DDBLTFX bltfx = { sizeof(DDBLTFX) };
        bltfx.ddckSrcColorkey.dwColorSpaceLowValue = bltfx.ddckSrcColorkey.dwColorSpaceHighValue = mask;
        surf->Blt(&dest_r, src_surf, &src_r, DDBLT_WAIT | DDBLT_KEYSRCOVERRIDE, &bltfx);
        damage(dest_r);
    }
};