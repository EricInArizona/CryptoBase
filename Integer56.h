// Copyright Eric Chauvin 2022


// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html


#pragma once


#include "../CppBase/BasicTypes.h"



class Integer56
  {
  private:
  bool testForCopy = false;
  static const Uint32 bitMask28 = 0xFFFFFFF;
                               // 0xFFF FFFF

  Uint64 N0 = 0;
  Uint64 N1 = 0;
  Uint64 N2 = 0;
  Uint64 N3 = 0;
  Uint64 N4 = 0; // Overflow

  public:
  inline Integer56( void )
    {
    setZero();
    }

  inline Integer56( const Integer56& in )
    {
    if( in.testForCopy )
      return;

    throw "Integer56 copy constructor.";
    }

  inline ~Integer56( void )
    {
    }


  inline void setZero( void )
    {
    N0 = 0;
    N1 = 0;
    N2 = 0;
    N3 = 0;
    N4 = 0;
    }

  inline void setOne( void )
    {
    N0 = 1;
    N1 = 0;
    N2 = 0;
    N3 = 0;
    N4 = 0;
    }

  inline void setFromULong( const Uint64 toSet )
    {
    N0 = toSet & bitMask28;
    N1 = (toSet >> 28) & bitMask28;
    N2 = toSet >> 56;
    N3 = 0;
    N4 = 0;
    }

  inline void addULong( const Uint64 toAdd )
    {
    N0 += toAdd & bitMask28;
    N1 += (toAdd >> 28) & bitMask28;
    N2 += (toAdd >> 56) & bitMask28;
    // N3 ...

    cleanCarry();
    }

  inline void cleanCarry( void )
    {
    Uint64 carry = N0 >> 28;
    N0 = N0 & bitMask28;

    N1 += carry;
    carry = N1 >> 28;
    N1 = N1 & bitMask28;

    N2 += carry;
    carry = N2 >> 28;
    N2 = N2 & bitMask28;

    N3 += carry;
    carry = N3 >> 28;
    N3 = N3 & bitMask28;

    N4 += carry;
    carry = N4 >> 28;
    N4 = N4 & bitMask28;

    if( carry != 0 )
      throw "Integer56 overflow.";

    }

  inline void add( const Integer56& toAdd )
    {
    // This doesn't check sign.

    N0 += toAdd.N0;
    N1 += toAdd.N1;
    N2 += toAdd.N2;
    N3 += toAdd.N3;
    N4 += toAdd.N4;

    cleanCarry();
    }

  inline bool isZero( void ) const
    {
    if( (N0 == 0) &&
        (N1 == 0) &&
        (N2 == 0) &&
        (N3 == 0) &&
        (N4 == 0) )
      {
      return true;
      }

    return false;
    }


  inline bool isOne( void ) const
    {
    if( (N0 == 1) &&
        (N1 == 0) &&
        (N2 == 0) &&
        (N3 == 0) &&
        (N4 == 0) )
      return true;

    return false;
    }


  inline void copy( Integer56& from )
    {
    N0 = from.N0;
    N1 = from.N1;
    N2 = from.N2;
    N3 = from.N3;
    N4 = from.N4;
    }


  inline void multiply( Integer56& x,
                        Integer56& y )
    {
    // Multiply 28 bit numbers.
    Uint64 low =  x.N0 * y.N0;
    Uint64 mid1 = x.N0 * y.N1;
    Uint64 mid2 = x.N1 * y.N0;
    Uint64 high = x.N1 * y.N1;

    N0 = low & bitMask28;
    Uint64 carry = low >> 28;

    if( (carry >> 28) != 0 )
      throw "This can never happen. carry 1.";

    // Three times the biggest 56 bit number
    // is 56 bits shifted left 2 bits.
    // So 58 bits.  Plenty of room.

    Uint64 mid = carry + mid1 + mid2;
    N1 = mid & bitMask28;
    carry = mid >> 28;

    // carry might have 28 + 2 bits in it.
    if( (carry >> 28) != 0 )
      throw "This can happen. carry 2.";

    if( (carry >> 30) != 0 )
      throw "This can't be right. carry 2.";

    Uint64 high2 = carry + high;
    N2 = high2  & bitMask28;

    N3 = (high2 >> 28) & bitMask28;

    // At most that's 8 bits.
    N4 = high2 >> 56;

    // This should not happen.
    if( N4 != 0 )
      throw "Can this ever happen with N4?";

    }


  inline void setHighPart( Integer56& hPart )
    {
    hPart.N0 = N2;
    hPart.N1 = N3;
    hPart.N2 = N4; // Overflow.
    hPart.N3 = 0;
    hPart.N4 = 0;
    }

  inline void setLowPart( Integer56& lPart )
    {
    lPart.N0 = N0;
    lPart.N1 = N1;
    lPart.N2 = 0;
    lPart.N3 = 0;
    lPart.N4 = 0;
    }


  inline void clearHighPart( void )
    {
    N2 = 0;
    N3 = 0;
    N4 = 0;
    }


  };
