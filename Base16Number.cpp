// Copyright Eric Chauvin 2022.



#include "Base16Number.h"



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
    throw "Base10Number: Too big for array.";

  Int32 c = toSet.getC( count );

  // Ignore white space, commas, non digits.
  // if ...

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
