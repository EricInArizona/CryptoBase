// Copyright Eric Chauvin, 2022.



#pragma once


#include "../LinuxApi/BasicTypes.h"
#include "../LinuxApi/BoolArray.h"
#include "../LinuxApi/RangeC.h"

#include "../CryptoBase/ProjConst.h"
#include "../CryptoBase/SPrimes.h"



class GoodX
  {
  private:
  Int32 testForCopy = 123;

  static const Int32 last =
                   ProjConst::crtDigitArraySize;

  BoolArray* bArrays;

  public:
  GoodX( void );
  GoodX( const GoodX& in );
  ~GoodX( void );
  void init( const SPrimes& sPrimes );
  void setAllFalse( void );

  inline bool getVal( const Int32 primeAt,
                      const Int32 where ) const
    {
    RangeC::test2( primeAt, 0, last - 1,
            "GoodX.getVal primeAt range." );

    // BoolArray has its own range check.
    // But check it here for testing.
    RangeC::test2( where, 0,
                 bArrays[primeAt].getSize() - 1,
                 "GoodX.getVal where range." );

    return bArrays[primeAt].getVal( where );
    }

  inline void setVal( const Int32 primeAt,
                      const Int32 where,
                      const bool toSet )
    {
    RangeC::test2( primeAt, 0, last - 1,
           "GoodX.setVal primeAt range." );

    // BoolArray has its own range check.
    // But check it here for testing.
    RangeC::test2( where, 0,
                 bArrays[primeAt].getSize() - 1,
                 "GoodX.getVal where range." );

    bArrays[primeAt].setVal( where, toSet );
    }

  inline Int32 getLengthAt( const Int32 where ) const
    {
    RangeC::test2( where, 0, last - 1,
            "getLengthAt where range." );

    return bArrays[where].getSize();
    }

  };
