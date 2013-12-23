// curve.cpp

#include "curve.hpp"

int main()
{
    // run-time configurable:

    const int  Nsweep = 1;
    const bool skipR1 = false;

          auto scanner   = create_scanner  ( "Z" );
    const auto distance  = create_condition( "123 nm" );
    const auto threshold = create_condition( "chan1", ">=", "2.7 V" );

    Curve curve;

    curve.times( Nsweep  )
         .scans( scanner )
         .add  ( Retract ().stop_on( distance  ) ).unless( skipR1 )
         .add  ( Approach().stop_on( threshold ) )
         .add  ( Retract ().stop_on( distance  ) )
         ;

    std::cout << "curve.sweep(): "; curve.sweep();
}

// Output: curve.sweep(): RZL AZ>= RZL

// cl -nologo -W3 -EHsc curve.cpp && curve
// g++ -Wall -Wextra -Weffc++ -std=c++11 -o curve.exe curve.cpp && curve
