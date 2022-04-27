// Copyright Eric Chauvin 2021 - 2022.



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html



#include "../CppBase/Casting.h"

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
