Search with CppCheck
======================

This folder contains code for my article *Search with CppCheck* in ACCU's [Overload journal issue 120](http://accu.org/var/uploads/journals/Overload120.pdf#page=1) of April 2014.

Files
-------
`my-rule.xml` - CppCheck rule to match if-else-if clamp construct.

`my-rule-extended.xml` - CppCheck rule to prevent matching longer if-else-if chains.

`my-rule-loose.xml` - CppCheck rule to match any code in if-else blocks.

`parse-sample.bat` - show CppCheck parse of sample.cpp.

`search-sample.bat` - show CppCheck match of rule my-rule.xml on sample.cpp.

`sample.cpp` - sample code to match with CppCheck rule file my-rule.xml.
