// Copyright Eric Chauvin 2021 - 2022.
// This copyright notice has to stay at the top.


// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html



#include "TwoDInt64.h"



TwoDInt64::TwoDInt64( void )
{
aR = new Int64[last];
}


TwoDInt64::TwoDInt64( const TwoDInt64& in )
{
aR = new Int64[last];

// Make the compiler think in is being used.
if( in.testForCopy == 7 )
  return;

throw "Copy constructor for TwoDInt64 called.";
}



TwoDInt64::~TwoDInt64( void )
{
delete[] aR;
}
