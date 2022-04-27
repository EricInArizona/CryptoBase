// Copyright Eric Chauvin 2022.


// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html


#include "../CppBase/Casting.h"

#include "MultInv.h"

// #include "Euclid.h"



MultInv::MultInv( void )
{
i32Arrays = new Int32Array[Casting::i32ToU64(
                                      last )];
}


MultInv::MultInv( const MultInv& in )
{
i32Arrays = new Int32Array[Casting::i32ToU64(
                                      last )];

// Make the compiler think in is being used.
if( in.testForCopy == 7 )
  return;

throw "Don't use copy constructor for MultInv.";
}


MultInv::~MultInv( void )
{
delete[] i32Arrays;
}



Int32 MultInv::calcMultInv( const Int32 prime,
                            const Int32 test )
{

return Casting::i64ToI32( calcMultInvL( test, prime ));

/*

Int32 testInv = CastE::i64ToI32(
             Euclid::multInvL( test, prime ));

for( Int32 count = 1; count < prime; count++ )
  {
  Int32 check = test * count;
  check = check % prime;
  if( check == 1 )
    {
    if( testInv != count )
      throw
        "MultInv.calcMultInv testInv != count";

    return count;
    }
  }

if( testInv != 0 )
  throw "MultInv.calcMultInv testInv != 0";

// Meaning it's false.  No inverse.
return 0;
*/
}



void MultInv::init( const SPrimes& sPrimes )
{
for( Int32 count = 0; count < last; count++ )
  {
  Int32 prime = sPrimes.getPrimeAt( count );
  i32Arrays[count].setSize( prime );
  i32Arrays[count].setVal( 0, 0 );

  for( Int32 countP = 1; countP < prime;
                                    countP++ )
    {
    Int32 inv = calcMultInv( prime, countP );
    i32Arrays[count].setVal( countP, inv );
    }
  }
}




Int64 MultInv::calcMultInvL( const Int64 knownX,
                             const Int64 modulus )
{
// Given knownX find inverse with this modulus.

// This is the standard extended Euclidean
// Algorithm like you'd find in any text book.

if( knownX == 1 )
  {
  // 1 * 1 = 1 mod anything.
  return 1; // true
  }

if( knownX == 0 )
  throw "getInv knownX is zero.";

if( modulus == 0 )
  throw "getInv with modulus that is zero.";

if( modulus <= knownX )
  throw "getInv with modulus <= knownX.";

Int64 bottomInverse = 0;
Int64 starting1 = 1;
Int64 starting0 = 0;
Int64 initialModulus = modulus;
Int64 startingInverse = 1;
Int64 initialKnownX = knownX;
Int64 inverse = 0;

while( true )
  {
  if( initialModulus < 0 )
    throw "getInv The initialModulus was negative.";

  if( initialKnownX < 0 )
    throw "getInv initialKnownX was negative.";

  Int64 quotient = initialModulus /
                               initialKnownX;
  Int64 remainder = initialModulus %
                               initialKnownX;

  // There is no multiplicative inverse if
  // GCD is more than 1.
  // Also see Pollard's P - 1 algorithm.
  if( remainder == 0 )
    return 0; // false;

  inverse = bottomInverse -
                  (startingInverse * quotient);

  Int64 holdinitialKnownX = initialModulus -
                    (initialKnownX * quotient);
  initialModulus = initialKnownX;
  initialKnownX = holdinitialKnownX;

  bottomInverse = startingInverse;
  startingInverse = inverse;

  Int64 holdStarting1 = starting1 -
                        (starting0 * quotient);
  starting1 = starting0;
  starting0 = holdStarting1;

  if( remainder == 1 )
    break;

  }

if( inverse < 0 )
  inverse += modulus;

Int64 test1 = inverse * knownX;
Int64 testMod = test1 % modulus;

// By the definition of Multiplicative inverse:
if( !(testMod == 1) )
  throw "getInv() is bad.\n";

return inverse;
}
