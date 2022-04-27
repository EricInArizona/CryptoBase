// Copyright Eric Chauvin, 2021 - 2022.


// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html



#pragma once



#include "../CppBase/BasicTypes.h"
#include "../CppBase/Str.h"
#include "../CppBase/FileIO.h"
#include "Integer.h"
#include "SPrimes.h"
#include "TwoDInt64.h"



class IntegerMath
  {
  private:
  Int32 testForCopy = 123;
  // Scratch pad, just like you would do on paper.
  TwoDInt64 M;
  Int64* scratch;


  void setMultiplySign( Integer& result,
                        const Integer& toMul );

  void searchSqrtXPart( const Int32 testIndex,
                        const Integer& fromSqr,
                        Integer& sqrRoot );


  public:
  IntegerMath( void );
  IntegerMath( const IntegerMath& in );
  ~IntegerMath( void  );
  static Int64 find64SqrRoot2(
                          const Int64 toMatch );
  Int32 isDivisibleBySmallPrime(
                        const Integer& toTest,
                        const SPrimes& sPrimes );

  void subLong48( Integer& result,
                  const Int64 toSub );
  void add( Integer& result,
                      const Integer& toAdd );
  void subtract( Integer& result,
                         const Integer& toSub );
  void subtractPositive( Integer& result,
                         const Integer& toSub );

  void multiplyInt( Integer& result,
                    const Int64 toMul );
  Int32 multInt24FromCopy( Integer& result,
                         const Integer& from,
                         const Int64 toMul );
  void multiplyLong48( Integer& result,
                       const Int64 toMul );
  void multiply( Integer& result,
                 const Integer& toMul );

/*
  void setFromStr( Integer& result, Str& in );
*/

  Str toString10( const Integer& from );
  void square( Integer& toSquare );
  Int32 getMod24( const Integer& in,
                  const Int64 divisor );
  Int64 getMod48( const Integer& in,
                  const Int64 divisor );
  static Int64 mod48FromTwoLongs( const Int64 P1,
                           const Int64 P0,
                           const Int64 divisor );

  void multiplyTop( Integer& result,
                    const Integer& toMul );
  void multiplyTopOne( Integer& result,
                       const Integer& toMul );
  bool squareRoot( const Integer& fromSqr,
                   Integer& sqrRoot );
  void setsqrRootTop( const Integer& fromSqr,
                      Integer& sqrRoot );

  };
