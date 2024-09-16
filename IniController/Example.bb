Include "IniController.bb"

IniWriteString("options.ini", "foo", "bar", "wow")
IniWriteBuffer("options.ini")
Print IniGetString("options.ini", "foo", "bar")
WaitKey