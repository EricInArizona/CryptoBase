// Copyright Eric Chauvin, 2022.


// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html



#include "Base64.h"
#include "../CppBase/FileIO.h"
#include "../CppBase/StIO.h"
#include "../Math/MathC.h"


Int32 Base64::lettersToBits( CharBuf& outBuf,
                             const char char1,
                             const char char2,
                             const char char3,
                             const char char4 )
{
// This should not happen.
if( char1 == padC )
  return -1;

Uint32 result = 0;

// There can be at most two pad characters
// in the last 4 letter group.
// Any characters after that would be
// ignored.


if( char2 == padC )
  return -1;

Uint32 bits1 = charTo6Bits( char1, false );
Uint32 bits2 = charTo6Bits( char2, false );
Uint32 bits3 = charTo6Bits( char3, false );
Uint32 bits4 = charTo6Bits( char4, false );

if( (bits1 >> 6) != 0 )
  return -1;

if( (bits2 >> 6) != 0 )
  return -1;

if( char3 != padC )
  {
  if( (bits3 >> 6) != 0 )
    return -1;

  }

if( char4 != padC )
  {
  if( (bits4 >> 6) != 0 )
    return -1;

  }

if( char3 == padC )
  {
  result = bits1; // Left 6 bits.
  result <<= 2;

  result |= bits2 >> 4; // 2 bits.
  outBuf.appendU8( result, 1024 );
  return 1; // 1 extra byte at end.
  }

if( char4 == padC )
  {
  // There are 16 bits left.
  result = bits1; // Left 6 bits.
  result <<= 2;
  result |= (bits2 >> 4); // 2 bits.

  result <<= 4;
  result |= bits2 & 0x0F; // 4 bits.
  result <<= 4;
  result |= (bits3 >> 2); // 4 bits.

  outBuf.appendU16( result, 1024 );

  return 2; // 2 bytes at end.
  }

// There are 24 bits left.
result = bits1; // Left 6 bits.

result <<= 6;
result |= bits2; // 12 bits.
result <<= 6;
result |= bits3; // 18 bits.
result <<= 6;
result |= bits4;  // 24 bits.

outBuf.append24Bits( result, 1024 );

return 3; // 3 bytes at end.
}




bool Base64::codeToBytes( CharBuf& inBuf,
                          CharBuf& outBuf )
{
outBuf.clear();

const Int32 lastIn = inBuf.getLast();

if( (lastIn % 4) != 0 )
  {
  StIO::putS( "Bad characters in codeToBytes." );
  // Or missing padding characters?
  return false;
  }

if( lastIn == 0 )
  return true; // Empty outBuf.

double ratio = 3.0 / 4.0;
double sizeDbl = ratio * lastIn;
Int32 outSize = MathC::roundI32( sizeDbl );
outSize += 1024;
outBuf.setSize( outSize );

for( Int32 count = 0; count < lastIn; count += 4 )
  {
  Uint32 letters4 = inBuf.getU32( count );

  // It is big endian.
  char char1 = 0x7F & (letters4 >> 24);
  char char2 = 0x7F & (letters4 >> 16);
  char char3 = 0x7F & (letters4 >> 8);
  char char4 = 0x7F & letters4;

  Int32 bytesAtEnd = lettersToBits( outBuf,
                                    char1,
                                    char2,
                                    char3,
                                    char4 );

  if( bytesAtEnd < 1 )
    return false;

  if( bytesAtEnd < 3 )
    return true; // No more to do.

  }

return true;
}




bool Base64::bytesToCode( CharBuf& inBuf,
                          CharBuf& outBuf )
{
outBuf.clear();

const Int32 lastIn = inBuf.getLast();
const Int32 remainder = lastIn % 3;
const Int32 whole3 = lastIn / 3;

if( lastIn == 0 )
  return true; // Empty outBuf.

double ratio = 4.0 / 3.0;
double sizeDbl = ratio * lastIn;
Int32 outSize = MathC::roundI32( sizeDbl );
outSize += 1024;
outBuf.setSize( outSize );

const Int32 lastPart = whole3 * 3;

for( Int32 count = 0; count < lastPart;
                                    count += 3 )
  {
  Uint32 bits24 = inBuf.get24Bits( count );
  // It is big endian.

  char letter = bits6ToChar( (bits24 >> 18) & 0x3F );
  outBuf.appendChar( letter, 1024 );

  letter = bits6ToChar( (bits24 >> 12) & 0x3F );
  outBuf.appendChar( letter, 1024 );

  letter = bits6ToChar( (bits24 >> 6) & 0x3F );
  outBuf.appendChar( letter, 1024 );

  letter = bits6ToChar( bits24 & 0x3F );
  outBuf.appendChar( letter, 1024 );
  }

if( remainder == 0 )
  return true; // No padding.

if( remainder == 1 )
  {
  // There are 8 bits left.
  Uint32 bits8 = inBuf.getU8( lastPart );
  Uint32 char1 = bits8 >> 2; // Left 6 bits.
  Uint32 char2 = (bits8 & 0x3) << 4;
                              // Right two bits.
  char letter = bits6ToChar( char1 );
  outBuf.appendChar( letter, 1024 );
  letter = bits6ToChar( char2 );
  outBuf.appendChar( letter, 1024 );

  outBuf.appendChar( padC, 1024 );
  outBuf.appendChar( padC, 1024 );

  return true;
  }

if( remainder == 2 )
  {
  // There are 16 bits left.
  Uint32 bits16 = inBuf.get16Bits( lastPart );

  Uint32 char1 = bits16 >> 10; // Left 6 bits.
  Uint32 char2 = (bits16 >> 4) & 0x3F; // Middle 6 bits.
  Uint32 char3 = (bits16 & 0xF) << 2;  // Right 4 bits.

  char letter = bits6ToChar( char1 );
  outBuf.appendChar( letter, 1024 );
  letter = bits6ToChar( char2 );
  outBuf.appendChar( letter, 1024 );
  letter = bits6ToChar( char3 );
  outBuf.appendChar( letter, 1024 );

  outBuf.appendChar( padC, 1024 );

  return true;
  }

// This can't happen.
return false;
}



