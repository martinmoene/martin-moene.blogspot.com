// curve.hpp

#include <iostream>
#include <memory>
#include <string>
#include <vector>

// crtp_cast:

template<class D, class B> D                & crtp_cast(B                & p) { return static_cast<D                &>( p ); }
template<class D, class B> D const          & crtp_cast(B const          & p) { return static_cast<D const          &>( p ); }
template<class D, class B> D       volatile & crtp_cast(B volatile       & p) { return static_cast<D       volatile &>( p ); }
template<class D, class B> D const volatile & crtp_cast(B const volatile & p) { return static_cast<D const volatile &>( p ); }

// Scanner interface and several scanner types:

struct Scanner
{
    virtual std::string type() const = 0;
};

struct ScannerCommon : Scanner { };

using ScannerPtr = std::shared_ptr<Scanner>;

struct NullScanner : ScannerCommon
{
    std::string type() const { return "0"; };
};

struct BiasScanner : ScannerCommon
{
    std::string type() const { return "B"; };
};

struct ZAxisScanner : ScannerCommon
{
    std::string type() const { return "Z"; };
};

ScannerPtr create_scanner( std::string type )
{
    if      ( type == "B" ) return std::make_shared<BiasScanner>();
    else if ( type == "Z" ) return std::make_shared<ZAxisScanner>();

    return std::make_shared<NullScanner>();
}

// Condition interface and several Condition types:

struct Condition
{
    virtual std::string type() const = 0;
};

using ConditionPtr = std::shared_ptr<Condition>;

struct ConditionCommon : Condition { };

struct NullCondition : ConditionCommon
{
    std::string type() const { return "0"; };
};

struct LessEqualThreshold : ConditionCommon
{
    std::string type() const { return "<="; };
};

struct GreaterEqualThreshold : ConditionCommon
{
    std::string type() const { return ">="; };
};

struct LengthCondition : ConditionCommon
{
    std::string type() const { return "L"; };
};

ConditionPtr create_condition( std::string /* channel */, std::string op, std::string /* value */ )
{
    if      ( op == "<=" ) return std::make_shared<LessEqualThreshold   >( /* channel, value */ );
    else if ( op == ">=" ) return std::make_shared<GreaterEqualThreshold>( /* channel, value */ );

    return std::make_shared<NullCondition>();
}

ConditionPtr create_condition( std::string /* length */ )
{
    return std::make_shared<LengthCondition>( /* length */ );
}

// Segment interface:

struct Segment
{
    virtual ~Segment() = 0;

    virtual void condition( ConditionPtr scan ) = 0;

    virtual void apply( ScannerPtr scanner ) = 0;
};

Segment::~Segment() {}

using SegmentPtr = std::unique_ptr<Segment>;

// Factor out code common to all classes using SegmentParameter<>:

struct SegmentCommon : Segment
{
    ConditionPtr the_condition{ std::make_shared<NullCondition>() };

    void condition( ConditionPtr cond ) { the_condition = cond; }

    void print( std::string segment, std::string scanner ) const
    {
        std::cout << segment << scanner << the_condition->type() << " ";
    }
};

// Tool for static polymorphic method chaining:

template <typename Derived>
struct SegmentParameter : SegmentCommon
{
#define self crtp_cast<Derived>(*this)

    Derived & stop_on( ConditionPtr cond )
    {
        condition( cond );
        return self;
    }

#undef self
};

// Various segment types and curve:

struct Approach : SegmentParameter<Approach>
{
    void apply( ScannerPtr scanner ) { print( "A", scanner->type() ); }
};

struct Retract : SegmentParameter<Retract>
{
    void apply( ScannerPtr scanner ) { print("R", scanner->type() ); }
};

struct Curve : SegmentParameter<Curve>
{
    int num_times{ 1 };
    std::vector<SegmentPtr> segments{};
    ScannerPtr the_scanner{ std::make_shared<NullScanner>() };

    template< typename T >
    Curve & add( T const & segment )
    {
        segments.emplace_back( new T( segment ) );
        return *this;
    }

    Curve & unless( bool skip )
    {
        if ( skip )
            segments.pop_back();
        return *this;
    }

    Curve & times( int n )
    {
        num_times = n;
        return *this;
    }

    Curve & scans( ScannerPtr scanner )
    {
        the_scanner = scanner;
        return *this;
    }

    void apply( ScannerPtr scanner )
    {
        for ( auto const & x : segments )
            x->apply( scanner );
    }

    void sweep()
    {
        for ( int i = 0; i < num_times; ++i)
            apply( the_scanner );
    }
};
