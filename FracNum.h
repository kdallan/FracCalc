
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
        str.reserve( 64 );
        
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

    int64_t
    GCD(
        int64_t   a,
        int64_t   b )
    {
        while( b > 0 )
        {
            const int64_t r = a % b;
            a = b;
            b = r;
        }
    
        return a;
    }
};
