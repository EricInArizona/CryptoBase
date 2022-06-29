// Copyright Eric Chauvin 2021 - 2022.



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html


#include "Integer128.h"

#include "../CppBase/Casting.h"
#include "../CppBase/CharBuf.h"
#include "Randomish.h"


/*
void Integer128::cleanCarry( void )
{
Integer64 carry;
Integer64 totalL;
Integer64 totalH;

const Int32 max = index;
for( Int32 count = 0; count < max; count++ )
  {
  void add( const Integer64& toAdd );

  Uint32 totalL = (carry & 0xFFFF) +
                  (D[count] & 0xFFFF);

  Uint32 totalH = (carry >> 16) +
                  (D[count] >> 16);

  D[count] = totalL & 0xFFFF;
  carry = totalH + (totalL >> 16);
  }

if( carry != 0 )
  {
  index++;
  if( index >= digitArSize )
    throw  "Integer64 carry overflow.";

  D[index] = carry;
  }
}
*/





/*
void Integer64::add( const Integer64& toAdd )
{
// ====== An IntegerMath for each size?
// Or is that even needed?

// There is a separate IntegerMath.Add() that
// is a wrapper to handle negative numbers too.
// if( negative )
// if( toAdd.negative )

if( toAdd.isULong() )
  {
  addULong( toAdd.getAsULong() );
  return;
  }

const Int32 thisIndex = index;
const Int32 toAddIndex = toAdd.index;
if( thisIndex < toAddIndex )
  {
  for( Int32 count = thisIndex + 1;
                  count <= toAddIndex; count++ )
    D[count] = toAdd.D[count];

  for( Int32 count = 0; count <= thisIndex;
                                      count++ )
    D[count] += toAdd.D[count];

  index = toAddIndex;
  }
else
  {
  for( Int32 count = 0; count <= toAddIndex;
                                         count++ )
    D[count] += toAdd.D[count];

  }

cleanCarry();
}

*/
