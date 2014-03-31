#include "BigInt.h"
#include <sstream>
#include <cmath>
#include <cctype>
#include <cstdlib>
#include <algorithm>
#include <stdexcept>
#include <queue>
// #define BigInt BigInt

//private
BigInt BigInt::zero = 0ul;

BigInt::buffer_type BigInt::conv_ulong ( unsigned long n )
{
    if ( n == 0 )
        return buffer_type ( 1, 0 );

    buffer_type t;
    t.reserve ( sizeof(n)*3 );

    while ( n )
    {
        t.push_back ( n%10 );
        n/=10;
    }

    return t;
}
BigInt::buffer_type BigInt::conv_float ( double x )
{
    return conv_ulong ( (unsigned long) ( std::abs(x) ) );
}
BigInt::buffer_type BigInt::conv_string ( const string& s )
{
    buffer_type t ( 1, 0 );
    t.reserve ( s.size() );

    unsigned i = 0;
    if ( s[0] == '+' || s[0] == '-' )
        i = 1;

    while ( std::isdigit ( s[i] ) )
        t.push_back ( s[i++]-'0' );

    std::reverse(t.begin(),t.end());
    return t;
}

BigInt::buffer_type& BigInt::remove_zero ( buffer_type& b )
{
    while ( b.size() > 1 && b[b.size()-1] == 0 )
        b = b.substr(0,b.size()-1);
    return b;
}

BigInt::reference BigInt::normalize()
{
    // leading 0
    remove_zero(buffer);

    // +-0
    if ( buffer.size() == 1 && buffer[0] == 0 )
        sign_ = false;

    return *this;
}

void BigInt::overflow ( unsigned start )
{
    for ( unsigned i = start; i < buffer.size() && buffer[i] >= 10; i++ )
    {
        buffer[i] %= 10;
        if ( i == buffer.size() -1 )
            buffer.push_back ( 1 );
        else
            buffer[i+1]++;
    }
}
void BigInt::underflow ( unsigned start )
{
    for ( unsigned i = start; i < buffer.size() && buffer[i] < 0; i++ )
    {
        buffer[i] += 10;
        if ( i == buffer.size() -1 )
            return; // something weird happened
        buffer[i+1]--;
    }
}
// constructors
BigInt::BigInt ( unsigned long n ) : sign_(false), buffer ( conv_ulong(n) )
{
    normalize();
}
BigInt::BigInt ( signed long n ) : sign_( n < 0 ), buffer ( conv_ulong(std::abs(n)) )
{
    normalize();
}
BigInt::BigInt ( int n ) : sign_( n < 0 ), buffer ( conv_ulong(std::abs(n)) )
{
    normalize();
}
BigInt::BigInt ( double x ) : sign_( x < 0 ), buffer ( conv_float(x) )
{
    normalize();
}
BigInt::BigInt ( const string& s ) : sign_(false), buffer ( conv_string(s) )
{
    if ( s[0] == '-' )
        sign_ = true;
    normalize();
}
BigInt::BigInt ( const char* s ) : sign_(false), buffer ( conv_string(s) )
{
    if ( s[0] == '-' )
        sign_ = true;
    normalize();
}

 // assignment
BigInt::reference   BigInt::operator= ( unsigned long n )
{
    sign_ = false;
    buffer = conv_ulong(n);
    return normalize();
}
BigInt::reference   BigInt::operator= ( signed long n )
{
    sign_ = n < 0;
    buffer = conv_ulong(std::abs(n));
    return normalize();
}
BigInt::reference   BigInt::operator= ( int n )
{
    return *this = long ( n );
}
BigInt::reference   BigInt::operator= ( string s )
{
    sign_ = s[0] == '-';
    buffer = conv_string ( s );
    return normalize();
}
BigInt::reference   BigInt::operator= ( const char* s )
{
    return *this = string(s);
}
BigInt::reference   BigInt::operator= ( double x )
{
    sign_ = x < 0;
    buffer = conv_float(x);
    return normalize();
}

// conversion
signed long BigInt::slong() const
{
    // out of range ?

    signed long t = 0, e = 1;

    for ( unsigned long i = 0; i < buffer.size(); i++, e*=10 )
        t += buffer[i]*e;

    return t * sign();
}
unsigned long BigInt::ulong() const
{
    unsigned long t = 0, e = 1;

    for ( unsigned long i = 0; i < buffer.size(); i++, e*=10 )
        t += buffer[i]*e;

    return t;
}
BigInt::string BigInt::str() const
{
    string r ( buffer.size()+1, 0 );
    r[0] = sign_ ? '-' : '+';
    for ( unsigned i = 1; i <= buffer.size(); i++ )
        r[i] = buffer[buffer.size()-i]+'0';
    return r;
}

// sign
BigInt::value BigInt::operator-() const
{
    if ( operator== ( zero ) )
        return *this;

    BigInt t ( *this );
    t.sign_ = !t.sign_;
    return t;
}
signed BigInt::sign() const
{
    if ( operator== ( zero ) )
        return 0;
    return sign_ ? -1 : 1;
}
BigInt::reference BigInt::neg()
{
    if ( operator!= ( zero ) )
        sign_ = !sign_;
    return *this;
}
//arithmetic
BigInt::reference BigInt::operator++ ()
{
    if ( sign_ )
    {
        abs();
        operator-- ();
        return neg();
    }

    buffer[0]++; // *should* always exist

    overflow(0);

    return normalize();
}
BigInt::reference BigInt::operator-- ()
{
    if ( sign_ || operator==(zero) )
    {
        abs();
        operator++ ();
        return neg();
    }

    buffer[0]--;

    underflow(0);

    return normalize();
}
BigInt::reference BigInt::operator+= ( const_reference o )
{
    if ( sign_ )
    {
        abs();
        operator-= ( o );
        return neg();
    }
    else if ( o.sign_ )
    {
        return operator-= ( -o );
    }

    if ( buffer.size() < o.buffer.size() )
        buffer.resize(o.buffer.size());

    for ( unsigned i = 0; i < o.buffer.size(); i++ )
    {
        buffer[i]+=o.buffer[i];
        if ( buffer[i] >= 10 )
        {
            buffer[i]-=10;
            if ( i >= buffer.size()-1 )
                buffer.push_back ( 1 );
            else
                buffer[i+1]++;
        }
    }
    overflow ( o.buffer.size()-1 );
    return normalize();
}

