@echo off
for %%f in (example-1 example-2) do (
    echo.
    echo %%f
    g++ -o %%f.exe %%f.cpp && %%f.exe
    cl -nologo -W3 -EHsc %%f.cpp && %%f.exe
)
