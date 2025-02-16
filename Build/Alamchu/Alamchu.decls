; Alamchu - A part of BlitzToolbox
; A text layout engine library for displaying multi-language text.
; v1.00 2025.2.16
; https://github.com/ZiYueCommentary/BlitzToolbox

.lib "Alamchu.dll"

Pango_DefaultFont%():"_Pango_DefaultFont@0"
Pango_LoadFont%(font_conf$, size%):"_Pango_LoadFont@8"
Pango_SetFont(font%):"_Pango_SetFont@4"
Pango_StringWidth%(str$):"_Pango_StringWidth@4"
Pango_StringHeight%(str$):"_Pango_StringHeight@4"
Pango_FreeFont(font%):"_Pango_FreeFont@4"

Pango_Text_(buffer%, x%, y%, text$, align_x%, align_y%):"_Pango_Text@24"