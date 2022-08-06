DownloadFile("https://scpcbgame.cn/changelog.txt", "changelog.txt")
Print "Download Completed!"
Print "DomainTXT: " + ParseDomainTXT(GetDomainTXT("version.scpcbgame.cn"), "version")
WaitKey