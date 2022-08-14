; IniControler - A part of BlitzToolBox
; Write & Read ini file.
; https://github.com/ZiYueCommentary/BlitzToolbox

Function IniWriteString(path$, section$, key$, value$, updateBuffer% = 1)
	IniWriteString_(path$, section$, key$, value$, updateBuffer%)
End Function

Function IniWriteInt(path$, section$, key$, value%, updateBuffer% = 1)
	IniWriteInt_(path$, section$, key$, value%, updateBuffer%)
End Function

Function IniWriteFloat(path$, section$, key$, value#, updateBuffer% = 1)
	IniWriteFloat_(path$, section$, key$, value#, updateBuffer%)
End Function

Function IniGetString$(path$, section$, key$, defaultValue$ = "", allowBuffer% = 1)
	Return IniGetString_(path$, section$, key$, defaultValue$, allowBuffer%)
End Function

Function IniGetInt%(path$, section$, key$, defaultValue% = 0, allowBuffer% = 1)
	Return IniGetInt_(path$, section$, key$, defaultValue%, allowBuffer%)
End Function

Function IniGetFloat#(path$, section$, key$, defaultValue# = 0.0, allowBuffer% = 1)
	Return IniGetFloat_(path$, section$, key$, defaultValue#, allowBuffer%)
End Function

Function IniGetBufferString$(path$, section$, key$, defaultValue$ = "")
	Return IniGetBufferString_(path$, section$, key$, defaultValue$)
End Function

Function IniGetBufferInt%(path$, section$, key$, defaultValue% = 0)
	Return IniGetBufferInt_(path$, section$, key$, defaultValue%)
End Function

Function IniGetBufferFloat#(path$, section$, key$, defaultValue# = 0.0)
	Return IniGetBufferFloat_(path$, section$, key$, defaultValue#)
End Function