@echo off
for %%f in (curve curve-extended curve-extended-times-alt) do (
    echo.
    echo %%f
    g++ -Wall -Wextra -Weffc++ -std=c++11 -o %%f.exe %%f.cpp && %%f.exe
    echo.
)

:: cl -nologo -W3 -EHsc %%f.cpp && %%f.exe
