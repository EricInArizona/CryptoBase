// Copyright Eric Chauvin 2022


// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html



// Make it work first, then make it work fast.


// This code was adapted and made modern,
// and converted to C++, from:
// RFC 6234 -
// US Secure Hash Algorithms (SHA and
// SHA-based HMAC and HKDF


// IETF Licensing and copyright stuff.
#include "../CryptoBase/ShaLicense.txt"


// The linux (Ubuntu) program sha256sum can
// be used to compare and test what this does.


#include "../CppBase/BasicTypes.h"
#include "../CppBase/CharArray.h"


// SHA 256 works on 32 bit words.
// SHA 512 works on 64 bit words.


class Sha256
  {
  private:
  inline Uint32 add( const Uint32 x,
                     const Uint32 y )
    {
    Uint64 result = x + y;
    // It's mod 2^32.
    result = result & 0xFFFFFFFF;
    return Casting::u64ToU32( result );
    }


  inline Uint32 rotateR( const Uint32 x, 
                         const Uint32 howMuch )
    {
    // Like rotating around in a circle.
    Uint64 result = (x >> howMuch) |
                    (x << (32 - howMuch));

    return Casting::u64ToU32( result );
    }


  inline Uint32 rotateL( const Uint32 x, 
                         const Uint32 howMuch )
    {
    Uint64 result = (x << howMuch) |
                    (x >> (32 - howMuch));

    return Casting::u64ToU32( result );
    }

  public:


  };
