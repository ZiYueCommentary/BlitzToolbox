DownloadFile("https://scpcbgame.com/changelog.txt", "changelog.txt")
Print "Download Completed!"
Print "DomainTXT: " + ParseDomainTXT(GetDomainTXT("version.scpcbgame.cn"), "version")
WaitKey