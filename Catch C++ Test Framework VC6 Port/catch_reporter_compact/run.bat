@echo off
::
::
::

set exercise=exercise_reporter.exe -r compact
set line=echo ----------------------------------------------------------

echo.
echo Print failing tests
%line%
%exercise%

echo.
echo Print failing and succeeding tests
%line%
%exercise% -s

echo.
echo Select no tests
%line%
%exercise% -t "pass/empty test case"

echo.
echo Warn about no assertions in test case
%line%
%exercise% -w NoAssertions -t "pass/empty test case"

echo.
echo Warn about no assertions in section
%line%
%exercise% -w NoAssertions -t "pass/test case with empty section"

echo.
echo Select only succeeding tests
%line%
%exercise% -t "pass/*"

echo.
echo Console reporter
%line%
exercise_reporter.exe
