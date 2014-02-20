@echo off
::
:: set-path.bat - add path for CppCheck to PATH, unless already present.
::

set text=%PATH%
if not "%text:cppcheck=%" == "%text%" goto :EOF

echo Adding CppCheck to path
PATH=%PATH%;C:\Program Files\Cppcheck;C:\Program Files (x86)\Cppcheck
