// Copyright Eric Chauvin 2021 - 2022.



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html



#pragma once



#include "../CppBase/BasicTypes.h"
#include "../CryptoBase/Integer.h"
#include "../CryptoBase/IntegerMath.h"
#include "../CryptoBase/Mod.h"



class Fermat
  {
  private:

  public:

  static void makeAPrime( Integer& result,
                   const Int32 setToIndex,
                   const SPrimes& sPrimes,
                   IntegerMath& intMath,
                   Mod& mod );
                   // FileIO& mainIO );

  static bool isPrime( const Integer& toTest,
                const SPrimes& sPrimes,
                IntegerMath& intMath,
                Mod& mod );
                // FileIO& mainIO );

  static bool isPrimeForOneValue(
                      const Integer& toTest,
                      const Int64 base,
                      IntegerMath& intMath,
                      Mod& mod );
                      // FileIO& mainIO );


  };
