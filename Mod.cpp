// Copyright Eric Chauvin 2021 - 2022.



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html

#include "Mod.h"
#include "Division.h"
#include "Euclid.h"



Mod::Mod( void )
{
}


Mod::Mod( const Mod& in )
{
// Make the compiler think in is being used.
if( in.testForCopy == 7 )
  return;

throw "Don't copy Mod in a copy constructor.";
}



void Mod::reduce( Integer& result,
               const Integer& toReduce,
               const Integer& modulus,
               IntegerMath& intMath )
{
numbSys.reduce( result,
                toReduce,
                modulus,
                intMath );

}



void Mod::makeExact( Integer& exact,
                     const Integer& modulus,
                     IntegerMath& intMath )
{
// Most of the time the math is not exact,
// like in the modular exponentiation function
// toPower().  It leaves some small
// multiple of the modulus.  So this gets rid
// of that small left over multiple of the
// modulus.

Integer quotient;
Integer remainder;

Division::divide( exact, modulus,
                  quotient, remainder, intMath );

exact.copy( remainder );
}



// This is the standard modular power algorithm
// that you could find in any standard textbook,
// but its use of the new modular reduction
// algorithm is new (in 2015).

void Mod::toPower( Integer& result,
                   const Integer& exponent,
                   const Integer& modulus,
                   IntegerMath& intMath )
{
if( result.isZero())
  {
  // mainIO.appendChars(
  //            "toPower() is zero at top.\n" );

  return; // With Result still zero.
  }

if( result.isEqual( modulus ))
  {
  // It is congruent to zero % ModN.
  // mainIO.appendChars(
  //       "Congruent to zero in toPower().\n" );
  result.setToZero();
  return;
  }

// Result is not zero at this point.
if( exponent.isZero() )
  {
  // mainIO.appendChars(
  //       "Exponent is zero in toPower().\n" );
  result.setToOne();
  return;
  }

// Integer quotient;
// Integer remainder;

if( modulus.paramIsGreater( result ))
  {
  makeExact( result, modulus, intMath );
  }


if( exponent.isOne())
  {
  // mainIO.appendChars(
  //      "Exponent is one in toPower().\n" );
  // Result stays the same.
  return;
  }

Integer X;
Integer expCopy;
Integer temp;

X.copy( result );
expCopy.copy( exponent );
// Uint32 testIndex = 0;

result.setToOne();

Int32 howBig = 0;

// For each bit.
while( true )
  {
  if( (expCopy.getD( 0 ) & 1) == 1 )
    {
    intMath.multiply( result, X );

    // temp is the result of this
    // reduce() operation.
    reduce( temp, result, modulus, intMath );
    result.copy( temp );
    }

  expCopy.shiftRight( 1 ); // Divide by 2.
  if( expCopy.isZero())
    break;

  // intMath.multiply( X, X );
  intMath.square( X );
  reduce( temp, X, modulus, intMath );
  X.copy( temp );
  }

// When reduce() gets called it multiplies a base
// number by a uint sized digit.  So that can
// make the result one digit bigger than the
// modulus.  Then when they are added up
// you can get carry bits that can make it a
// little bigger.

howBig = result.getIndex() - modulus.getIndex();
// if( howBig > 1 )
  // throw "This does happen.";

if( howBig > 2 )
  throw "This never happens yet. howBig.";

reduce( temp, result, modulus, intMath );
result.copy( temp );

// Notice that this Divide() is done once.
// Not a thousand or two thousand times.
makeExact( result, modulus, intMath );
}



void Mod::verifyInBaseRange(
                     const Integer& toCheck,
                     const Integer& modulus )
{
if( toCheck.getNegative() )
  throw "verifyInBaseRange() is negative.";

if( modulus.paramIsGreaterOrEq( toCheck ))
  throw "verifyInBaseRange() modulus >= toCheck";

}



void Mod::verifyMoreThanZero(
                        const Integer& toCheck )
{
if( toCheck.getNegative() )
  throw "verifyMoreThanZero() is negative.";

if( toCheck.isZero() )
  throw "verifyMoreThanZero() is zero.";

}



