; OpenCC extension - A part of BlitzToolbox
; Conversion between Traditional and Simplified Chinese.
; https://github.com/ZiYueCommentary/BlitzToolbox

.lib "OpenCC.dll"

CreateOpenCC%(configFileName$):"_CreateOpenCC@4"
OpenCConvert$(converter%, input$):"_OpenCConvert@8"
FreeOpenCC(converter%):"_FreeOpenCC@4"