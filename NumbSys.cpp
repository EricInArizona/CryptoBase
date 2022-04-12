// Copyright Eric Chauvin 2021 - 2022.


// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html


// A Number System used for Modular Reduction.

// As far as I know I invented this idea for
// doing fast modular reduction.

// An early version of this idea is in old code
// I wrote from 2015 and earlier at:
// https://github.com/Eric7Apps/OldCode/
       //     blob/master/ModularReduction.cs


#include "NumbSys.h"
#include "Division.h"


NumbSys::NumbSys( void )
{
intBufAr = new IntBuf[last];
}


NumbSys::NumbSys( const NumbSys& in )
{
intBufAr = new IntBuf[last];

// Make the compiler think in is being used.
if( in.testForCopy == 7 )
  return;

throw "Don't use copy constructor for NumbSys.";
}


NumbSys::~NumbSys( void )
{
delete[] intBufAr;
}


void NumbSys::setupBaseArray(
                        const Integer& setBase,
                        IntegerMath& intMath )
{
currentBase.copy( setBase );

Integer base2;   // 0x1 00 00 00
base2.setFromLong48( 0x1000000L );

Integer baseValue;
Integer quotient;
Integer remainder;

baseValue.setToOne();

for( Int32 count = 0; count < last; count++ )
  {
  Division::divide( baseValue, currentBase,
                    quotient, remainder,
                    intMath );

  // setOneBaseFromInt( count, remainder );
  remainder.copyToIntBuf( intBufAr[count] );

  // Done at the bottom for the next round of
  // the loop.
  baseValue.copy( remainder );
  intMath.multiply( baseValue, base2 );
  }
}



void NumbSys::reduce( Integer& result,
                      const Integer& toReduce,
                      const Integer& modulus,
                      IntegerMath& intMath )
{
// currentBase would start out being set to
// zero, so it has to be set the first time
// it gets called.
if( !modulus.isEqual( currentBase ))
  setupBaseArray( modulus, intMath );

if( toReduce.paramIsGreater( currentBase ))
  {
  result.copy( toReduce );
  return;
  }

numVal.copy( toReduce );
const Int32 ind = numVal.getIndex();

Integer accumRow;

result.setToZero();
for( Int32 row = 0; row <= ind; row++ )
  {
  Int64 val = numVal.getD( row );
  const Int32 lastBase = intBufAr[row].
                           getIndex();

  accumRow.setIndex( lastBase );
  for( Int32 column = 0; column <= lastBase;
                                       column++ )
    {
    Int64 baseVal = intBufAr[row].getD( column );
    Int64 calc = baseVal * val;
    accumRow.setD( column, calc );
    }

  accumRow.cleanUp();
  result.add( accumRow );
  }
}
