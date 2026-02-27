@echo off
REM manual setup. 
REM set "VCROOT=C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.43.34808"
REM set "SDKROOT=C:\Program Files (x86)\Windows Kits\10"

REM set "INCLUDE=%VCROOT%\include;%SDKROOT%\Include\10.0.22621.0\ucrt;%SDKROOT%\Include\10.0.22621.0\um;%SDKROOT%\Include\10.0.22621.0\shared"
REM set "LIB=%VCROOT%\lib\x64;%SDKROOT%\Lib\10.0.22621.0\ucrt\x64;%SDKROOT%\Lib\10.0.22621.0\um\x64"
REM set "PATH=%VCROOT%\bin\Hostx64\x64;%SDKROOT%\bin\10.0.22621.0\x64;%PATH%"


REM for automatic setup but have to run every time. before complieing program

call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat"
