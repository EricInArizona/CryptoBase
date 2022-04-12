// Copyright Eric Chauvin 2022.



#include "MakePrime.h"
#include "Fermat.h"


MakePrime::MakePrime( void )
{
// new
}



MakePrime::MakePrime( const MakePrime& in )
{

// Make the compiler think in is being used.
if( in.testForCopy == 7 )
  return;

throw "Copy constructor: MakePrime.";
}


MakePrime::~MakePrime( void )
{
// delete[]
}


void MakePrime::makeIt( Integer& result,
                        const Int32 indexSize,
                        const SPrimes& sPrimes,
                        IntegerMath& intMath,
                        Mod& mod )
{
while( true )
  {
  // This checks if it's divisible by any of
  // the small primes in SPrimes and then it
  // does a Fermat primality test.
  Fermat::makeAPrime( result,
                      indexSize,
                      sPrimes,
                      intMath,
                      mod );

  // Make sure that the prime minus 1 doesn't
  // have all small factors.
  // Do Pollard's algorithm to test this?

  // primeMinus1.copy( result );
  //   intMath.subLong48( primePMinus1, 1 );

  // Temporarily giving it a meaninless test
  // until I have more tests.
  if( (result.getD( 0 ) & 1) == 1 )
    break;
  else
    continue;

  // if( some other test )
    // break;
  // else
    // continue;

  }
}
