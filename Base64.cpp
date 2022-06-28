// Copyright Eric Chauvin, 2022.


// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html



#include "Base64.h"
#include "../CppBase/FileIO.h"
#include "../CppBase/StIO.h"
#include "../CppBase/MathC.h"


bool Base64::lettersToBits( CharBuf& outBuf,
                             const char char1,
                             const char char2,
                             const char char3,
                             const char char4 )
{
Uint32 result = 0;

// There can be at most two pad characters
// in the last 4 letter group.
// Any characters after that would be
// ignored.

if( char1 == padC )
  throw "Base64 padC at first char.";
  // return false;

if( char2 == padC )
  throw "Base64 padC at second char.";
  // return false;

Uint32 bits1 = charTo6Bits( char1, false );
Uint32 bits2 = charTo6Bits( char2, false );
Uint32 bits3 = charTo6Bits( char3, false );
Uint32 bits4 = charTo6Bits( char4, false );

if( (bits1 >> 6) != 0 )
  throw "Base64 Bad bits a bits1.";
  // return false;

if( (bits2 >> 6) != 0 )
  throw "Base64 Bad bits a bits2.";
  // return false;

if( char3 != padC )
  {
  if( (bits3 >> 6) != 0 )
    throw "Base64 Bad bits a bits3.";

  // return false;
  }

if( char4 != padC )
  {
  if( (bits4 >> 6) != 0 )
    throw "Base64 Bad bits a bits4.";

  }

if( char3 == padC )
  {
  result = bits1; // Left 6 bits.
  result <<= 2;
  result |= bits2; // Right 2 bits.
  outBuf.appendU8( result, 1024 );
  return false; // No more to do.
  }

if( char4 == padC )
  {
  // There are 16 bits left.
  result = bits1; // Left 6 bits.
  result <<= 2;
  result |= bits2 >> 4; // 2 bits.
  result <<= 4;
  result |= bits2 >> 2; // 4 bits.
  result <<= 4;
  result |= bits3; // 4 bits.

  outBuf.appendU16( result, 1024 );
  return false; 
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
 
return true; // More to do?
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
Int32 outSize = MathC::round32( sizeDbl );
outSize += 1024;
outBuf.setSize( outSize );

StIO::putS( "out size: " );
StIO::printFD( outSize );
StIO::putChar( '\n' );

for( Int32 count = 0; count < lastIn; count += 4 )
  {
  Uint32 letters4 = inBuf.getU32( count );

  // It is big endian.
  char char1 = 0x7F & (letters4 >> 24);
  char char2 = 0x7F & (letters4 >> 16);
  char char3 = 0x7F & (letters4 >> 8);
  char char4 = 0x7F & letters4;

  if( !lettersToBits( outBuf,
                      char1,
                      char2,
                      char3,
                      char4 ))
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
Int32 outSize = MathC::round32( sizeDbl );
outSize += 1024;
outBuf.setSize( outSize );

// StIO::putS( "out size: " );
// StIO::printFD( outSize );
// StIO::printF( "\n" );

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

// if( (outBuf.getLast() % 4) != 0 )
  // throw "Base64 out mod 4 not zero.";

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
CharBuf testData;

// FileIO::readAll( "\\ABackups\\TestGMap.zip",
//                 fileData );


fileData.appendChar( 'f', 1024 );
fileData.appendChar( 'o', 1024 );
fileData.appendChar( 'o', 1024 );

fileData.appendChar( 'f', 1024 );
fileData.appendChar( 'o', 1024 );
fileData.appendChar( 'o', 1024 );

fileData.appendChar( 'f', 1024 );
fileData.appendChar( 'o', 1024 );
fileData.appendChar( 'o', 1024 );

fileData.appendChar( 'f', 1024 );
fileData.appendChar( 'o', 1024 );
fileData.appendChar( 'o', 1024 );

fileData.appendChar( 'b', 1024 );
fileData.appendChar( 'a', 1024 );
// fileData.appendChar( 'r', 1024 );

StIO::putCharBuf( fileData );
StIO::printF( "\n" );


bytesToCode( fileData, outData );

StIO::putCharBuf( outData );
StIO::printF( "\n" );
StIO::putS( "End of data." );


codeToBytes( outData,
             testData );


StIO::putS( "\nBack to normal:" );

StIO::putCharBuf( testData );
StIO::printF( "\n" );
StIO::putS( "End of data." );

if( testData.equalsCBuf( fileData ))
  StIO::putS( "Test is good." );
else
  StIO::putS( "Test is bad." );




// A zip file should contain random looking data.
// The full range of bytes.  Also converting
// it back to a zip file you can tell if it
// got corrupted if it can't be unzipped.

// FileIO::writeAll( "\\ABackups\\TestGMapTest.zip",
 //                 testData );

}


void Base64::setupBoolAlpha( void )
{
const Int32 alphaSize = 128;

boolAlpha.setSize( alphaSize );

for( Int32 count = 0; count < alphaSize; count++ )
  boolAlpha.setVal( count, false );

const Int32 max = charSet.getLast();
for( Int32 count = 0; count < max; count++ )
  {
  char c = charSet.getC( count );
  boolAlpha.setVal( c, true );
  }

boolAlpha.setVal( padC, true );
}
