/*
 *  Created by Martin on 14/12/2012.
 *  Copyright 2012 Two Blue Cubes Ltd. All rights reserved.
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef TWOBLUECUBES_CATCH_REPORTER_COMPACT_HPP_INCLUDED
#define TWOBLUECUBES_CATCH_REPORTER_COMPACT_HPP_INCLUDED

namespace Catch {

    struct CompactReporter : AccumulatingReporter {
        CompactReporter( ReporterConfig const& _config )
        : AccumulatingReporter( _config )
        {}

        virtual ~CompactReporter();

        static std::string getDescription() {
            return "Reports test results as compact text";
        }

        virtual ReporterPreferences getPreferences() const {
            ReporterPreferences prefs;
            prefs.shouldRedirectStdOut = false;
            return prefs;
        }

        virtual void assertionStarting( AssertionInfo const& ) {
        }

        virtual void assertionEnded( Ptr<AssertionStats const> const& _assertionStats ) {

            AssertionResult const& result = _assertionStats->assertionResult;

            // Drop out if result was successful and we're not printing those
            if( !m_config.includeSuccessfulResults() && result.isOk() )
                return;

            printLineInfo( result.getSourceInfo() );

            switch( result.getResultType() ) {
                // info and warning print 'info:' resp. 'warning:' themselves
                case ResultWas::Info:
                case ResultWas::Warning:
                    break;
                // for rest, indicate success or failure:
                default:
                {
                    if( result.succeeded() ) {
                        TextColour colour( TextColour::ResultSuccess );
                        stream << "passed";
                    }
                    else {
                        TextColour colour( TextColour::ResultError );
                        stream << "failed";
                        if( result.isOk() ) {
                            stream << ", but was ok";
                        }
                    }
                    stream << ": ";
                }
            }

            switch( result.getResultType() ) {
                case ResultWas::ThrewException:
                {
                    stream << "unexpected exception with message: '" << result.getMessage() << "'";
                    if( result.hasExpression() ) {
                        stream << "; ";
                        // NTS: abusing text colour; gray/dim white
                        TextColour colour( TextColour::FileName );
                        stream << "expression was: ";
                    }
                }
                    break;
                case ResultWas::DidntThrowException:
                {
                    stream << "expected exception, got none";
                    if( result.hasExpression() ) {
                        stream << "; ";
                        // NTS: abusing text colour; gray/dim white
                        TextColour colour( TextColour::FileName );
                        stream << "expression was: ";
                    }
                }
                    break;
                case ResultWas::Info:
                {
                    stream << "info: " << result.getMessage();
//                    streamVariableLengthText( "info", result.getMessage() );
                }
                    break;
                case ResultWas::Warning:
                {
                    // NTS: abusing text colour; like bright orange for warnings
                    {
                        TextColour colour( TextColour::ReconstructedExpression );
                        stream << "warning: ";
                    }
                    stream << result.getMessage();

//                    streamVariableLengthText( "warning", result.getMessage() );
                }
                    break;
                case ResultWas::ExplicitFailure:
                {
                    stream << "with message: '" << result.getMessage() << "'";
                }
                    break;
                case ResultWas::Unknown: // These cases are here to prevent compiler warnings
                case ResultWas::Ok:
                case ResultWas::FailureBit:
                case ResultWas::ExpressionFailed:
                case ResultWas::Exception:
                    if( result.hasMessage() ) {
                        stream << "with message: " << result.getMessage();
//                        streamVariableLengthText( "with message", result.getMessage() );
                    }
                    break;
            }

            stream << result.getExpression();

            if( result.hasExpandedExpression() ) {
                {
                    // NTS: abusing text colour; gray/dim white
                    TextColour colour( TextColour::FileName );
                    stream << " for: ";
                }
//                if( result.getExpandedExpression().size() > 40 ) {
//                    stream << "\n";
//                    if( result.getExpandedExpression().size() < 70 )
//                        stream << "\t";
//                }

                stream << result.getExpandedExpression();
            }

            stream << std::endl;
        }

//        void streamVariableLengthText( std::string const& prefix, std::string const& text ) {
//            std::string trimmed = trim( text );
//            if( trimmed.find_first_of( "\r\n" ) == std::string::npos ) {
//                stream << prefix << ": " << trimmed;
//            }
//            else {
//                stream  << "\n" << prefix << " >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n" << trimmed
//                        << "\nend of " << prefix << " <<<<<<<<<<<<<<<<<<<<<<<<\n";
//            }
//        }

        void printTotals( const Totals& totals ) {
            if( totals.assertions.total() == 0 ) {
                stream << "No tests ran";
            }
            else if( totals.assertions.failed ) {
                TextColour colour( TextColour::ResultError );
                stream << "Failures: "
                    << totals.testCases.failed << " of " << pluralise( totals.testCases.total(), "test case" ) << ", "
                    << totals.assertions.failed << " of " << pluralise( totals.assertions.total(), "assertion" );

            }
            else {
                TextColour colour( TextColour::ResultSuccess );
                stream << "All tests passed ("
                    << pluralise( totals.assertions.passed, "assertion" ) << " in "
                    << pluralise( totals.testCases.passed, "test case" ) << ")";
            }
        }

        virtual void sectionEnded( Ptr<SectionStats const> const& _sectionStats ) {
            resetLastPrintedLine();
            if( _sectionStats->missingAssertions ) {
                printLineInfo( _sectionStats->sectionInfo.lineInfo );
                // NTS: abusing text colour; like bright orange for warnings
                {
                    TextColour colour( TextColour::ReconstructedExpression );
                    stream << "warning: ";
                }
                stream << "no assertions in section '" << _sectionStats->sectionInfo.name << "'" << std::endl;
            }
            AccumulatingReporter::sectionEnded( _sectionStats );
        }

        virtual void testCaseEnded( Ptr<TestCaseStats const> const& _testCaseStats ) {
            resetLastPrintedLine();
            if( _testCaseStats->missingAssertions ) {
                printLineInfo( _testCaseStats->testInfo.lineInfo );
                // NTS: abusing text colour; like bright orange for warnings
                {
                    TextColour colour( TextColour::ReconstructedExpression );
                    stream << "warning: ";
                }
                stream << "no assertions in test case '" << _testCaseStats->testInfo.name << "'" << std::endl;
            }
            AccumulatingReporter::testCaseEnded( _testCaseStats );
        }

        virtual void testGroupEnded( Ptr<TestGroupStats const> const& _testGroupStats ) {
            AccumulatingReporter::testGroupEnded( _testGroupStats );
        }

        virtual void testRunEnded( Ptr<TestRunStats const> const& _testRunStats ) {
            if( !unusedTestCaseInfo ) {
                printTotals( _testRunStats->totals );
                stream << std::endl;
            }
            AccumulatingReporter::testRunEnded( _testRunStats );
        }

        void printLineInfo( SourceLineInfo const& lineInfo ) {
            if( !lineInfo.empty() ) {
                stream << lineInfo;
            }
        }

        void resetLastPrintedLine() {
            m_lastPrintedLine = SourceLineInfo();
        }
        SourceLineInfo m_lastPrintedLine;
    };

    CompactReporter::~CompactReporter() {}

    INTERNAL_CATCH_REGISTER_REPORTER( "compact", CompactReporter )

} // end namespace Catch

#endif // TWOBLUECUBES_CATCH_REPORTER_COMPACT_HPP_INCLUDED
