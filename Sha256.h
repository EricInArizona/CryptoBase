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


#include "../CppBase/BasicTypes.h"
#include "../CppBase/CharBuf.h"
#include "../CppBase/Uint32Array.h"



class Sha256
  {
  private:
  bool testForCopy = false;
  Uint32Array intermediateHash;
  // W is called the Message Schedule.
  Uint32Array W;

  void init( void );
  void appendPadding( CharBuf& charBuf );

  // The C11 standard says that wrapping
  // unsigned integers is the normal behavior.
  // I think C++ works that way.
  // It's mod 2^32.


  // From the RFC:

  // #define SHA256_ROTR(bits,word)                         \
  //             (((word) >> (bits)) |
  //             ((word) << (32-(bits))))

  static inline Uint32 rotateR( const Uint32 x,
                         const Int32 howMuch )
    {
    // ROTR^n(x) = (x >> n) OR (x <<(w-n))

    return (x >> howMuch) |
           (x << (32 - howMuch));
    }


  // #define SHA256_ROTL(bits,word)                         \
  //        (((word) << (bits)) |
  //      ((word) >> (32-(bits))))

  static inline Uint32 rotateL( const Uint32 x,
                           const Int32 howMuch )
    {
    return (x << howMuch) |
           (x >> (32 - howMuch));
    }

  // "equivalent and potentially faster."
  // ... on some systems.
  // #define SHA_Ch(x, y, z)
  //        (((x) & ((y) ^ (z))) ^ (z))
  // #define SHA_Maj(x, y, z)
  //     (((x) & ((y) | (z))) | ((y) & (z)))


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
    //    (SHA256_ROTR( 2,word) ^
    //    SHA256_ROTR(13,word) ^
    //    SHA256_ROTR(22,word))

    return rotateR( x, 2 ) ^ rotateR( x, 13 ) ^
                             rotateR( x, 22 );
    }

  static inline Uint32 shaBSigma1( const Uint32 x )
    {
    // BSIG1(x) = ROTR^6(x) XOR ROTR^11(x) XOR
    //                  ROTR^25(x)

    // #define SHA256_SIGMA1(word)
    //     (SHA256_ROTR( 6,word) ^
    //     SHA256_ROTR(11,word) ^
    //     SHA256_ROTR(25,word))

    return rotateR( x, 6 ) ^ rotateR( x, 11 ) ^
                             rotateR( x, 25 );
    }

  static inline Uint32 shaSSigma0( const Uint32 x )
    {
    // SSIG0(x) = ROTR^7(x) XOR ROTR^18(x) XOR
    //  SHR^3(x)

    // #define SHA256_sigma0(word)
    //      (SHA256_ROTR( 7,word) ^
    //       SHA256_ROTR(18,word) ^
    //       SHA256_SHR( 3,word))

    return rotateR( x, 7 ) ^ rotateR( x, 18 ) ^
                             (x >> 3);
    }

  static inline Uint32 shaSSigma1( const Uint32 x )
    {
    // SSIG1(x) = ROTR^17(x) XOR ROTR^19(x) XOR
    //  SHR^10(x)

    // #define SHA256_sigma1(word)
    //     (SHA256_ROTR(17,word) ^
    //      SHA256_ROTR(19,word) ^
    //      SHA256_SHR(10,word))

    return rotateR( x, 17 ) ^ rotateR( x, 19 ) ^
                             (x >> 10);
    }



  // #define SHA_Parity(x, y, z)  ((x) ^ (y) ^ (z))


  // For SHA 256.

  static constexpr Uint32 K[64] = {
      0x428a2f98, 0x71374491,
      0xb5c0fbcf, 0xe9b5dba5,
      0x3956c25b, 0x59f111f1,
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
