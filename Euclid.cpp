// Copyright Eric Chauvin 2021 - 2022.



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html


#include "Euclid.h"
#include "Division.h"

// This hasn't been tested yet.
Int64 Euclid::GcdL1( const Int64 x,
                     const Int64 y )
{

// This is the basic Euclidean Algorithm
// for GCD.

if( x == 0 )
  throw "GcdL1 with x that is zero.";

if( y == 0 )
  throw "GcdL1 with y that is zero.";

if( x == y  )
  return x;

Int64 gcdX = x;
Int64 gcdY = y;

// Don't change the original numbers that came
// in as parameters.
if( x < y )
  {
  gcdX = y;
  gcdY = x;
  }

while( true )
  {
  // This one doesn't use the quotient, but the
  // extended algorithm does.
  // Int64 quotient = gcdX / gcdY;

  Int64 remainder = gcdX % gcdY;
  if( remainder == 0 )
    return gcdY; // It's the smaller one.

  gcdX = gcdY;
  gcdY = remainder;
  }
}




void Euclid::greatestComDiv( const Integer& X,
                             const Integer& Y,
                             Integer& gcd,
                             IntegerMath& intMath )
{
// This is the basic Euclidean Algorithm
// for GCD.

if( X.isZero())
  throw "Doing GCD with a param that is zero.";

if( Y.isZero())
  throw "Doing GCD with a param that is zero.";

if( X.isEqual( Y ))
  {
  gcd.copy( X );
  return;
  }

Integer gcdX;
Integer gcdY;


// Don't change the original numbers that came
// in as parameters.
if( X.paramIsGreater( Y ))
  {
  gcdX.copy( Y );
  gcdY.copy( X );
  }
else
  {
  gcdX.copy( X );
  gcdY.copy( Y );
  }

Integer quotient;
Integer remainder;

while( true )
  {
  Division::divide( gcdX, gcdY, quotient,
                    remainder, intMath );
  if( remainder.isZero())
    {
    gcd.copy( gcdY ); // It's the smaller one.
    // It can't return from this loop until
    // the remainder is zero.
    return;
    }

  gcdX.copy( gcdY );
  gcdY.copy( remainder );
  }
}



bool Euclid::multInverse( const Integer& knownX,
                          const Integer& modulus,
                          Integer& inverse,
                          IntegerMath& intMath )
{
// Given knownX find inverse with this modulus.

// This is the standard extended Euclidean
// Algorithm like you'd find in any text book.

if( knownX.isOne())
  {
  // 1 * 1 = 1 mod anything.
  inverse.setToOne();
  return true;
  }

if( knownX.isZero())
  throw "MultInverse knownX is zero.\n";

if( modulus.isZero())
  throw "MultInverse with modulus that is zero.";

// if( modulus.paramIsGreaterOrEq( knownX ))

Integer U0;
Integer U1;
Integer U2;

Integer V0;
Integer V1;
Integer V2;

Integer T0;
Integer T1;
Integer T2;

// Don't change the original numbers that were
// passed in as references.

// U is the old part to keep.
U0.setToZero();
U1.setToOne();
U2.copy( modulus );

// V is the new part.
V0.setToOne();
V1.setToZero();
V2.copy( knownX );

T0.setToZero();
T1.setToZero();
T2.setToZero();

Integer quotient;
Integer remainder;

Integer temp1;
Integer temp2;

Integer testForModInverse1;
Integer testForModInverse2;

quotient.setToZero();
// while( not forever )
for( Int32 count = 0; count < 10000; count++ )
  {
  if( U2.getNegative() )
    throw "The U2 modulus was negative.\n";

  if( V2.getNegative() )
    throw "V2 was negative.\n";

  // Something divided by 1 is x / 1 = y
  // x * 1/1 = y
  Division::divide( U2, V2, quotient,
                    remainder, intMath );

  // There is no multiplicative inverse if
  // GCD is more than 1.
  // Also see Pollard's P - 1 algorithm.
  if( remainder.isZero())
    return false;

  temp1.copy( U0 );
  temp2.copy( V0 );
  intMath.multiply( temp2, quotient );
  intMath.subtract( temp1, temp2 );
  T0.copy( temp1 );
  temp1.copy( U1 );
  temp2.copy( V1 );
  intMath.multiply( temp2, quotient );
  intMath.subtract( temp1, temp2 );
  T1.copy( temp1 );
  temp1.copy( U2 );
  temp2.copy( V2 );
  intMath.multiply( temp2, quotient );
  intMath.subtract( temp1, temp2 );
  T2.copy( temp1 );
  U0.copy( V0 );
  U1.copy( V1 );
  U2.copy( V2 );
  V0.copy( T0 );
  V1.copy( T1 );
  V2.copy( T2 );
  if( remainder.isOne())
    break;

  }

inverse.copy( T0 );
if( inverse.getNegative() )
  intMath.add( inverse, modulus );

testForModInverse1.copy( inverse );
testForModInverse2.copy( knownX );
intMath.multiply( testForModInverse1,
                            testForModInverse2 );
Division::divide( testForModInverse1,
         modulus, quotient, remainder, intMath );

// By the definition of Multiplicative inverse:
if( !remainder.isOne())
  throw "Euclid multInverse() is bad.\n";

return true;
}




/*
Not used:

    // This method is for: KnownNumber * ToFind = 1
    // mod Modulus
    // An example:
    // PublicKeyExponent * X = 1 mod PhiN.
    // PublicKeyExponent * X = 1 mod (P - 1)(Q - 1).
    // This means that
    // (PublicKeyExponent * X) = (Y * PhiN) + 1
    // X is less than PhiN.
    // So Y is less than PublicKExponent.
    // Y can't be zero.
    // If this equation can be solved then it
    // can be solved modulo
    // any number.  So it has to be solvable
    //   mod PublicKExponent.
*/





// The definition of the problem:
// Mult inverse is A * x = 1 + Base * y

// The extended Euclidean algorithm gives the
// multiplicative inverse.

// A*X + B*Y = GCD
// But GCD has to be 1 or there can't
// be a multiplicative inverse.

// Either A or B has to be negative here.
// A*X + B*Y = 1 If there's a multiplicative
// inverse.

// If B is negative then it is:
// A*X + -B*Y = 1
// A*X = 1 + B*Y
// A is the multiplicative inverse of X mod B.
// A is also the multiplicative inverse of X
// mod Y.


Int64 Euclid::multInvL( const Int64 knownX,
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
  throw "multInvL knownX is zero.";

if( modulus == 0 )
  throw "multInvL with modulus that is zero.";

if( modulus <= knownX )
  throw "multInvL with modulus <= knownX.";

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
    throw "The initialModulus was negative.";

  if( initialKnownX < 0 )
    throw "initialKnownX was negative.";

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
  throw "Euclid multInvL() is bad.\n";

return inverse; // true;
}
