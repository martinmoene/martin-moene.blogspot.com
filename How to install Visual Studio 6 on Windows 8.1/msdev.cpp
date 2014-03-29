
#include <process.h>

const char* path = "C:\\Program Files (uac)\\Microsoft Visual Studio\\Common\\MSDev98\\Bin\\MSDEV8.EXE";

int main( int argc, char * argv[] )
{
    return spawnvp( P_WAIT, path, argv );
}

// VC12 (VS2013)
// cl -nologo -EHsc msdev.cpp
