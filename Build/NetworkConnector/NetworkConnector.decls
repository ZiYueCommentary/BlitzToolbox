; NetworkConnector - A part of BlitzToolbox
; Parse domain's TXT and download files.
; v1.01 2022.10.6
; https://github.com/ZiYueCommentary/BlitzToolbox

.lib "NetworkConnector.dll"

DownloadFile(url$, file$):"_DownloadFile@8"
GetDomainTXT$(url$):"_GetDomainTXT@4"
ParseDomainTXT$(url$, section$):"_ParseDomainTXT@8"