BigInt::reference   BigInt::operator-= ( const_reference o )
{
    if ( sign_ )
    {
        abs();
        operator+= ( o );
        return neg();
    }
    else if ( o.sign_ )
    {
        return operator+= ( -o );
    }
    else if ( *this < o )
    {
        BigInt t = o;
        t -= *this;
        return *this = -t;
    }

    for ( unsigned i = 0; i < o.buffer.size(); i++ )
    {
        buffer[i]-=o.buffer[i];
        if ( buffer[i] < 0 )
        {
            buffer[i] += 10;
            buffer[i+1]--; // [i+1] exists because o <= *this ( or so I hope )
        }
    }

    underflow ( o.buffer.size()-1 );

    return normalize();
}

BigInt::reference BigInt::multiply_by_digit ( digit_type d )
{
    if ( d == 0 )
        return *this = zero;
    if ( d != 1 )
    {
        for ( unsigned i = 0; i < buffer.size(); i++ )
            buffer[i]*= d;

        // check/fix overflow
        for ( unsigned i = 0; i < buffer.size(); i++ )
            if ( buffer[i] >= 10 )
            {
                digit_type n = buffer[i] / 10;
                buffer[i] %= 10;
                if ( i >= buffer.size()-1 )
                    buffer.push_back ( n );
                else
                    buffer[i+1] += n;
            }
    }
    return *this;
}
BigInt::reference BigInt::operator*= ( const_reference o )
{
    if ( o.buffer.size() == 1 )
        return multiply_by_digit ( o.buffer[0] );

    sign_ = sign_ != o.sign_;

    std::queue<BigInt> partial;

    for ( unsigned i = 0; i < o.buffer.size(); i++ )
    {
        BigInt copy = *this;
        copy.multiply_by_digit ( o.buffer[i] );
        partial.push ( copy );
    }

    BigInt temp;
    *this = zero;
    for( unsigned i = 0; !partial.empty(); i++ )
    {
        temp = partial.front();
        partial.pop();
        temp.buffer = buffer_type(i,0) + temp.buffer;
        *this += temp;
    }
    return normalize();
}
BigInt::reference BigInt::operator/= ( value o )
{
    if ( o == zero )
        throw std::domain_error ( "Division by zero" );

    bool rsign = false;

    if ( sign_ )
    {
        abs();
        rsign = true;
    }
    if ( o.sign_ )
    {
        o.abs();
        rsign = !rsign;
    }

    BigInt q = zero;
    while ( *this >= 0 )
    {
        operator-= ( o );
        ++q;
    }
    // now, *this + o = r
    --q;

    buffer= q.buffer;
    sign_ = rsign;

    return normalize();
}

BigInt::value BigInt::operator+ ( const_reference o )
{
    return BigInt(*this) += o;
}
BigInt::value BigInt::operator- ( const_reference o )
{
    return BigInt(*this) -= o;
}
BigInt::value BigInt::operator* ( const_reference o )
{
    return BigInt(*this) *= o;
}
BigInt::value BigInt::operator/ ( const_reference o )
{
    return BigInt(*this) /= o;
}

// comparison
bool BigInt::operator<  ( const_reference o ) const
{
    if ( &o == &zero )
        return sign_;

    if ( sign_ != o.sign_ )
        return sign_;
    if ( sign_ && o.sign_ )
        return ::abs(*this) > ::abs(o);

    if ( buffer.size() != o.buffer.size() )
        return buffer.size() < o.buffer.size();

    for ( long i = buffer.size()-1; i >= 0; i-- )
        if ( buffer[i] != o.buffer[i] )
            return buffer[i] < o.buffer[i];

    return false; // equal
}
bool BigInt::operator<= ( const_reference o ) const
{
    if ( &o == &zero )
        return sign_;

    if ( sign_ != o.sign_ )
        return sign_;
    if ( sign_ && o.sign_ )
        return ::abs(*this) >= ::abs(o);

    if ( buffer.size() != o.buffer.size() )
        return buffer.size() < o.buffer.size();

    for ( long i = buffer.size()-1; i >= 0; i-- )
        if ( buffer[i] != o.buffer[i] )
            return buffer[i] <= o.buffer[i];

    return true;
}
bool BigInt::operator== ( const_reference o ) const
{
    if ( &o != &zero && sign_ != o.sign_ )
        return false;

    if ( buffer.size() != o.buffer.size() )
        return false;

    for ( long i = buffer.size()-1; i >= 0; i-- )
        if ( buffer[i] != o.buffer[i] )
            return false;

    return true;
}
bool BigInt::operator!= ( const_reference o ) const
{
    return ! operator== ( o );
}
bool BigInt::operator>= ( const_reference o ) const
{
    return ! operator< ( o );
}
bool BigInt::operator>  ( const_reference o ) const
{
    return ! operator<= ( o );
}


std::ostream& operator<< ( std::ostream& os, const BigInt& BigIntn )
{
    return os << BigIntn.str();
}

std::istream& operator>> ( std::istream& is, BigInt& BigIntn )
{
    std::string t;
    is >> t;
    BigIntn = t;
    return is;
}
