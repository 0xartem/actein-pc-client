makecert.exe -n "CN=Actein AS Root,O=Actein AS,C=NO" -r -pe -a sha512 -len 4096 -cy authority -sv CAActeinRootKey.pvk CAActeinRootCert.cer
pvk2pfx.exe -pvk CAActeinRootKey.pvk -spc CAActeinRootCert.cer -pfx CAActeinRoot.pfx -po password

makecert.exe -n "CN=Actein AS,O=Actein AS,C=NO" -iv CAActeinRootKey.pvk -ic CAActeinRootCert.cer -pe -a sha512 -len 4096 -sky signature -eku 1.3.6.1.5.5.7.3.3 -sv ActeinCodeSignKey.pvk ActeinCodeSignCert.cer
pvk2pfx.exe -pvk ActeinCodeSignKey.pvk -spc ActeinCodeSignCert.cer -pfx ActeinCodeSign.pfx -po password
