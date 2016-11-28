@echo off
setlocal
REM Call batch file to set the password
call "%1"
signtool.exe sign /v /f "%2" /p %SIGNPASS% /t http://timestamp.verisign.com/scripts/timstamp.dll "%3"
endlocal
