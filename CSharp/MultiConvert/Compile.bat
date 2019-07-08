setlocal enabledelayedexpansion enableextensions
set LIBS=
for %%x in (*.dll) do (
  echo %%x|findstr ",">nul 2>&1
  if errorlevel 1 (set LIBS=!LIBS!,%%x) else (set LIBS=!LIBS!,"%%x")
)
if "%LIBS%"=="" (for /F %%v in ('dir /s %WINDIR%\Microsoft.NET\csc.exe /B') do echo %%v -debug %%* > cscLatest.bat ) else (for /F %%v in ('dir /s %WINDIR%\Microsoft.NET\csc.exe /B') do echo %%v -debug /r:%LIBS:~1% %%* > cscLatest.bat )
if "%1" == "" cscLatest.bat *.cs
cscLatest.bat %*
