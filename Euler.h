// Copyright Eric Chauvin, 2022.



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html



#pragma once



#include "../CppBase/BasicTypes.h"

#include "Integer.h"
#include "IntegerMath.h"
#include "Mod.h"


class Euler
  {
  private:
  Int32 testForCopy = 123;

  public:
  Euler( void );
  Euler( const Euler& in );
  ~Euler( void );
  static bool criterion(
                    const Integer& toTest,
                    const Integer& prime,
                    // const Integer& pMinus1Over2,
                    Mod& mod,
                    IntegerMath& intMath );


  };


/*

Euler's Criterion

The proof is in Fermat's little theorem.

Not counting zero, there are p - 1 / 2 pairs
of numbers that are quadratic residues.
Half of them.  The ones not zero.

Throw an exception if it's not 1 or -1.

Euler's criterion is related to the Law of quadratic reciprocity.


*/
