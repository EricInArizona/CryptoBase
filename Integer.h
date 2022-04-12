// Copyright Eric Chauvin, 2021 - 2022.



#pragma once



#include "../LinuxApi/BasicTypes.h"
#include "../LinuxApi/Str.h"
#include "../LinuxApi/RangeC.h"

#include "ProjConst.h"
#include "IntBuf.h"



class Integer
  {
  private:
  Int32 testforCopy = 123;
  bool negative = false;
  Int32 index = 0;

  // It is Int64 to hold multiplied values.
  // I want a fixed array that gets pushed on
  // to the stack.  In other words there is
  // no memory allocation that gets done when
  // an Integer is created other than to move
  // the stack pointer.
  // See the /STACK option in BuildProj.bat
  Int64 D[ProjConst::digitArraySize] = { 0,1,2 };
  // Int64* D;

/*
  void setOneDValueFromChar( Int64 toSet,
                Int32 position, Int32 offset );
  char getOneCharFromDValue(
                Int32 position, Int32 offset );
  void setOneDValueFromByte( Int64 toSet,
                             Int32 atIndex,
                             Int32 offset );
  char getOneByteFromDValue(
                  Int32 atIndex, Int32 offset );

*/

  public:
  Integer( void );
  Integer( const Integer& obj );
  // ~Integer();

  inline void setToZero( void )
    {
    negative = false;
    index = 0;
    D[0] = 0;
    }

  inline void setToOne( void )
    {
    negative = false;
    index = 0;
    D[0] = 1;
    }


  inline bool isZero( void ) const
    {
    if( (index == 0) && (D[0] == 0) )
      return true;

    return false;
    }

  inline bool isOne( void ) const
    {
    if( negative )
      return false;

    if( (index == 0) && (D[0] == 1) )
      return true;

    return false;
    }

  inline bool isNegativeOne( void ) const
    {
    if( !negative )
      return false;

    if( (index == 0) && (D[0] == 1) )
      return true;

    return false;
    }

  inline bool isMoreThanInt24( const Int32 check )
                                           const
    {
    RangeC::test2( check, 0, 0xFFFFFF,
            "Integer.isMoreThanInt24() size." );

    if( negative )
      return false;

    if( index > 0 )
      return true;

    if( D[0] > check )
      return true;

    return false;
    }

  inline bool getNegative( void ) const
    {
    return negative;
    }

  inline void setNegative( bool setTo )
    {
    if( setTo )
      {
      if( isZero())
        throw "Setting zero to negative.";

      }

    negative = setTo;
    }

  inline Int32 getIndex( void ) const
    {
    return index;
    }

  inline void setIndex( const Int32 setTo )
    {
    // RangeC::test( setTo, 0,
       //        ProjConst::digitArraySize - 1,
       //         "Integer.setIndex() range." );

    index = setTo;
    }


  inline Int64 getD( const Int32 where ) const
    {
    // RangeC::test( where, 0,
       //      ProjConst::digitArraySize - 1,
       //             "Integer.getD() range." );

    return D[where];
    }

  inline void setD( const Int32 where,
                    const Int64 toSet )
    {
    // I want to be able to use toSet values
    // that might be a full 48 bits long.

    // RangeC::test( where, 0,
    //           ProjConst::digitArraySize - 1,
    //          "Integer.setD() where range." );

    RangeC::test2( toSet, 0, 0xFFFFFFFFFFFFL,
                "Integer.setD() toSet range." );

    D[where] = toSet;
    }

  inline void incrementIndex( void )
    {
    index++;
    if( index >= ProjConst::digitArraySize )
      throw "Integer IncrementIndex() overflow.";

    }

  void setToMaxValue( void );

  inline void setFromInt24( const Int32 toSet )
    {
    RangeC::test2( toSet, 0, 0xFFFFFF,
              "Integer.setFromInt24() size." );

    negative = false;
    D[0] = toSet;
    index = 0;
    }

  inline void setFromLong48( const Int64 toSet )
    {
    RangeC::test2( toSet, 0, 0xFFFFFFFFFFFFL,
        "Integer.setFromLong48() toSet range." );

    negative = false;

    // If toSet was zero then D[0] would be zero
    // and index would be zero.

    // For 24 bits
    D[0] = toSet & 0xFFFFFF;
    D[1] = toSet >> 24;

    if( D[1] == 0 )
      index = 0;
    else
      index = 1;

    }

  void copy( const Integer& copyFrom );
  void copyUpTo( const Integer& copyFrom,
                 const Int32 where );
  bool isEqualToInt24( const Int32 toTest ) const;
  bool isEqualToInt48( const Int64 toTest ) const;
  bool isEqual( const Integer& x ) const;
  bool isLong48( void ) const;
  Int64 getAsLong48( void ) const;
  bool paramIsGreater( const Integer& x ) const;
  bool paramIsGreaterOrEq( const Integer& x )
                                          const;
  void increment( void );
  void addLong48( const Int64 toAdd );
  void add( const Integer& toAdd );
  void square0( void );
  void square1( void );
  void square2( void );
  void shiftLeft( const Int32 shiftBy );
  void shiftRight( const Int32 shiftBy );
  void setDigitAndClear( const Int32 where,
                         const Int64 toSet );
  bool makeRandomOdd( const Int32 setToIndex );

/*
  bool setFromAsciiStr( Str& in );
  Str getAsciiStr( void );

  do charBuf.
  void setFromBigEndianByteArray(
                              Uint8Array& U8Ar );
  void getBigEndianByteArray(
                             Uint8Array& toGet );
*/

  void copyFromIntBuf( const IntBuf& intBuf );
  void copyToIntBuf( IntBuf& intBuf ) const;
  void cleanUp( void );

  };
