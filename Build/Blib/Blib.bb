; Blib - A part of BlitzToolbox
; Zip Utils for Blitz3D.
; v1.01 2023.11.5
; https://github.com/ZiYueCommentary/BlitzToolbox

Function BlibCreateZip%(file$, password$ = "")
	Return BlibCreateZip_(file, password)
End Function

Function BlibOpenZip%(file$, password$ = "")
	Return BlibOpenZip_(file, password)
End Function