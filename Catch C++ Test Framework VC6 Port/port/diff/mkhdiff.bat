@echo off & setlocal
::
:: mkhdiff.bat - Create colorised unified diff of catch.hpp, catch-vc6.hpp as html.
::

if not [%1] == [] (
    echo Usage: mkhdiff
    exit /b 1
)

set name=catch
set postfix=-vc6

set    org=%name%
set    vc6=%name%%postfix%
set diffed=%org%-diff

set     diff=diff
set diff2html=..\..\Tools\diff2html\diff2html.py

%diff% -u %org%.hpp %vc6%.hpp > %diffed%.txt

%diff2html% -i %diffed%.txt -o %diffed%.html

echo %0: Output saved in file %diffed%.html

endlocal

::
:: end of file
::
