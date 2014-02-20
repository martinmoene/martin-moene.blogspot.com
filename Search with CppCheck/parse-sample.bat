@echo off
::
:: parse-sample.bat - Show how sample.cpp is parsed by CppCheck.
::

call set-path.bat
cppcheck %* --rule=".+" sample.cpp
