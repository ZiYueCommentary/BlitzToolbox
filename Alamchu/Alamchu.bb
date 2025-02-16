; Alamchu - A part of BlitzToolbox
; A text layout engine library for displaying multi-language text.
; v1.00 2025.2.16
; https://github.com/ZiYueCommentary/BlitzToolbox

Const PANGO_TEXT_LEFT% = 0
Const PANGO_TEXT_CENTER% = 1
Const PANGO_TEXT_RIGHT% = 2
Const PANGO_TEXT_TOP% = 0
Const PANGO_TEXT_MIDDLE% = 1
Const PANGO_TEXT_BOTTOM% = 2

Function Pango_Text(x%, y%, text$, align_x% = PANGO_TEXT_LEFT, align_y% = PANGO_TEXT_TOP)
    Pango_Text_(GraphicsBuffer(), x, y, text, align_x, align_y)
End Function