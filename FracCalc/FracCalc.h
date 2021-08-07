
#pragma once

#include "FracNum.h"
#include <cassert>
#include <iostream>

class FracCalc final
{
public:

    FracCalc()
        : mPtr( nullptr ),
          mStart( nullptr ),
          mError( 0 ),
          mSilent( false ),
          mEOS( '\0' )
    {
    }
        
    bool
    calculate(
        const char * const  aProblem,
        FracNum             &aAnswer )
    {
        if( nullptr == aProblem ) return false;
    
        //
        // Rentrant. Reset the necessary variables
    
        mStart  = const_cast<char*>( aProblem );
        mPtr    = mStart;
        mError  = 0;
        
        //
        // Start of the recursive descent parser
        
        parseExpression( aAnswer );
        
        //
        // Make sure we parsed everything correctly
        
        expect( '\0' );
        
        return 0 == mError;
    }
    
    void
    setSilent(
        bool aSilent = true )
    {
        //
        // Suppress any output.
        
        mSilent = aSilent;
    }

private:

    char        mEOS;           // Dummy memory location so mPtr can point to EOS
    char        *mStart;        // Start of the program. Keep this to calculate any error position
    char        *mPtr;          // Current parsing location
    uint8_t     mError;         // Error code. 0 - no error
    bool        mSilent;        // true is we want to silence any parse error output

    inline void
    ignoreWhitespace()
    {
        while( isspace( *mPtr )) ++mPtr;
    }

    void
    parseError(
        const std::string &aMessage )
    {
        if( mError ) return; // Only report the first error
    
        mError = 1; // Room for different error codes in the future
        
        if( !mSilent )
        {
            std::cout << mStart << '\n';
        
            const int64_t pos = mPtr - mStart; // Position in the string where the error occurred

            std::string pointer( pos, ' ' );
            pointer += '^';
            std::cout << pointer << '\n';
        
            std::cout << aMessage << '\n';
        }
        
        mPtr = &mEOS; // To suppress further parsing as expression unwinds (we're at a dummy EOS)
    }
    
    const std::string
    charOrEOS(
        const char aChar )
    {
        if( '\0' == aChar ) return "end of string";
        
        return std::string( 1, aChar );
    }
    
    void
    expectError(
        const std::string &aError )
    {
        std::string err( "Expecting " );
        err += aError + " found ";
        err += charOrEOS( *mPtr );
        parseError( err );
    }
    
    bool
    expect(
        const char  aChar )
    {
        ignoreWhitespace();
        
        if( aChar != *mPtr )
        {
            std::string err{ charOrEOS( aChar ) };
            expectError( err );
            return false;
        }
        
        if( '\0' != *mPtr ) ++mPtr; // Eat expected character
        return true;
    }
    
    bool
    expectNumber()
    {
        if( !isdigit( *mPtr ) && '+' != *mPtr && '-' != *mPtr )
        {
            expectError( "a number" );
            return false;
        }
        
        return true;
    }
    
    int64_t
    parseInteger()
    {
        if( !expectNumber()) return 0;
        
        const bool neg = '-' == *mPtr;
        if( neg || '+' == *mPtr )
        {
            ++mPtr;
            ignoreWhitespace();
        }
        
        int64_t num = 0;
        while( isdigit( *mPtr ))
        {
            num = (10*num) + (*mPtr-'0');
            ++mPtr;
        }
        
        return neg?-num:num;
    }
    
    void
    evaluate(
        const FracNum   &aOperand0,
        const FracNum   &aOperand1,
        const char      aOperator,
        FracNum         &aResult )
    {
        switch( aOperator )
        {
        case '+':
            aResult.num = aOperand0.num * aOperand1.den + aOperand1.num * aOperand0.den;
            aResult.den = aOperand0.den * aOperand1.den;
            break;
            
        case '-':
            aResult.num = aOperand0.num * aOperand1.den - aOperand1.num * aOperand0.den;
            aResult.den = aOperand0.den * aOperand1.den;
            break;
            
        case '*':
            aResult.num = aOperand0.num * aOperand1.num;
            aResult.den = aOperand0.den * aOperand1.den;
            break;
            
        case '/':
            {
                int64_t newnum = aOperand0.num * aOperand1.den;
                aResult.den = aOperand0.den * aOperand1.num;
                aResult.num = newnum;
            }
            break;
                     
        default:
            assert( !"Illegal operator!" );
            break;
        }
        
        aResult.simplify();
    }
    
    void
    parseFactor(
        FracNum &aFracNum )
    {
        ignoreWhitespace();
        
        if ( '(' == *mPtr )
        {
            ++mPtr; // Eat (
            parseExpression( aFracNum );
            expect( ')' );
        }
        else parseFracNumber( aFracNum );
    }
    
    void
    parseTerm(
        FracNum &aFracNum )
    {
        parseFactor( aFracNum );
        
        for(;;)
        {
            ignoreWhitespace();
            
            const char op = *mPtr;
            if( '*' == op || '/' == op )
            {
                ++mPtr; // Eat * /

                FracNum b;
                parseFactor( b );
            
                evaluate( aFracNum, b, op, aFracNum );
            }
            else break;
        }
    }
    
    void
    parseExpression(
        FracNum &aFracNum )
    {
        parseTerm( aFracNum );
        
        for(;;)
        {
            ignoreWhitespace();
            
            const char op = *mPtr;
            if( '+' == op  || '-' == op )
            {
                ++mPtr; // Eat + -

                FracNum b;
                parseTerm( b );

                evaluate( aFracNum, b, op, aFracNum );
            }
            else break;
        }
    }

    bool
    parseFracNumber(
        FracNum &aFracNum )
    {
        aFracNum.num = parseInteger();
        aFracNum.den = 1;
        
        ignoreWhitespace();
    
        if( '/' == *mPtr )
        {
            ++mPtr; // Eat /
            aFracNum.den = parseInteger();
        }
        else if( '_' == *mPtr
#if defined(ALLOW_WHOLE_FRAC_SPACE)
                ||  isdigit( *mPtr ) // // allow 3_1/2 or 3 1/2
#endif
                )
        {
            if( '_' == *mPtr) ++mPtr; // Eat _
            
            const int64_t numerator = parseInteger();
            if( !expect( '/' )) return false;
            
            aFracNum.den = parseInteger();
            ignoreWhitespace();
            
            aFracNum.num = aFracNum.num*aFracNum.den + numerator;
        }
        
        if( 0 == aFracNum.den )
        {
            parseError( "Divide by zero" );
            return false;
        }
        
        aFracNum.simplify();
        return true;
    }
};

