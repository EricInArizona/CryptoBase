// Copyright Eric Chauvin 2022


// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html


#include "Sha256.h"
#include "../CppBase/StIO.h"
#include "../CppBase/ByteHex.h"
// #include "../CryptoBase/Base16Number.h"



  // Some Intel processors have SHA instructions.
  // https://www.intel.com/content/www/us/en/
  //           developer/articles/technical/
  //           intel-sha-extensions.html
  // They are SIMD extensions.

  // __asm__



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





void Sha256::appendPadding( CharBuf& charBuf )
{
const Uint64 originalLength =
             Casting::i32ToU64( charBuf.getLast());

StIO::printF( "originalLength: " );
StIO::printFD( Casting::u64ToI32(
              originalLength ));
StIO::putS( "" );


// To restore it to the original length:
// CharBuf.truncateLast( Int32 setTo )

// Append that 1 bit.
charBuf.appendU8( 128, 1024 );
// char test = charBuf.getC( 0 );
// if( (test & 0xFF) == 0x80 )
  // throw "Got that bit.";

Int32 howBig = charBuf.getLast() % 64;

StIO::printF( "howBig: " );
StIO::printFD( howBig );
StIO::putS( "" );


Int32 toAdd = 64 - howBig;

// You have to add the least number of zeros
// possible in order to make it zero mod
// 64 bytes.
// It already has seven zero bits after that
// 1 bit.

// if( toAdd == 64 )
  // toAdd = 0;

StIO::printF( "toAdd at top: " );
StIO::printFD( toAdd );
StIO::putS( "" );

// For SHA 512 it's a 128 bit length value.
// So 16 bytes.

// 8 bytes for length.
toAdd -= 8;
if( toAdd < 0 )
  toAdd += 64;

StIO::printF( "toAdd: " );
StIO::printFD( toAdd );
StIO::putS( "" );

// The spec says this has to be the smallest
// number of zeros to make it come to 64 bytes.
// 512 bits.

for( Int32 count = 0; count < toAdd; count++ )
  charBuf.appendU8( 0, 1024 );

// It already has seven zero bits after that
// 1 bit.  Those aren't part of the message
// length.

// A message of zero length makes this zero.
// "The appended integer is the length of the
// original message."
// "abc" is length 3 times 8.  Bits.

// The hash is done on the padding too.

Uint64 lengthInBits = originalLength * 8;
charBuf.appendU64( lengthInBits,
                   1024 );


Int32 finalSize = charBuf.getLast();

StIO::printF( "finalSize: " );
StIO::printFD( finalSize );
StIO::putS( "" );

StIO::printF( "finalSize mod: " );
StIO::printFD( finalSize % 64 );
StIO::putS( "" );

if( (finalSize % 64) != 0 )
  throw "SHA padding finalSize is not right.";

/*
StIO::putS( "Bytes:" );

for( Int32 count = 0; count < 64; count++ )
  {
  char test = charBuf.getC( count );
  Int32 show = Casting::u64ToI32( Casting::
                          charToU32( test ));

  StIO::printFD( count );
  StIO::printF( ") " );
  StIO::printFD( show );
  StIO::putS( "" );
  }
*/
}



void Sha256::init( void )
{
W.setSize( 64 );

intermediateHash.setSize( 8 );

// This is how they get set before the first
// block, but after the first block they are
// using the intermediate values from the
// previous block.  A chain of blocks.
// A block chain.

// Initial Hash Values: FIPS 180-3
// static uint32_t SHA256_H0[SHA256HashSize/4] =
//  {
//  0x6A09E667, 0xBB67AE85, 0x3C6EF372, 0xA54FF53A,
//  0x510E527F, 0x9B05688C, 0x1F83D9AB, 0x5BE0CD19
// };

intermediateHash.setVal( 0, 0x6A09E667 );
intermediateHash.setVal( 1, 0xBB67AE85 );
intermediateHash.setVal( 2, 0x3C6EF372 );
intermediateHash.setVal( 3, 0xA54FF53A );
intermediateHash.setVal( 4, 0x510E527F );
intermediateHash.setVal( 5, 0x9B05688C );
intermediateHash.setVal( 6, 0x1F83D9AB );
intermediateHash.setVal( 7, 0x5BE0CD19 );

StIO::putS( "Hash at start:" );
showHash();
}


bool Sha256::processAllBlocks( CharBuf& charBuf )
{
appendPadding( charBuf );
init();

Int32 max = charBuf.getLast();
if( (max % 64) != 0 )
  throw "processAllBlocks (max % 64) != 0";

for( Int32 where = 0; where < max; where += 64 )
  {
  StIO::putS( "Processing message block." );
  if( !processMessageBlock( charBuf, where ))
    {
    StIO::putS( "processMessageBlock false." );
    return false;
    }
  }


StIO::putS( "Hash at end:" );
showHash();

StIO::putS( "Finished the block chain." );
return true;
}



