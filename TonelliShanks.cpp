// Copyright Eric Chauvin 2022.


// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html


// The Tonelli Shanks algorithm.
#include "TonelliShanks.h"


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



// A good tutorial:
// https://exploringnumbertheory.wordpress.com/
//         2015/12/09/solving-quadratic-
//         congruences-with-odd-prime-moduli/


bool TonelliShanks::modRoot( const Integer& N,
                        Integer& result,
                        const Integer& prime,
                        IntegerMath& intMath,
                        Mod& mod )
{
// What is the modular square root of N?

// See Fermat.cpp for Fermat's little theorem.
// See Euler.cpp for Euler's Criterion.

// Find a modular square root.

if( (prime.getD( 0 ) & 3) == 3 )
  {
  // If the prime is congruent to 3 mod 4 then
  // there is an easier solution.
  Integer primePlus1Over4;
  primePlus1Over4.copy( prime );
  primePlus1Over4.increment(); // Add 1.
  primePlus1Over4.shiftRight( 2 ); // Divide by 4.
  result.copy( N );
  mod.toPower( result, primePlus1Over4,
               prime, intMath );

  Integer test;
  Integer test2;
  test.copy( result );
  intMath.square( test );
  mod.reduce( test2, test, prime, intMath );
  if( test2.isEqual( N ))
    return true;

  return false;
  }


Integer pMinus1;
pMinus1.copy( prime );
intMath.subLong48( pMinus1, 1 );

Integer pMinus1Over2;
pMinus1Over2.copy( pMinus1 );
pMinus1Over2.shiftRight( 1 ); // Divide by 2.

Int32 s = 0;
Integer q;
q.copy( pMinus1 );

while( (q.getD( 0 ) & 1) == 0 )
  {
  q.shiftRight( 1 ); // Divide by 2.
  s++;
  }


/*
if( s == 1 )
  {
  long PDiv2Zeros = (p+1) / 4;
  long r = pow_mod( n, PDiv2Zeros, p );
  if( ((r * r) % p) == n )
    return r;

  return 0;  // There is no sqr root.
  }

// Find the first quadratic non-residue z by
// brute-force search
long z = 1;
long pMinus1 = p - 1;
long pMinus1Over2 = (p - 1) / 2;
while( pow_mod( ++z, pMinus1Over2, p ) != pMinus1 );

long c = pow_mod( z, q, p );
long r = pow_mod( n, (q+1)/2, p );
long t = pow_mod( n, q, p );
long m = s;
while( t != 1 )
  {
  long tt = t;
  long i = 0;
  while( tt != 1 )
    {
    tt = (tt * tt) % p;
    ++i;
    if( i == m )
      return 0;

    }

  long b = pow_mod( c, pow_mod( 2, m-i-1, p-1), p);
  long b2 = (b * b) % p;
  r = (r * b) % p;
  t = (t * b2) % p;
  c = b2;
  m = i;
  }

if ((r * r) % p == n)
  return r;

return 0;
*/

return false;
}


