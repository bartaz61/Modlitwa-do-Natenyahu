@echo off
title Kompilowanie programu SDL2

echo.
echo ==============================
echo      KOMPILOWANIE PROGRAMU
echo ==============================
echo.

gcc main.c -o ModlitwaDoNatenyahu.exe ^
    -ISDL2/include/SDL2 ^
    -LSDL2/lib ^
    -lmingw32 ^
    -lSDL2main ^
    -lSDL2 ^
    -lSDL2_image ^
    -lSDL2_mixer

if errorlevel 1 (
    echo.
    echo ==============================
    echo       BLAD KOMPILACJI!
    echo ==============================
    echo.
    pause
    exit /b 1
)

echo.
echo ==============================
echo       KOMPILACJA OK!
echo ==============================
echo.
echo Utworzono: ModlitwaDoNatenyahu.exe
echo.

pause