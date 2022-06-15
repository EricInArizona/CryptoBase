// Copyright Eric Chauvin 2022


// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html



#include "Randomish.h"
#include "../CppBase/Casting.h"


// Get this source code.
#include <random>



// RFC 4086 Randomness Requirements for Security


void Randomish::makeRandomBytes(
                            Uint16Array& uArray,
                            const Int32 howMany )
{
// This is pretty crude at this point.

// The hardware source of entropy.
std::random_device rd;

// rd.entropy()
// rd.min()
// rd.max()

// std::mersenne_twister_engine
// std::mt19937_64
// Mersenne twister engine
// Mersenne as in Mersenne prime.
// 19937 is the exponent for a Mersenne prime.


std::mt19937 gen( rd() );

Int32 where = 0;
for( Int32 count = 0; count < 100000; count++ )
  {
  // dist( gen())

  // Truncate the unsigned int to an 8 bit byte.
  Uint16 c = Casting::
                U64ToU16ByteTruncate( gen());

  // Use non zero bytes.
  if( c == 0 )
    continue;

  uArray.setVal( where, c );
  where++;
  if( where >= howMany )
    return;

  }
}