void Sha256::showHash( void )
{
CharBuf hashBuf;
getHash( hashBuf );

const Int32 maxBytes = hashBuf.getLast();
for( Int32 count = 0; count < maxBytes; count++ )
  {
  Uint8 oneByte = hashBuf.getU8( count );
  char leftC = ByteHex::getLeftChar( oneByte );
  StIO::putChar( leftC );
  char rightC = ByteHex::getRightChar( oneByte );
  StIO::putChar( rightC );
  StIO::putChar( ' ' );
  }

StIO::putChar( '\n' );
}



// Get the Message Digest as a series of bytes.
void Sha256::getHash( CharBuf& charBuf )
{
// From the RFC code:
//  for (i = 0; i &lt; HashSize; ++i)
//    Message_Digest[i] = (uint8_t)
//      Intermediate_Hash[
// i >> 2] // divide by 4.
// Shift the bits to make it big endian.
// >> 8 * ( 3 - ( i & 0x03 ) ));
// i mod 4


// The meaning of the word concatinate.

for( Int32 count = 0; count < 8; count++ )
  {
  Uint32 toSet = intermediateHash.getVal( count );
  charBuf.appendU32( toSet, 1024 );
  }
}



bool Sha256::processMessageBlock(
                  CharBuf& charBuf,
                  const Int32 where )
{
const Int32 last = charBuf.getLast();
if( (where + 63) >= last )
  {
  StIO::putS( "Sha256.processMessageBlock( last." );
  return false;
  }

//  for (t = t4 = 0; t &lt; 16; t++, t4 += 4)
//    W[t] =
//      (((uint32_t)Message_Block[t4]) << 24) |
//      (((uint32_t)Message_Block[t4 + 1]) << 16) |
//      (((uint32_t)Message_Block[t4 + 2]) << 8) |
//      (((uint32_t)Message_Block[t4 + 3]));


for( Int32 count = 0; count < 16; count++ )
  {
  W.setVal( count, charBuf.getU32(
                       where + (count * 4) ));
  }

for( Int32 count = 16; count < 64; count++ )
  {
  // Wt = SSIG1( W(t-2)) + W(t-7) +
  //         SSIG0( w(t-15)) + W( t-16)
  // W[t] = SHA256_sigma1(W[t-2]) + W[t-7] +
  //      SHA256_sigma0(W[t-15]) + W[t-16];

  Uint32 toSet1 = shaSSigma1( W.getVal( count - 2 ));
  Uint32 toSet2 = W.getVal( count - 7 );
  Uint32 toSet3 = shaSSigma0( W.getVal(
                                     count - 15 ));
  Uint32 toSet4 = W.getVal( count - 16 );

  Uint32 toSet = toSet1 + toSet2 + toSet3 +
                                   toSet4;

  W.setVal( count, toSet );
  }

Uint32 A = intermediateHash.getVal( 0 );
Uint32 B = intermediateHash.getVal( 1 );
Uint32 C = intermediateHash.getVal( 2 );
Uint32 D = intermediateHash.getVal( 3 );
Uint32 E = intermediateHash.getVal( 4 );
Uint32 F = intermediateHash.getVal( 5 );
Uint32 G = intermediateHash.getVal( 6 );
Uint32 H = intermediateHash.getVal( 7 );

for( Int32 t = 0; t < 64; t++ )
  {
  // T1 = h + BSIG1(e) + CH(e,f,g) + Kt + Wt
  // temp1 = H + SHA256_SIGMA1(E) +
  //       SHA_Ch(E,F,G) + K[t] + W[t];

  Uint32 temp1 = H + shaBSigma1( E ) +
         shaCh( E, F, G ) + K[t] +
         W.getVal( t );


  // T2 = BSIG0(a) + MAJ(a,b,c)
  // temp2 = SHA256_SIGMA0(A) + SHA_Maj(A,B,C);

  Uint32 temp2 = shaBSigma0( A ) +
                            shaMaj( A, B, C );

  H = G;
  G = F;
  F = E;
  E = D + temp1;
  D = C;
  C = B;
  B = A;
  A = temp1 + temp2;
  }

// Intermediate_Hash[0] += A;

intermediateHash.setVal( 0,
           intermediateHash.getVal( 0 ) + A );

intermediateHash.setVal( 1,
           intermediateHash.getVal( 1 ) + B );

intermediateHash.setVal( 2,
           intermediateHash.getVal( 2 ) + C );

intermediateHash.setVal( 3,
           intermediateHash.getVal( 3 ) + D );

intermediateHash.setVal( 4,
           intermediateHash.getVal( 4 ) + E );

intermediateHash.setVal( 5,
           intermediateHash.getVal( 5 ) + F );

intermediateHash.setVal( 6,
           intermediateHash.getVal( 6 ) + G );

intermediateHash.setVal( 7,
           intermediateHash.getVal( 7 ) + H );

return true;
}
