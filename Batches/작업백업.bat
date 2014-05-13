REM @echo off

IF "%~1" == "" goto err

REM dir %HAVOK_THIRDPARTY_DIR%\..\programtrunk\
REM @echo "%~dp1"

cd /d "%~dp1"

for /F "usebackq delims=" %%f IN (%~nx1) DO xcopy "%HAVOK_THIRDPARTY_DIR%\..\programtrunk\%%f" "%%~dpf"


goto end

:err
@echo %0 filelist



:end
pause