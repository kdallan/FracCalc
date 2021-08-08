
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
        assert( 0 != den );
        
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
        if( 0 == den )
        {
            return "Zero denominator";
        }
        
        if( std::abs( num ) > den )
        {
            const int64_t whole = num/den;
            assert( 0 != whole );
            
            const int64_t newnum = std::abs( num )-(den * std::abs( whole ));
            
            if( newnum )
            {
                str = std::to_string( whole );
#if defined(ALLOW_WHOLE_FRAC_SPACE)
                str += " ";
#else
                str += "_";
#endif
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

#if defined( USE_BINARY_GCD )
    static uint64_t
    GCD(
        uint64_t u,
        uint64_t v ) noexcept
    {
        const uint64_t t = u | v;
        if( 0==u || 0==v ) return t;

        const int commonfactor = __builtin_ctzll( t );
        u >>= __builtin_ctzll( u );
        
        do
        {
            v >>= __builtin_ctzll( v );
            if( u > v )
            {
                uint64_t tmp = v;
                v = u;
                u = tmp;
            }
            v = v - u;
        }
        while( 0 != v );
        
        return u << commonfactor;
    }
#else
    static uint64_t
    GCD(
        uint64_t   a,
        uint64_t   b )
    {
        while( b > 0 )
        {
            const uint64_t r = a % b;
            a = b;
            b = r;
        }

        return a;
    }
#endif // USE_BINARY_GCD
};
