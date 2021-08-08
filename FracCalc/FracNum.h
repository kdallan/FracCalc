
#pragma once

#include <cassert>
#include <cmath>
#include <iostream>

class FracNum final
{
public:

    int64_t num;
    int64_t den;
    
    void
    simplify() noexcept
    {
        if( 0 == den ) return; // Already infinity

        //
        // Always want the denominator to be +ve
        
        if( den < 0 )
        {
            num = -num;
            den = -den;
        }

        const int64_t divisor = GCD( std::abs( num ), den );
        assert( 0 != divisor );
        
        num /= divisor;
        den /= divisor;
    }
    
    const std::string
    toString() const
    {        
        std::string str;        
        if( 0 == den ) return "Infinity";
        
        if( std::abs( num ) > den )
        {
            const int64_t whole = num/den;
            assert( 0 != whole );
            
            const int64_t newnum = std::abs( num )-(den * std::abs( whole ));
            
            if( newnum )
            {
                str = std::to_string( whole );
                str += "_";
                str += std::to_string( newnum ) + "/" + std::to_string( den );
            }
            else
            {
                str = std::to_string( whole );
            }
        }
        else
        {
            if( 1 != den )
            {
                str = std::to_string( num ) + "/" + std::to_string( den );
            }
            else
            {
                str = std::to_string( num );
            }
        }
    
        return str;
    }
        
private:

    static uint64_t
    GCD(
        uint64_t   a,
        uint64_t   b ) noexcept
    {
        while( b > 0 )
        {
            const uint64_t r = a % b;
            a = b;
            b = r;
        }

        return a;
    }
};

constexpr FracNum FracZero{ 0, 1 };
constexpr FracNum FracOne{ 1, 1 };
constexpr FracNum FracInfinity{ 1, 0 };
