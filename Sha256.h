// Copyright Eric Chauvin 2022


// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html



// Make it work first, then make it work fast.



// RFC 6234 -
// US Secure Hash Algorithms (SHA and
// SHA-based HMAC and HKDF)

// FIPS 180-2

// Wikipedia article with a lot of links.
// https://en.wikipedia.org/wiki/SHA-2

// Test Vectors:
// For "abc"
// SHA-256
// ba7816bf 8f01cfea 414140de 5dae2223 b00361a3
   //             96177a9c b410ff61 f20015ad

// For the empty string.
// Length 0.
// If you test with an empty string then you are
// appending a 1 bit, and a length of zero.
// That is the 64 bytes.

// SHA-256
// e3b0c442 98fc1c14 9afbf4c8 996fb924
// 27ae41e4 649b934c a495991b 7852b855

// Input message:
// "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnl
//      mnomnopnopq"
// (length 448 bits).

// SHA-256
// 248d6a61 d20638b8 e5c02693 0c3e6039
// a33ce459 64ff2167 f6ecedd4 19db06c1

// A linux test:
// echo -n "abc" | sha256sum
// ba7816bf...



#include "../CppBase/BasicTypes.h"
#include "../CppBase/CharBuf.h"
#include "../CppBase/Uint32Array.h"


// SHA 256 works on 32 bit words.
// SHA 512 works on 64 bit words.



class Sha256
  {
  private:
  bool testForCopy = false;
  Uint32Array intermediateHash;
  // W is called the Message Schedule.
  Uint32Array W;

  // Some Intel processors have SHA instructions.
  // https://www.intel.com/content/www/us/en/
  //           developer/articles/technical/
  //           intel-sha-extensions.html
  // They are SIMD extensions.

  // __asm__


  void init( void );
  void appendPadding( CharBuf& charBuf );

  /* A test for adding.
  static inline Uint32 shaAdd( const Uint32 x,
                               const Uint32 y )
    {
    The C11 standard says that wrapping mod
    2^32 is the normal behavior.
    Uint32 test = x + y;
    // The add instruction would set a carry flag,
    // but have no overflow exception.
    // So you don't know if there is an overflow
    // unless you check for it.
    // See Signals.cpp
    // For signed integers you get
    // Erroneous arithmetic operation.
    // SIGFPE

    Uint64 result = x + y;
    result = result & 0xFFFFFFFF;
    // It's mod 2^32.
    if( result != test )
      throw "This exception never happens.";

    // Casting it to 32 bit.
    return result & 0xFFFFFFFF;
    }
  */


  // &gt; tag is: >

  static inline Uint32 rotateR( const Uint32 x,
                         const Int32 howMuch )
    {
    // Like rotating around in a circle.
    // ROTR^n(x) = (x >> n) OR (x <<(w-n))

    return (x >> howMuch) |
           (x << (32 - howMuch));
    }


  static inline Uint32 rotateL( const Uint32 x,
                           const Int32 howMuch )
    {
    return (x << howMuch) |
           (x >> (32 - howMuch));
    }


  static inline Uint32 shaCh( const Uint32 x,
                              const Uint32 y,
                              const Uint32 z)
    {
    // CH( x, y, z) = (x AND y) XOR ( (NOT x) AND z)
    return (x & y) ^ ((!x) & z);
    }

  static inline Uint32 shaMaj( const Uint32 x,
                               const Uint32 y,
                               const Uint32 z )
    {
    // MAJ( x, y, z) = (x AND y) XOR (x AND z)
    //            XOR (y AND z)
    return (x & y) ^ (x & z) ^ (y & z);
    }

  // These sigma functions are different for
  // SHA 512.

  // Notice how they did upper and lower case
  // names for the sigma #define statements.
  // That comes from the greek sigma symbol,
  // and there is a capital greek sigma and a
  // lower case sigma.
  // Upper case is B, lower case is S.
  // B for Big, S for Small.

  static inline Uint32 shaBSigma0( const Uint32 x )
    {
    // BSIG0(x) = ROTR^2(x) XOR ROTR^13(x)
    //               XOR ROTR^22(x)

    // #define SHA256_SIGMA0(word)

    return rotateR( x, 2 ) ^ rotateR( x, 13 ) ^
                             rotateR( x, 22 );
    }

  static inline Uint32 shaBSigma1( const Uint32 x )
    {
    // BSIG1(x) = ROTR^6(x) XOR ROTR^11(x) XOR
    //                  ROTR^25(x)

    // #define SHA256_SIGMA1(word)

    return rotateR( x, 6 ) ^ rotateR( x, 11 ) ^
                             rotateR( x, 25 );
    }

  static inline Uint32 shaSSigma0( const Uint32 x )
    {
    // SSIG0(x) = ROTR^7(x) XOR ROTR^18(x) XOR
    //  SHR^3(x)

    // #define SHA256_sigma0(word)

    return rotateR( x, 7 ) ^ rotateR( x, 18 ) ^
                             (x >> 3);
    }

  static inline Uint32 shaSSigma1( const Uint32 x )
    {
    // SSIG1(x) = ROTR^17(x) XOR ROTR^19(x) XOR
    //  SHR^10(x)

    // #define SHA256_sigma1(word)

    return rotateR( x, 17 ) ^ rotateR( x, 19 ) ^
                             (x >> 10);
    }



  // #define SHA_Parity(x, y, z)  ((x) ^ (y) ^ (z))


  // For SHA 256.

  static constexpr Uint32 K[64] = {
      0x428a2f98, 0x71374491,
      0xb5c0fbcf, 0xe9b5dba5,
      0x3956c25b,0x59f111f1,
      0x923f82a4, 0xab1c5ed5,
      0xd807aa98, 0x12835b01,
      0x243185be, 0x550c7dc3,
      0x72be5d74, 0x80deb1fe,
      0x9bdc06a7, 0xc19bf174,
      0xe49b69c1, 0xefbe4786,
      0x0fc19dc6, 0x240ca1cc,
      0x2de92c6f, 0x4a7484aa,
      0x5cb0a9dc, 0x76f988da,
      0x983e5152, 0xa831c66d,
      0xb00327c8, 0xbf597fc7,
      0xc6e00bf3, 0xd5a79147,
      0x06ca6351, 0x14292967,
      0x27b70a85, 0x2e1b2138,
      0x4d2c6dfc, 0x53380d13,
      0x650a7354, 0x766a0abb,
      0x81c2c92e, 0x92722c85,
      0xa2bfe8a1, 0xa81a664b,
      0xc24b8b70, 0xc76c51a3,
      0xd192e819, 0xd6990624,
      0xf40e3585, 0x106aa070,
      0x19a4c116, 0x1e376c08,
      0x2748774c, 0x34b0bcb5,
      0x391c0cb3, 0x4ed8aa4a,
      0x5b9cca4f, 0x682e6ff3,
      0x748f82ee, 0x78a5636f,
      0x84c87814, 0x8cc70208,
      0x90befffa, 0xa4506ceb,
      0xbef9a3f7, 0xc67178f2 };


  public:
  inline Sha256( void )
    {
    }

  inline Sha256( const Sha256& in )
    {
    if( in.testForCopy )
      return;

    throw "Sha256 copy constructor.";
    }

  inline ~Sha256( void )
    {
    }

  bool processMessageBlock(
                      CharBuf& charBuf,
                      const Int32 where );

  bool processAllBlocks( CharBuf& charBuf );
  void getHash( CharBuf& charBuf );
  void showHash( void );

  };
