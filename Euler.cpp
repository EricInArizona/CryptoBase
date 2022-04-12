// Copyright Eric Chauvin 2022.


// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html



#include "Euler.h"


Euler::Euler( void )
{

}


Euler::Euler( const Euler& in )
{
// Make the compiler think in is being used.
if( in.testForCopy == 7 )
  return;

throw "Don't use copy constructor for Euler.";
}


Euler::~Euler( void )
{

}


// Euler's Criterion for whether a number is a
// quadratic residue mod the prime.
bool Euler::criterion( Integer& result,
                    const Integer& toTest,
                    const Integer& prime,
                    const Integer& pMinus1Over2,
                    Mod& mod,
                    IntegerMath& intMath )
{
// Fermat's little theorem
// a^(p-1) mod p = 1
// ((a^((p-1) / 2))^2 mod p = 1.
// y^2 mod p = 1 has exactly two solutions.
// 1 and -1.
// x^2 mod p = A  Is A a quardratic residue?

result.copy( toTest );
mod.toPower( result, pMinus1Over2,
             prime, intMath );

if( result.isOne())
  return true;

// It has to be either 1 or -1.
if( !result.isNegativeOne())
  throw "Euler's criterion is not right with -1.";

return false;
}
