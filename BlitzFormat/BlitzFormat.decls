; BlitzFormat - A part of BlitzToolBox
; String formatter.
; v1.0 2022.8.15
; https://github.com/ZiYueCommentary/BlitzToolbox

.lib "BlitzFormat.dll"

Format$(fmt$, arg$):"_Format@8"
Format2$(fmt$, arg1$, arg2$):"_Format2@12"
Format3$(fmt$, arg1$, arg2$, arg3$):"_Format3@16"
Format4$(fmt$, arg1$, arg2$, arg3$, arg4$):"_Format4@20"
Format5$(fmt$, arg1$, arg2$, arg3$, arg4$, arg5$):"_Format5@24"
Format6$(fmt$, arg1$, arg2$, arg3$, arg4$, arg5$, arg6$):"_Format6@28"
Format7$(fmt$, arg1$, arg2$, arg3$, arg4$, arg5$, arg6$, arg7$):"_Format7@32"
Format8$(fmt$, arg1$, arg2$, arg3$, arg4$, arg5$, arg6$, arg7$, arg8$):"_Format8@36"
Format9$(fmt$, arg1$, arg2$, arg3$, arg4$, arg5$, arg6$, arg7$, arg8$, arg9$):"_Format9@40"
Format10$(fmt$, arg1$, arg2$, arg3$, arg4$, arg5$, arg6$, arg7$, arg8$, arg9$, arg10$):"_Format10@44"