// Copyright Eric Chauvin 2022


// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html


#include "Sha256.h"
#include "../CppBase/StIO.h"
#include "../CryptoBase/Base16Number.h"



// Block size is 512 bits for SHA-256
// and 1024 bits for SHA-512.


void Sha256::appendPadding( CharBuf& charBuf )
{
const Uint64 originalLength =
             Casting::i32ToU64( charBuf.getLast());

// StIO::printF( "originalLength: " );
// StIO::printFD( Casting::u64ToI32(
             // originalLength ));
// StIO::putS( "" );


// To restore it to the original length:
// CharBuf.truncateLast( Int32 setTo )

// Append that 1 bit.
charBuf.appendChar( static_cast<char>( 0x80 ),
                                        1024 );

Int32 howBig = charBuf.getLast() % 64;

// StIO::printF( "howBig: " );
// StIO::printFD( howBig );
// StIO::putS( "" );


Int32 toAdd = 64 - howBig;

// You have to add the least number of zeros
// possible in order to make it zero mod
// 64 bytes.
// It already has seven zero bits after that
// 1 bit.

if( toAdd == 64 )
  toAdd = 0;

// StIO::printF( "toAdd at top: " );
// StIO::printFD( toAdd );
// StIO::putS( "" );

// For SHA 512 it's a 128 bit length value.
// So 16 bytes.

// 8 bytes for length.
toAdd -= 8;
if( toAdd < 0 )
  toAdd += 64;

// StIO::printF( "toAdd: " );
// StIO::printFD( toAdd );
// StIO::putS( "" );

for( Int32 count = 0; count < toAdd; count++ )
  charBuf.appendChar( 0, 1024 );

Uint64 lengthInBits = originalLength * 8;
charBuf.appendUint64( lengthInBits,
                      1024 );

Int32 finalSize = charBuf.getLast();

// StIO::printF( "finalSize mod: " );
// StIO::printFD( finalSize % 64 );
// StIO::putS( "" );

if( (finalSize % 64) != 0 )
  throw "SHA padding finalSize is not right.";

// Maximum message size is 2^64 bits.
}



void Sha256::init( void )
{
W.setSize( 64 );

intermediateHash.setSize( 8 );

// This is how they get set before the first
// block, but after the first block they are
// using the intermediate values from the
// previous block.  A block chain.
// Blocks chained together.

intermediateHash.setVal( 0, 0x6a09e667 );
intermediateHash.setVal( 1, 0xbb67ae85 );
intermediateHash.setVal( 2, 0x3c6ef372 );
intermediateHash.setVal( 3, 0xa54ff53a );
intermediateHash.setVal( 4, 0x510e527f );
intermediateHash.setVal( 5, 0x9b05688c );
intermediateHash.setVal( 6, 0x1f83d9ab );
intermediateHash.setVal( 7, 0x5be0cd19 );
}


// Process one char array of 64 bytes.
// That's 512 bits.
bool Sha256::processAllBlocks(
                         const CharBuf& charBuf )
{
Int32 max = charBuf.getLast();
for( Int32 where = 0; where < max; where += 64 )
  {
  if( !Sha256::processMessageBlock(
                             charBuf, where ))
    {
    StIO::putS( "processMessageBlock false." );
    return false;
    }
  }


// Get the hash value.
// Base16Numbers.h.
// Then do a toString().
// Need test vectors.


StIO::putS( "Finished the block chain." );
return true;
}


// Get the Message Digest as a series of bytes.
void Sha256::getHash( CharBuf& charBuf )
{
// This gets an array of bytes that is the
// bit string for the hash.
// The Message Digest is an array of bytes.

Uint32 toSet = intermediateHash.getVal( 0 );
charBuf.appendUint32( toSet, 1024 );

toSet = intermediateHash.getVal( 1 );
charBuf.appendUint32( toSet, 1024 );

toSet = intermediateHash.getVal( 2 );
charBuf.appendUint32( toSet, 1024 );

toSet = intermediateHash.getVal( 3 );
charBuf.appendUint32( toSet, 1024 );

toSet = intermediateHash.getVal( 4 );
charBuf.appendUint32( toSet, 1024 );

toSet = intermediateHash.getVal( 5 );
charBuf.appendUint32( toSet, 1024 );

toSet = intermediateHash.getVal( 6 );
charBuf.appendUint32( toSet, 1024 );

toSet = intermediateHash.getVal( 7 );
charBuf.appendUint32( toSet, 1024 );
}



bool Sha256::processMessageBlock(
                  const CharBuf& charBuf,
                  const Int32 where )
{
const Int32 last = charBuf.getLast();
if( (where + 63) >= last )
  {
  StIO::putS( "Sha256.processMessageBlock( last." );
  return false;
  }

for( Int32 count = 0; count < 16; count++ )
  {
  W.setVal( count, charBuf.getUint32(
                       where + (count * 4) ));
  }

for( Int32 count = 16; count < 64; count++ )
  {
  // Wt = SSIG1(W(t-2)) + W(t-7) +
  //         SSIG0(w(t-15)) + W(t-16)

  Uint32 toSet1 = shaSSigma1( W.getVal( count - 2 ));
  Uint32 toSet2 = W.getVal( count - 7 );
  Uint32 toSet3 = shaSSigma0( W.getVal(
                                     count - 15 ));
  Uint32 toSet4 = W.getVal( count - 16 );

  // shaAdd is commutative.
  Uint32 toSet = shaAdd( toSet1, toSet2 );
  toSet = shaAdd( toSet, toSet3 );
  toSet = shaAdd( toSet, toSet4 );

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
  Uint32 temp1 = shaAdd( H, shaBSigma1( E ));
  temp1 = shaAdd( temp1, shaCh( E, F, G ));
  temp1 = shaAdd( temp1, K[t] );
  temp1 = shaAdd( temp1, W.getVal( t ));

  Uint32 temp2 = shaAdd( shaBSigma0( A ),
                            shaMaj( A, B, C));
  H = G;
  G = F;
  F = E;
  E = shaAdd( D, temp1 );
  D = C;
  C = B;
  B = A;
  A = shaAdd( temp1, temp2 );
  }

Uint32 tempHash = shaAdd(
              intermediateHash.getVal( 0 ), A );
intermediateHash.setVal( 0, tempHash );

tempHash = shaAdd(
              intermediateHash.getVal( 1 ), B );
intermediateHash.setVal( 1, tempHash );

tempHash = shaAdd(
              intermediateHash.getVal( 2 ), C );
intermediateHash.setVal( 2, tempHash );

tempHash = shaAdd(
              intermediateHash.getVal( 3 ), D );
intermediateHash.setVal( 3, tempHash );

tempHash = shaAdd(
              intermediateHash.getVal( 4 ), E );
intermediateHash.setVal( 4, tempHash );

tempHash = shaAdd(
              intermediateHash.getVal( 5 ), F );
intermediateHash.setVal( 5, tempHash );

tempHash = shaAdd(
              intermediateHash.getVal( 6 ), G );
intermediateHash.setVal( 6, tempHash );

tempHash = shaAdd(
              intermediateHash.getVal( 7 ), H );
intermediateHash.setVal( 7, tempHash );

return true;
}
