// Copyright Eric Chauvin 2021 - 2022.



#include "../LinuxApi/Casting.h"

#include "IntBuf.h"


IntBuf::IntBuf( void )
{
D = new Int32[Casting::i32ToU64( last )];
}


IntBuf::IntBuf( const IntBuf& in )
{
D = new Int32[Casting::i32ToU64( last )];

// Make the compiler think in is being used.
if( in.testForCopy == 7 )
  return;

throw "Don't copy IntBuf in a copy constructor.";
}


IntBuf::~IntBuf( void )
{
delete[] D;
}
