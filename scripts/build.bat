@ECHO OFF
SETLOCAL ENABLEDELAYEDEXPANSION

echo Buildsystem for BeepPlayer - Windows
echo (c) 2020 Gabriel Daenner
echo.

if not exist ..\src (
    echo Error: No source directory found.
    echo Closing application ...
    echo.
    pause
    exit /b
)

if not exist ..\obj (
    echo Error: No object directory found.
    echo Closing application ...
    echo.
    pause
    exit /b
)

if not exist ..\bin (
    echo Error: No build directory found.
    echo Closing application ...
    echo.
    pause
    exit /b
)

echo Cleaning up ...
cd ..\bin
if exist *.exe (
    del *.exe
)
cd ..\obj
if exist *.o (
    del *.o
)
cd ..\scripts

echo Searching for g++ in path ...
where /q g++.exe
if ERRORLEVEL 1 (
    echo Error: g++ seems to be missing. Ensure it is installed and placed in your PATH.
    echo Closing application ...
    echo.
    pause
    exit /b
)

echo Searching for windres in path ...
where /q windres.exe
if ERRORLEVEL 1 (
    echo Error: windres seems to be missing. Ensure it is installed and placed in your PATH.
    echo Closing application ...
    echo.
    pause
    exit /b
)

echo Building object files ...
g++ -O3 -c -o ..\obj\main.o ..\src\main.cpp
g++ -O3 -c -o ..\obj\player.o ..\src\player.cpp
windres -o ..\obj\app.o ..\src\app.rc

echo Building application ...
g++ -O3 -o ..\bin\BeepPlayer.exe ..\obj\main.o ..\obj\player.o ..\obj\app.o -mwindows

echo.
pause