void Mod::add( Integer& result,
               const Integer& toAdd,
               const Integer& modulus,
               IntegerMath& intMath )
{
verifyInBaseRange( result, modulus );
verifyInBaseRange( toAdd, modulus );

// Integer temp;

intMath.add( result, toAdd );

// numbSys.reduce( temp, result, modulus, intMath );
// result.copy( temp );
makeExact( result, modulus, intMath );
}


void Mod::addL( Integer& result,
                const Int64 toAdd,
                const Integer& modulus,
                IntegerMath& intMath )
{
if( toAdd == 0 )
  return;


verifyInBaseRange( result, modulus );
// verifyInBaseRange( toAdd, modulus );

// Integer temp;

result.addLong48( toAdd );

// numbSys.reduce( temp, result, modulus, intMath );
// result.copy( temp );
makeExact( result, modulus, intMath );
}



void Mod::negate( Integer& result,
                  const Integer& modulus,
                  IntegerMath& intMath )
{
// If y was 1 and base was 5.
// 1 + (-1) = 0;
// y = -1 + 5 = 4.
// 1 + 4 = 0 mod 5.
// The definition of the word 'negate':
// y + negate( y ) = 0.

verifyInBaseRange( result, modulus );

// Since it was checked to see if it's in
// the base range, it is not negative.

result.setNegative( true );
// Adding a positive number to a negative
// number to make it positive.
intMath.add( result, modulus );
}



void Mod::subtract( Integer& result,
                    const Integer& toSub,
                    const Integer& modulus,
                    IntegerMath& intMath )
{
verifyInBaseRange( result, modulus );
verifyInBaseRange( toSub, modulus );


intMath.subtract( result, toSub );
if( result.getNegative())
  intMath.add( result, modulus );

// This add() can't add a negative number.
//  result.add( modulus );

verifyInBaseRange( result, modulus );

// numbSys.reduce( temp, result, modulus, intMath );
// result.copy( temp );
// makeExact( result, modulus, intMath );
}



void Mod::multiply( Integer& result,
                    const Integer& toMul,
                    const Integer& modulus,
                    IntegerMath& intMath )
{
verifyInBaseRange( result, modulus );
verifyInBaseRange( toMul, modulus );

intMath.multiply( result, toMul );

Integer temp;

reduce( temp, result, modulus, intMath );
result.copy( temp );
makeExact( result, modulus, intMath );
}


void Mod::multiplyL( Integer& result,
                      const Int64 toMul,
                      const Integer& modulus,
                      IntegerMath& intMath )
{
verifyInBaseRange( result, modulus );
// verifyInBaseRange( toMul, modulus );

intMath.multiplyLong48( result, toMul );

Integer temp;
reduce( temp, result, modulus, intMath );
result.copy( temp );
makeExact( result, modulus, intMath );
}




void Mod::square( Integer& result,
                  const Integer& modulus,
                  IntegerMath& intMath )
{
verifyInBaseRange( result, modulus );

intMath.square( result );

Integer temp;

reduce( temp, result, modulus, intMath );
result.copy( temp );
makeExact( result, modulus, intMath );
}



void Mod::divide( Integer& result,
                  const Integer& numerator,
                  const Integer& divisor,
                  const Integer& modulus,
                  IntegerMath& intMath )
{
verifyInBaseRange( divisor, modulus );
verifyInBaseRange( numerator, modulus );
verifyMoreThanZero( divisor );

if( numerator.isZero())
  {
  result.setToZero();
  return;
  }

if( divisor.isOne())
  {
  result.copy( numerator );
  return;
  }

// The definition of multiplicative inverse:
// soAndSo * multInverse == 1.
// You can't have a multiplicative inverse
// of zero.  0 * anything = 0, so it can't
// equal 1.

Integer inverse;

// Get the multiplicative inverse.
if( !Euclid::multInverse( divisor,
                          modulus,
                          inverse,
                          intMath ))
  throw "GCD not 1 in Mod::divide().";

verifyInBaseRange( result, modulus );

result.copy( numerator );
intMath.multiply( result, inverse );

Integer temp;

reduce( temp, result, modulus, intMath );
result.copy( temp );
makeExact( result, modulus, intMath );
}
