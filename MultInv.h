// Copyright Eric Chauvin, 2022.


// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html



#pragma once


#include "../CppBase/BasicTypes.h"
#include "../CppBase/RangeC.h"
#include "../CppBase/Int32Array.h"

#include "ProjConst.h"
#include "SPrimes.h"



class MultInv
  {
  private:
  Int32 testForCopy = 123;
  static const Int32 last =
                   ProjConst::crtDigitArraySize;

  Int32Array* i32Arrays;
  Int32 calcMultInv( const Int32 prime,
                     const Int32 test );

  Int64 calcMultInvL( const Int64 knownX,
                      const Int64 modulus );

  public:
  MultInv( void );
  MultInv( const MultInv& in );
  ~MultInv( void );
  void init( const SPrimes& sPrimes );
  inline Int32 getInv( const Int32 primeAt,
                       const Int32 value ) const
    {
    // RangeC::test( primeAt, 0, last - 1,
       //   "MultInv.getInv primeAt range." );

    // Get the inverse of value.
    return i32Arrays[primeAt].getVal( value );
    }


  };
