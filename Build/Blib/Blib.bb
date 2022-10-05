; Blib - A part of BlitzToolbox
; Zip Utils for Blitz3D.
; v1.0 2022.10.3
; https://github.com/ZiYueCommentary/BlitzToolbox

Function BlibCreateZip%(file$, password$ = "")
	Return BlibCreateZip_(file, password)
End Function

Function BlibOpenZip%(file$, password$ = "")
	Return BlibOpenZip_(file, password)
End Function