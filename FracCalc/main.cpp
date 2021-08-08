
#include "FracNum.h"
#include "FracCalc.h"
#include <iostream>
#include <cassert>
#include <vector>
#include <cstring>

#if !defined(RUN_UNIT_TESTS)

int
processCurrentArguments(
    const int   argc,
    const char  *argv[] )
{
    //
    // Combine the arguments into one expression and do one calculation
    // before returning
    
    std::string program;
    program.reserve( 128 ); // To minimize string reallocation when appending
    
    //
    // Combine all the arguments into one program
    
    for( int iarg=1; iarg<argc; ++iarg )
    {
        program += argv[ iarg ];
    }

    FracNum num;
    FracCalc calc;

    const bool success = calc.calculate( program.c_str(), num );
    if( success )
    {
        std::cout << "= " << num.toString() << '\n';
    }
    
    return success?0:-2;
}

bool
wasQuitTyped(
    const std::string &aInput )
{
    const std::string quit( "quit" );

    const size_t pos = aInput.find( quit );
    if ( std::string::npos != pos )
    {
        if( 0==pos || isspace( aInput[pos-1] ))
        {
            if( pos+quit.length() == aInput.length() || isspace( aInput[ pos+quit.length() ] ))
            {
                return true;
            }
        }
    }
    
    return false;
}

void
enterCalculatorMode()
{
    std::cout << "Fraction Calculator for OneLogin\nType in your fraction expression then press return\n";
    std::cout << "You can type 'quit' to exit the calculator\n";
    std::cout << "? " << std::flush;
    
    std::string input;        
    while( std::getline( std::cin, input ))
    {
        if( wasQuitTyped( input )) break;
        
        FracNum num;
        FracCalc calc;
    
        const bool success = calc.calculate( input.c_str(), num );
        if( success )
        {
            std::cout << "= " << num.toString() << '\n';
        }
        
        std::cout << "? " << std::flush;
    }
}

int
main(
    int         argc,
    const char  *argv[] )
{
    if( argc > 1 )
    {
        return processCurrentArguments( argc, argv );
    }
    
    enterCalculatorMode();    
    return 0;
}

#else

struct test_t
{
    std::string program;
    std::string response;

} testRun[] = {
    { "", "0" },
    { " ", "0" },
    { "  ", "0" },
    { "(1/2)", "1/2" },
    { "((1/2))", "1/2" },
    { "(((-1/2)))", "-1/2" },
    { "(((((-1/2)))))", "-1/2" },
    { "0", "0" },
    { "0 + 0", "0" },
    { "3*3/4", "2_1/4" },
    { "(1+2)*(-3/4)", "-2_1/4" },
    { "(1/2)", "1/2" },
    { "1/2 + 1/3", "5/6" },
    { "1/2 + 1/3 + 10/11", "1_49/66" },
    { "5/6 + 10/11", "1_49/66" },
    { "4/3 * 1/10", "2/15" },
    { "1/2 + 2/15 + 5", "5_19/30" },
    { "1/2 + 4/3 * 1/10 + 5 + 0", "5_19/30" },
    { "1/2 + 1/3 + 10/11 - 4/3 * 1/10 + 2 + 0", "3_67/110" },
    { " ( 1 - -123 / 14 ) ", "9_11/14" },
    { "(1/2 + 1/3) * (7/8 - 3/8)", "5/12" },
    { "5/6 * 1/2", "5/12" },
    { "1/2 / 1/2", "1" },
    { "1/3 / 1/2", "2/3" },
    { "-1/2--1/2", "0" },
    { "-123/14", "-8_11/14" },
    { "123/14", "8_11/14" },
    { "-1/-2 + 1", "1_1/2" },
    { "1/-2 + 1", "1/2" },
    { "1/2 + ((1/2) + 10/20)", "1_1/2" },
    { "((10/2))", "5" },
    { "10", "10" },
    { "4/8", "1/2" },
    { "3_1/2", "3_1/2" },
    { "37/22", "1_15/22" },
    { "3_1/0", "Zero denominator" }
};
    
int
main()
{
    std::cout << "Running Unit Tests:\n";
    
    size_t failures = 0;
    size_t tests = 0;
    
    for( const auto &test : testRun )
    {
        FracCalc calc;
        FracNum result;
        
        calc.setSilent();        
        calc.calculate( test.program.c_str(), result );
        
        const std::string resultString = result.toString();
        if( 0 != test.response.compare( resultString ))
        {
            std::cout << "Expecting: " << test.response << " found " << resultString << '\n';
            ++failures;
        }
        
        ++tests;
    }
    
    std::cout << '\t' << std::to_string( tests ) << " tests\n";
    std::cout << '\t' << std::to_string( failures ) << " failures" << std::endl;
}

#endif // RUN_UNIT_TESTS

