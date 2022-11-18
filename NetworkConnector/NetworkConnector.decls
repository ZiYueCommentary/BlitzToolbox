; NetworkConnector - A part of BlitzToolbox
; Parse domain's TXT and download files.
; v1.02 2022.11.19
; https://github.com/ZiYueCommentary/BlitzToolbox

.lib "NetworkConnector.dll"

DownloadFile(url$, file$):"_DownloadFile@8"
GetDomainTXT$(url$):"_GetDomainTXT@4"
ParseDomainTXT$(txt$, key$):"_ParseDomainTXT@8"