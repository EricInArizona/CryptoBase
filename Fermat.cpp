// Copyright Eric Chauvin 2021 - 2022.


// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html


#include "Fermat.h"



void Fermat::makeAPrime( Integer& result,
                         const Int32 setToIndex,
                         const SPrimes& sPrimes,
                         IntegerMath& intMath,
                         Mod& mod )
                         // FileIO& mainIO )
{
// for( Int32 count = 0; count < 100; count++ )
while( true )
  {
  // ThreadEC::sleep( 1 );

  if( !result.makeRandomOdd( setToIndex ))
    {
    throw "Fermat: Error making random number.";
    // return;
    }


  // For testing.
  if( setToIndex == 1 )
    {
    // Make it short
    // This could hang up making primes
    // if it's too small.
    result.setD( setToIndex,
         result.getD( setToIndex ) & 0xFFFF );

    }



  // Make sure that it's the size I think it is.
  if( result.getIndex() < setToIndex )
    throw "Size of random prime is wrong.";

  // This tests all the primes in SPrimes.
  Int32 testPrime = intMath.
                     isDivisibleBySmallPrime(
                                      result,
                                      sPrimes );
  if( 0 != testPrime)
    continue;

  if( !isPrime( result, sPrimes, intMath, mod ))
                                   // mainIO ))
    {
    // mainIO.appendChars(
    //          "Did not pass Fermat test.\n" );
    continue;
    }

  return;
  }
}




bool Fermat::isPrime( const Integer& toTest,
                      const SPrimes& sPrimes,
                      IntegerMath& intMath,
                      Mod& mod )
                      // FileIO& mainIO )
{
// Use bigger primes for Fermat test because the
// modulus can't be too small.  And also it's
// more likely to be congruent to 1 with a very
// small modulus.  In other words it's a lot more
// likely to appear to be a prime when it isn't.
// This Fermat primality test is usually
// described as using random primes to test with,
// and you could do it that way too.
// An adversary will know which primes you use
// for testing this unless they are random.
// A common way of doing this is to use a
// multiple of several primes as the base, like
// 2 * 3 * 5 * 7 = 210.


// Halfway down the array.
Int32 startAt = ProjConst::primesArraySize >> 1;

for( Int32 count = startAt; count <
                   (100 + startAt); count++ )
  {
  if( !isPrimeForOneValue( toTest,
                   sPrimes.getPrimeAt( count ),
                   intMath,
                   mod ))
                   // mainIO ))
    {
    // It is definitely not a prime.
    return false;
    }
  }

// It _might_ be a prime if it passed this test.
// Increasing how many loops it does, how many
// tests it does, increases the probability
// that it's a prime.

return true;
}



bool Fermat::isPrimeForOneValue(
                         const Integer& toTest,
                         const Int64 base,
                         IntegerMath& intMath,
                         Mod& mod )
                          // FileIO& mainIO )
{
// Assume toTest is not a small number.  (Not
// the size of a small prime.)
// Normally it would be something like a 1024
// bit number or bigger, but I assume it's at
// least bigger than a 32 bit number.
// Assume this has already been checked to
// see if it's divisible by a small prime.
// A has to be coprime to P and it is here
// because toTest is not divisible by a small
// prime.
// Fermat's little theorem:
// A ^ (P - 1) is congruent to 1 mod P if P
// is a prime.
// Or: A^P - A is congruent to A mod P.
// If you multiply A by itself P times then
// divide it by P, the remainder is A.
//  (A^P / P)
// 5^3 = 125.  125 - 5 = 120.  A multiple of 5.
// 2^7 = 128.  128 - 2 = 7 * 18 (a multiple of 7.)

Integer pMinus1;
Integer A;

pMinus1.copy( toTest );
intMath.subLong48( pMinus1, 1 );
A.setFromLong48( base );

// Integer testA;
// testA.copy( A );

mod.toPower( A, pMinus1, toTest, intMath );

if( A.isOne())
  return true; // It _might_ be a prime.
else
  return false; // It is definitely a
                // composite number.

}
