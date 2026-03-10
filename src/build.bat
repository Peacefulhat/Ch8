@echo off

set CommonCompilerFlags=-MTd -Oi -GR -Gm- -W4 -wd4201 -wd4100 -FC -Z7 
set CommonLinkerFlags=-incremental:no User32.lib Gdi32.lib Winmm.lib raylibdll.lib

set RaylibLibPath=D:\Raylib\lib
set RaylibIncludePath=D:\Raylib\include

IF NOT EXIST ..\build mkdir ..\build

pushd ..\build

cl %CommonCompilerFlags% ..\src\chip-8.cpp ..\src\core.cpp /I%RaylibIncludePath% /link /LIBPATH:%RaylibLibPath% %CommonLinkerFlags%
popd
