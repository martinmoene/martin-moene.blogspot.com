/*
 *  exercise_reporter.cpp
 *
 *  Created by Martin on 14/12/2012.
 *  Copyright 2012 Two Blue Cubes Ltd. All rights reserved.
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

/*
 * Microsoft recommends using the multithreaded, dynamic link library (/MD or /MDd compiler option) to avoid possible problems.[36]
 * http://en.wikipedia.org/wiki/Visual_C%2B%2B
 */

#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "catch_reporter_compact.hpp"

// Catch-VC6 cannot handle i+3 etc. as left-hand expression

TEST_CASE( "pass/comparison/ok", "" )
{
    const int i = 5;
    REQUIRE( i == 2 + i - 2 );
}

TEST_CASE( "fail/comparison/error", "" )
{
    const int i = 5;
    REQUIRE( i == 2 + i - 1 );
}

TEST_CASE( "fail/exception/unexpected/no message", "" )
{
    throw std::exception();
}

TEST_CASE( "fail/exception/unexpected/with message", "" )
{
    throw std::runtime_error("throwing unexpected exception");
}

void foo( std::string const & text ) {}

TEST_CASE( "pass/exception/expected/not throwing", "" )
{
    REQUIRE_NOTHROW( foo( "not throwing" ) );
}

TEST_CASE( "fail/exception/expected/not throwing", "" )
{
    REQUIRE_THROWS( foo( "not throwing" ) );
}

TEST_CASE( "fail/exception/expected/wrong type", "" )
{
    REQUIRE_THROWS_AS( throw std::runtime_error("throwing with wrong type"), std::range_error );
}

TEST_CASE( "fail/info", "" )
{
    INFO( "this is info going with the following failure" );
    FAIL( "failure for above info" );
}

TEST_CASE( "fail/scoped info", "" )
{
    {
        SCOPED_INFO( "this is scoped info unrelated to the following failure" );
    }
    {
        SCOPED_INFO( "this is scoped info going with the following failure" );
        FAIL( "failure for above scoped info" );
    }
}

TEST_CASE( "fail/capture", "" )
{
    const double m = 1, c = 299792458;
    CAPTURE( (m * c * c) );
    FAIL( "failure for above capture" );
}

TEST_CASE( "pass/succeed", "" )
{
    SUCCEED( "message from succeed" );
}

TEST_CASE( "pass/warning", "" )
{
    WARN( "this is a warning" );
}

TEST_CASE( "pass/test case with section", "" )
{
    SECTION( "section", "" )
    {
        WARN( "this is a section" );
    }
}

TEST_CASE( "pass/empty test case", "" )
{
}

TEST_CASE( "pass/test case with empty section", "" )
{
    SECTION( "empty section", "" )
    {
    }
}

// These generate long expanded expressions:

inline const char* testStringForMatching()
{
    return "this string contains 'abc' as a substring";
}

TEST_CASE("pass/matchers", "")
{
    REQUIRE_THAT( testStringForMatching(), Contains( "string" ) );
    CHECK_THAT( testStringForMatching(), Contains( "abc" ) );

    CHECK_THAT( testStringForMatching(), StartsWith( "this" ) );
    CHECK_THAT( testStringForMatching(), EndsWith( "substring" ) );
}

TEST_CASE("fail/matchers", "")
{
    CHECK_THAT( testStringForMatching(), Contains( "Catch" ) );
}

// cl -nologo -W3 -EHsc -GR -MD exercise_reporter.cpp && exercise_reporter.exe -r compact
// g++ -Wall -o exercise_reporter.exe exercise_reporter.cpp && exercise_reporter.exe -r compact
