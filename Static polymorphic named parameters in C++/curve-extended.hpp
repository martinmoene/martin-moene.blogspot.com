//
// Some background information
//
// The following code exists to construct and generate a curve.
//
// A curve can be generated (swept) a number of times.
// A curve exists (is a collection) of sections and several kinds of segments.
// Like a curve, a section also is a collection of sections and segments.
//
// There are two modifiers:
// - once: perform a section or segment once per sweep.
// - times: perform a section or segment N times in succession.
//
// Further an element can be skipped via member unless(bool skip), or included
// via member when(bool include).
//
// Thus a basic curve structure can be specified in code, with several
// variations that can be provided at runtime (see main() in curve-extended.cpp).
//
// In constructing the curve the specific types of section, segments and
// modifiers are needed to construct the proper element and to enable postfixing
// the unless() and when() methods to skip or include an element at runtime.
// Elements are constructed as temporary 'exemplars' and copies of them are
// placed in dynamic storage via template<typename T> Section::add(T const &).
//
// When generating the curve, sections and curves are referred to via the
// interface Segments. See Curve::sweep() and Section::Apply().
//
// Note: in the real 'thing' sections take several and varied arguments.
//
// Curiously recurring template pattern:
// - http://talesofcpp.fusionfenix.com/post-12/episode-eight-the-curious-case-of-the-recurring-template-pattern.
// - http://eli.thegreenplace.net/2011/05/17/the-curiously-recurring-template-pattern-in-c/ .

#include <iostream>
#include <memory>
#include <stdexcept>
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

    virtual void reset() = 0;

    virtual void condition( ConditionPtr condition ) = 0;

    virtual void apply( ScannerPtr scanner ) = 0;
};

Segment::~Segment() {}

using SegmentPtr = std::shared_ptr<Segment>;

// Factor out code common to all classes using SegmentParameter<>:

struct SegmentCommon : Segment
{
    ConditionPtr the_condition{ std::make_shared<NullCondition>() };

    void reset() { }

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

// Various segment types and modifiers:

struct Approach : SegmentParameter<Approach>
{
    void apply( ScannerPtr scanner ) { print( "A", scanner->type() ); }
};

struct Retract : SegmentParameter<Retract>
{
    void apply( ScannerPtr scanner ) { print("R", scanner->type() ); }
};

struct Dwell : SegmentParameter<Dwell>
{
    void apply( ScannerPtr scanner ) { print("T", scanner->type() ); }
};

struct Once : SegmentParameter<Once>
{
    bool is_done = false;
    const SegmentPtr segment;

    template< typename T >
    Once( T const & segment )
   : segment( new T( segment ) ) {}

    bool done() const
    {
        return is_done;
    }

    void reset()
    {
        segment->reset();
        is_done = false;
    }

    void apply( ScannerPtr scanner )
    {
        if ( done() )
            return ;

        segment->apply( scanner );
        is_done = true;
    }
};

struct Times : SegmentParameter<Times>
{
    const int n;
    const SegmentPtr segment;

    template< typename T >
    Times( int n, T const & segment )
    : n( n ), segment( new T( segment ) ) {}

    void reset()
    {
        segment->reset();
    }

    void apply( ScannerPtr scanner )
    {
        for ( int i = 0 ; i < n; ++i )
            segment->apply( scanner );
    }
};

// Section, a collection of segments and modifiers:

struct Section : SegmentParameter<Section>
{
    using SegmentParameter<Section>::SegmentParameter;

    std::vector<SegmentPtr> segments{ };

    template< typename T >
    Section & add( T const & segment )
    {
        segments.emplace_back( new T( segment ) );
        return *this;
    }

    Section & unless( bool const skip )
    {
        if ( skip )
        {
            if ( segments.empty() )
                throw std::runtime_error( "Curve::unless() called, but no segment present" );
            segments.pop_back();
        }
        return *this;
    }

    Section & when( bool const include )
    {
        return unless( ! include );
    }

    void reset()
    {
        for ( auto const & x : segments )
            x->reset();
    }

    void apply( ScannerPtr scanner )
    {
        for ( auto const & x : segments )
            x->apply( scanner );
    }
};

// Curve, applies number of sweeps to its segments (sections).

struct Curve : Section
{
    int num_times{ 1 };
    ScannerPtr the_scanner{ std::make_shared<NullScanner>() };

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

    void sweep()
    {
        for ( int i = 0; i < num_times; ++i)
            apply( the_scanner );
    }
};
