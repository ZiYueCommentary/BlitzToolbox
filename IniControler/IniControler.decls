.lib "IniControler.dll"

IniWriteBuffer(path$):"_IniWriteBuffer@4"
IniWriteString(path$, section$, key$, value$, updateBuffer%=1):"_IniWriteString@20"
IniWriteInt(path$, section$, key$, value%, updateBuffer%=1):"_IniWriteInt@20"
IniWriteFloat(path$, section$, key$, value#, updateBuffer%=1):"_IniWriteFloat@20"
IniGetString$(path$, section$, key$, defaultValue$="", allowBuffer%=1):"_IniGetString@20"
IniGetInt%(path$, section$, key$, defaultValue%=0, allowBuffer%=1):"_IniGetInt@20"
IniGetFloat#(path$, section$, key$, defaultValue#=0.0, allowBuffer%=1):"_IniGetFloat@20"