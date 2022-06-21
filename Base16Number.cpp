// Copyright Eric Chauvin 2022.



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html



#include "Base16Number.h"


Base16Number::Base16Number( void )
{
D = new Int32[digitArraySize];
index = 0;
D[0] = 0;
}


Base16Number::Base16Number( Str& toSet )
{
D = new Int32[digitArraySize];
setFromStr( toSet );
}


Base16Number::Base16Number(
                        const Base16Number& obj )
{
D = new Int32[digitArraySize];

for( Int32 count = 0; count < digitArraySize;
                                        count++ )
  {
  D[count] = obj.D[count];
  }
}


Base16Number::~Base16Number( void )
{
delete[] D;
}



void Base16Number::setFromStr( const Str& toSet )
{
index = 0;
D[0] = 0;

const Int32 last = toSet.getLast();
if( last < 1 )
  return;

Int32 where = 0;
for( Int32 count = last - 1; count >= 0; count-- )
  {
  if( where >= digitArraySize )
    throw "Base16Number: Too big for array.";

  Int32 c = toSet.getC( count );

  // Ignore white space, commas, non digits.
  if( !isDigit( c ))
    continue;

  Int32 digit = convertDigit( c );

  D[where] = digit;
  where++;
  }

if( where == 0 )
  {
  // No valid digit was ever set.
  throw "Base16Number: no valid digits.";
  }

index = where - 1;
}



void Base16Number::setFromBytes( 
                          const CharBuf& charBuf )
{
index = 0;
D[0] = 0;

const Int32 last = charBuf.getLast();
if( last < 1 )
  return;

Int32 where = 0;
// Start at the right most byte.
// It is setting it to little endian.
for( Int32 count = last - 1; count >= 0; count-- )
  {
  if( where >= digitArraySize )
    throw "Base16Number: Too big for array.";

  Int32 c = charBuf.getC( count );
  

  D[where] = c & 0xF;
  where++;
  D[where] = (c >> 4) & 0xF;
  where++;
  }

if( where == 0 )
  {
  // No valid digit was ever set.
  throw "Base16Number: no valid digits.";
  }

index = where - 1;
}


