// Copyright Eric Chauvin 2021 - 2022.



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html


#include "Integer64.h"

#include "../CppBase/Casting.h"
#include "../CppBase/CharBuf.h"
#include "Randomish.h"


void Integer64::setFromULong( const Uint64 toSet )
{
negative = false;

// For 16 bits
D[0] = toSet & 0xFFFF;
D[1] = (toSet >> 16) & 0xFFFF;
D[2] = (toSet >> 32) & 0xFFFF;
D[3] = (toSet >> 48) & 0xFFFF;

index = 0;

for( Int32 count = 1; count < 4; count++ )
  {
  if( D[count] != 0 )
    index = count;

  }
}


void Integer64::cleanCarry( void )
{
Uint32 carry = 0;
const Int32 max = index;
for( Int32 count = 0; count < max; count++ )
  {
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



void Integer64::addULong( const Uint64 toAdd )
{
D[0] += toAdd & 0xFFFF;
D[1] += (toAdd >> 16) & 0xFFFF;
D[2] += (toAdd >> 32) & 0xFFFF;
D[3] += (toAdd >> 48) & 0xFFFF;

cleanCarry();
}



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


void Integer64::copy( const Integer64& from )
{
negative = from.negative;

const Int32 last = from.index;

index = last;
for( Int32 count = 0; count <= last; count++ )
      D[count] = from.D[count];

}


void Integer64::copyLow( const Integer64& from )
{
negative = from.negative;

// digitArSize is always 8.
index = from.index;
if( index > 3 )
  index = 3;

Int32 max = index;
for( Int32 count = 0; count <= max; count++ )
  D[count] = from.D[count];

}


void Integer64::copyHigh( const Integer64& from )
{
negative = from.negative;

// digitArSize is always 8.
// index = from.index;
if( fron.index < 4 )
  {
  setZero();
  return;
  }

index = fron.index - 4;

Int32 max = index;
for( Int32 count = 0; count <= max; count++ )
  D[count] = from.D[count + 4];

}

