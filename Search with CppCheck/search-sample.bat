@echo off
::
:: Usage: search-sample.bat [cppcheck-options]
::
:: cppcheck options, e.g.
::    --enable=(id) 
::    (id): all, warning, style, performance, portability, 
::          information, unusedFunction, missingInclude
::

call set-path.bat
cppcheck %* --rule-file="my-rule.xml" sample.cpp