void Base64::test( void )
{
// Test Vectors from RFC 4648:
// BASE64("") = ""
// BASE64("f") = "Zg=="
// BASE64("fo") = "Zm8="
// BASE64("foo") = "Zm9v"
// BASE64("foob") = "Zm9vYg=="
// BASE64("fooba") = "Zm9vYmE="
// BASE64("foobar") = "Zm9vYmFy"

StIO::putS( "Starting Base64 test." );

CharBuf fileData;
CharBuf outData;
CharBuf testDataIn;
CharBuf testDataOut;


FileIO::readAll( "\\ABackups\\TestGMap.zip",
                 fileData );

/*
fileData.appendChar( 'f', 1024 );
fileData.appendChar( 'o', 1024 );
fileData.appendChar( 'o', 1024 );

fileData.appendChar( 'f', 1024 );
fileData.appendChar( 'o', 1024 );
fileData.appendChar( 'o', 1024 );

fileData.appendChar( 'f', 1024 );
fileData.appendChar( 'o', 1024 );
fileData.appendChar( 'o', 1024 );

// Add some characters like in MIME.
fileData.appendChar( '\r', 1024 );
fileData.appendChar( '\n', 1024 );

fileData.appendChar( 'f', 1024 );
fileData.appendChar( 'o', 1024 );
fileData.appendChar( 'o', 1024 );

fileData.appendChar( 'b', 1024 );
// fileData.appendChar( 'a', 1024 );
// fileData.appendChar( 'r', 1024 );

StIO::putCharBuf( fileData );
StIO::putChar( '\n' );
*/

// Filter out things like line feed and CR,
// like with MIME encoding.
filterCharSet( fileData, testDataIn );

bytesToCode( testDataIn, outData );
StIO::putChar( '\n' );

// StIO::putCharBuf( outData );
// StIO::putChar( '\n' );

codeToBytes( outData,
             testDataOut );


// StIO::putS( "\nBack to normal:" );
// StIO::putCharBuf( testDataOut );
// StIO::putChar( '\n' );

if( testDataIn.equalsCBuf( testDataOut ))
  StIO::putS( "Test is good." );
else
  StIO::putS( "Test is bad." );


// A zip file should contain random looking data.
// The full range of bytes.  Also when converting
// it back to a zip file you can tell if it
// got corrupted if it can't be unzipped.

FileIO::writeAll( "\\ABackups\\TestGMapTest.zip",
                  testDataOut );

}



void Base64::setupBoolAlpha( void )
{
boolAlpha.setSize( boolAlphaSize );

for( Int32 count = 0; count < boolAlphaSize;
                                       count++ )
  boolAlpha.setVal( count, false );

const Int32 max = charSet.getLast();
for( Int32 count = 0; count < max; count++ )
  {
  char c = charSet.getC( count );
  boolAlpha.setVal( c, true );
  }

boolAlpha.setVal( padC, true );
}



void Base64::filterCharSet( CharBuf& inBuf,
                            CharBuf& outBuf )
{
// StIO::putS( "Filtering data." );

const Int32 max = inBuf.getLast();
outBuf.setSize( max + 16 );

for( Int32 count = 0; count < max; count++ )
  {
  char c = inBuf.getC( count );
  // char is signed.
  if( c <= ' ' ) // Includes CR, LF, tab, etc.
    continue;

  if( c >= boolAlphaSize )
    continue;

  // If it's not in the character set or padC.
  if( !boolAlpha.getVal( c ))
    continue;

  outBuf.appendChar( c, 1024 );
  // StIO::putChar( c );
  }

// StIO::putChar( '\n' );
}
