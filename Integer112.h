// Copyright Eric Chauvin 2022


// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html



#pragma once


#include "../CppBase/BasicTypes.h"
#include "Integer56.h"



class Integer112
  {
  private:
  bool testForCopy = false;

  Integer56 N0;
  Integer56 N1;
  Integer56 N2;
  Integer56 N3;
  Integer56 N4; // Overflow

  public:
  inline Integer112( void )
    {
    setZero();
    }

  inline Integer112( const Integer112& in )
    {
    if( in.testForCopy )
      return;

    throw "Integer112 copy constructor.";
    }

  inline ~Integer112( void )
    {
    }


  inline void setZero( void )
    {
    N0.setZero();
    N1.setZero();
    N2.setZero();
    N3.setZero();
    N4.setZero();
    }


  inline void setOne( void )
    {
    N0.setOne();
    N1.setZero();
    N2.setZero();
    N3.setZero();
    N4.setZero();
    }

  inline void setFromULong( const Uint64 toSet )
    {
    N0.setFromULong( toSet );
    N1.setZero();
    N2.setZero();
    N3.setZero();
    N4.setZero();

    // cleanCarry();
    }

  inline void addULong( const Uint64 toAdd )
    {
    N0.addULong( toAdd );
    N1.setZero();
    N2.setZero();
    N3.setZero();
    N4.setZero();

    cleanCarry();
    }


  inline void cleanCarry( void )
    {
    Integer56 carry;

    N0.setHighPart( carry );
    N0.clearHighPart();

    N1.add( carry );
    N1.setHighPart( carry );
    N1.clearHighPart();

    N2.add( carry );
    N2.setHighPart( carry );
    N2.clearHighPart();

    N3.add( carry );
    N3.setHighPart( carry );
    N3.clearHighPart();

    N4.add( carry );
    N4.setHighPart( carry );
    // N4.clearHighPart();

    if( !carry.isZero() )
      throw "Integer112 overflow.";

    }



  inline void add( const Integer112& toAdd )
    {
    N0.add( toAdd.N0 );
    N1.add( toAdd.N1 );
    N2.add( toAdd.N2 );
    N3.add( toAdd.N3 );
    N4.add( toAdd.N4 );

    cleanCarry();
    }


  inline bool isZero( void ) const
    {
    if( N0.isZero() &&
        N1.isZero() &&
        N2.isZero() &&
        N3.isZero() &&
        N4.isZero())
      {
      return true;
      }

    return false;
    }


  inline bool isOne( void ) const
    {
    if( N0.isOne() &&
        N1.isZero() &&
        N2.isZero() &&
        N3.isZero() &&
        N4.isZero())
      return true;

    return false;
    }


  inline void copy( Integer112& from )
    {
    N0.copy( from.N0 );
    N1.copy( from.N1 );
    N2.copy( from.N2 );
    N3.copy( from.N3 );
    N4.copy( from.N4 );
    }


  inline void multiply( Integer112& x,
                        Integer112& y )
    {
    // Multiply 112 bit numbers.
    Integer56 low;
    Integer56 mid1;
    Integer56 mid2;
    Integer56 mid;
    Integer56 high;
    Integer56 high2;
    Integer56 carry;

    low.multiply( x.N0, y.N0 );
    mid1.multiply( x.N0, y.N1 );
    mid2.multiply( x.N1, y.N0 );
    high.multiply( x.N1, y.N1 );

    N0.setHighPart( carry );
    N0.clearHighPart();

    mid.copy( carry );
    mid.add( mid1 );
    mid.add( mid2 );

    // Set N1 to the low part of mid.
    mid.setLowPart( N1 );
    mid.setHighPart( carry );

    high2.copy( carry );
    high2.add( high );

    high2.setLowPart( N2 );
    high2.setHighPart( N3 );

    N3.setHighPart( N4 );
    N3.clearHighPart();
    }

/*
  inline Uint64 getMod64( Uint64 x )
    {
    if( (!N2.isZero()) ||
        (!N3.isZero()) ||
        (!N4.isZero()) )
      throw "Integer112 getMod64.";

    Uint64 smallPart = N0.getAsULong() % x;
    Uint64 basePart = 0xFFF FFFF FFF FFFFULL % x;
    Uint64 bigPart = N1.getAsULong() % x;

Add these up mod x.

    result = result % x;
    return result;
    }
*/


/*
  inline void getHighPart( Integer56& hPart )
    {
    hPart.N0 = N2;
    hPart.N1 = N3;
    hPart.N2 = N4; // Overflow.
    hPart.N3 = 0;
    hPart.N4 = 0;
    }

  inline void getLowPart( Integer56& lPart )
    {
    lPart.N0 = N0; // 28 bits.
    lPart.N1 = N1; // All 56 bits.
    lPart.N2 = 0;
    lPart.N3 = 0;
    lPart.N4 = 0;
    }

*/

  };
