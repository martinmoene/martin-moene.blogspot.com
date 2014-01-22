#include "curve-extended-times-alt.hpp"

int main()
{
    try
    {
        using once  = Once;
        using times = Times;

        // run-time configurable:

        const int  Nsweep    = 1;
        const int  Nsection  = 2;
        const bool onceX     = true;

              auto scanner   = create_scanner  ( "Z" );
        const auto distance  = create_condition( "123 nm" );
        const auto threshold = create_condition( "chan1", "<=", "2.7 V" );

        Curve curve;

        curve.times( Nsweep )
             .scans( scanner )
             .add  (        Retract().stop_on( distance )   ).unless( onceX )
             .add  ( once ( Retract().stop_on( distance ) ) ).when  ( onceX )
             .add  ( times( Nsection )
                     .add ( Dwell()                         )
                     .add ( Approach().stop_on( threshold ) )
                     .add ( Dwell()                         )
                     .add ( Retract ().stop_on( distance  ) ) )
             ;

        std::cout << "\n1.curve.sweep(): "; curve.sweep();
        std::cout << "\n2.curve.sweep(): "; curve.sweep();
    }
    catch ( std::exception const & e )
    {
        std::cout << "Error: " << e.what() << std::endl;
    }
}

// Output:
// 1.curve.apply(): RZL TZ0 AZ>= TZ0 RZL TZ0 AZ>= TZ0 RZL
// 2.curve.apply(): TZ0 AZ>= TZ0 RZL TZ0 AZ>= TZ0 RZL

// cl -nologo -W3 -EHsc curve-extended-times-alt.cpp && curve-extended-times-alt
// g++ -Wall -Wextra -Weffc++ -std=c++11 -o curve-extended-times-alt.exe curve-extended-times-alt.cpp && curve-extended-times-alt
