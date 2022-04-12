// Copyright Eric Chauvin 2021 - 2022.


// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html


#pragma once



#include "../LinuxApi/BasicTypes.h"
#include "Integer.h"
#include "IntegerMath.h"


// Mult inverse is A * x = 1 + Base * y



class Euclid
  {
  private:

  public:
  static Int64 GcdL1( const Int64 x,
                      const Int64 y );

  static void greatestComDiv(
                       const Integer& X,
                       const Integer& Y,
                       Integer& gcd,
                       IntegerMath& intMath );

  static Int64 multInvL( const Int64 knownX,
                         const Int64 modulus );

  static bool multInverse(
                       const Integer& knownX,
                       const Integer& modulus,
                       Integer& inverse,
                       IntegerMath& intMath );

  };
