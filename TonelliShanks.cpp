// Copyright Eric Chauvin 2022.



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html


// The Tonelli Shanks algorithm.
#include "TonelliShanks.h"
#include "Euler.h"


/*
TonelliShanks::TonelliShanks( void )
{

}


TonelliShanks::TonelliShanks(
                       const TonelliShanks& in )
{
// Make the compiler think in is being used.
if( in.testForCopy == 7 )
  return;

throw
  "Don't use copy constructor for TonelliShanks.";
}


TonelliShanks::~TonelliShanks( void )
{
}
*/




bool TonelliShanks::modRoot3( const Integer& A,
                        Integer& result,
                        const Integer& prime,
                        IntegerMath& intMath,
                        Mod& mod )
{
// For the special case where the prime
// is congruent to 3 mod 4.

Integer primePlus1Over4;
primePlus1Over4.copy( prime );
primePlus1Over4.increment(); // Add 1.
primePlus1Over4.shiftRight( 2 ); // Divide by 4.

result.copy( A );
mod.toPower( result, primePlus1Over4,
             prime, intMath );

// The second solution is prime - result.
// Negative result squared is the same as
// result squared.

Integer test;
test.copy( result );
mod.square( test, prime, intMath );

if( test.isEqual( A ))
  return true;

return false;
}






bool TonelliShanks::modRoot( const Integer& A,
                        Integer& result,
                        const Integer& prime,
                        IntegerMath& intMath,
                        Mod& mod )
{
// What is the modular square root of A mod
// prime?

if( A.isZero())
  {
  // With an elliptic curve, y can be zero.
  result.setToZero();
  return true;
  }

if( A.isOne())
  {
  result.setToOne();
  return true;
  }

if( (prime.getD( 0 ) & 3) == 3 )
  {
  // If the prime is congruent to 3 mod 4.
  return modRoot3( A, result, prime,
                   intMath, mod );
  }


Integer pMinus1;
pMinus1.copy( prime );
intMath.subLong48( pMinus1, 1 );

Integer pMinus1Over2;
pMinus1Over2.copy( pMinus1 );
pMinus1Over2.shiftRight( 1 ); // Divide by 2.

// Find s, the largest power of two in pMinus1.
Int32 s = 0;

Integer q;
q.copy( pMinus1 );

// Factor out the largest power of 2.
// So it is p - 1 = 2^s * q.
while( (q.getD( 0 ) & 1) == 0 )
  {
  if( q.isZero())
    throw "TonelliShanks.  Q is zero.";

  q.shiftRight( 1 ); // Divide by 2.
  s++;
  }

// s would have to be at least 1 here because
// otherwise it would have been 3 mod 4 above.

// Find the first non residue.
Integer y;
y.setToOne();
while( true )
  {
  if( !Euler::criterion( y, prime, mod, intMath ))
    break;

  y.increment();
  }

// Now I have y which is a non residue.

Integer qPlus1Over2;
qPlus1Over2.copy( q );
qPlus1Over2.increment(); // Plus 1.
qPlus1Over2.shiftRight( 1 ); // Divide by 2.

// This is the first approximation to the answer.
result.copy( A );
mod.toPower( result, qPlus1Over2, prime, intMath );


Integer c;
c.copy( y );
mod.toPower( c, q, prime, intMath );

Integer t;
t.copy( A );
mod.toPower( t, q, prime, intMath );

// It can be 1, if it already has the answer.
if( t.isOne())
  return true;

//  throw "TonelliShanks. Can it be one here?";

Int32 E = s;

Integer tt;
Integer temp;

while( !t.isOne() )
  {
  tt.copy( t );

  // Find the least value of i between 0 and E.
  // Looking for t^(2^i) is congruent to 1 mod
  // prime.
  Int32 i = 0;
  while( !tt.isOne())
    {
    // Powers of 2.
    //  Keep squaring the previous square.
    mod.square( tt, prime, intMath );
    i++;

    // It never found an i value.
    if( i == E )
      // throw "TonelliShanks. Never found i.";
      return false;

    }

  Integer expon;
  expon.setFromInt24( E );
  intMath.subLong48( expon, i );
  intMath.subLong48( expon, 1 );

  // 2^(E - i - 1)
  Integer exponBig;
  exponBig.setFromInt24( 2 );
  mod.toPower( exponBig, expon, pMinus1, intMath );

  Integer b;
  b.copy( c );
  mod.toPower( b, exponBig, prime, intMath );

  // Set new values for the next loop.

  mod.multiply( result, b, prime, intMath );

  Integer bSqr;
  bSqr.copy( b );
  mod.square( bSqr, prime, intMath );
  c.copy( bSqr );

  // The order of t should be getting smaller.
  // That means like how big the group is.
  // Like when x^y = 1.  When it comes back
  // to the identity in the group.
  // And that has to do with finding i above.

  mod.multiply( t, bSqr, prime, intMath );

  E = i;
  }

// And prime - result would be the other
// solution.

Integer test;
test.copy( result );
mod.square( test, prime, intMath );
if( test.isEqual( A ))
  return true;

throw "TonelliShanks at the bottom false.";
// return false;
}
