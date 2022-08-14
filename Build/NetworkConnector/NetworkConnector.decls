; NetworkConnector - A part of BlitzToolbox
; Parse domain's TXT and download files.
; https://github.com/ZiYueCommentary/BlitzToolbox

.lib "NetworkConnector.dll"

DownloadFile(url$, file$):"_DownloadFile@8"
GetDomainTXT$(url$):"_GetDomainTXT@4"
ParseDomainTXT$(url$, section$):"_ParseDomainTXT@8"