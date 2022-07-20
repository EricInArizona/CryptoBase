// Copyright Eric Chauvin 2022.



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html



#include "Aes.h"



// This is only partially done.
// I'm still translating from my old C# code.
// After I translate it from my C# code I have
// to check it against the specs and the test
// vectors.


/*

    // InBlock plain text for the test vector example in Appendix C.3.
    // PLAINTEXT: 00112233445566778899aabbccddeeff
    byte[] TestBlock = new byte[16]
      {
      0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
      0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff
      };

*/




void Aes::keyExpansion( void )
{
// Makes a total of Nb(Nr + 1) words.

// Using these variable names to match up with
// the specs in the FIPS document.

// Nb = Number of columns, 32 bit words, in
// the state.  It is always 4.
// const Int32 Nb = 4;


// Nk = Number of 32 bit words in the key.
// It is 4, 6 or 8.
const Int32 Nk = keyWordsSize;

// Nr = Number of rounds.
// const Int32 Nr = numberOfRounds;

for( Int32 count = 0; count < Nk; count++ )
  {
  // The four bytes: 0x60, 0x3d, 0xeb, 0x10,
  // become the uint: w0 = 603deb10
  Uint32 theWord = makeUintFromBytes(
                  aesKey.getV( 4 * count ),
                  aesKey.getV( (4 * count) + 1 ),
                  aesKey.getV( (4 * count) + 2 ),
                  aesKey.getV( (4 * count) + 3 ));

  aesKeyWords.setV( count, theWord );
  }

// max = 4 * (numberOfRounds + 1)
const Int32 max = keyWordsSize;
for( Int32 count = Nk; count < max; count++ )
  {
  Uint32 temp = aesKeyWords.getV( count - 1 );

  if( (count % Nk) == 0) // mod 4, 6 or 8.
    {
    Uint32 rConVal = aesRCon.getV( count / Nk );
    temp = subWord( rotateWord( temp )) ^ rConVal;
    }

  // The above if-statement can't be true if
  // this is true.  It is an else-if.
  // if Nk is > 6 then this is a 256 bit key.
  if( (Nk > 6) && ((count % Nk) == 4) )
    {
    temp = subWord( temp );
    }

  Uint32 prevWord = aesKeyWords.getV( count - Nk );
  aesKeyWords.setV( count, prevWord ^ temp );
  }

moveKeyScheduleWordsToBytes();
}



void Aes::moveKeyScheduleWordsToBytes( void )
{
const Int32 max = keyWordsSize;
for( Int32 count = 0; count < max; count++ )
  {
  Uint32 aWord = aesKeyWords.getV( count );

  // Row, Column
  Uint8 theByte = (aWord >> 24) & 0xFF;
  aesKeyBytes.setV( 0, count, theByte );

  theByte = (aWord >> 16) & 0xFF;
  aesKeyBytes.setV( 1, count, theByte );

  theByte = (aWord >> 8) & 0xFF;
  aesKeyBytes.setV( 2, count, theByte );

  theByte = aWord & 0xFF;
  aesKeyBytes.setV( 3, count, theByte );
  }
}


void Aes::addRoundKey( Int32 round )
{
// StateArray[Row, Column]
Uint8 newByte = aesState.getV( 0, 0 ) ^
                 aesKeyBytes.getV( 0, round * 4 );
aesState.setV( 0, 0, newByte );

newByte = aesState.getV( 1, 0 ) ^
                 aesKeyBytes.getV( 1, round * 4 );
aesState.setV( 1, 0, newByte );

newByte = aesState.getV( 2, 0 ) ^
                 aesKeyBytes.getV( 2, round * 4 );
aesState.setV( 2, 0, newByte );

newByte = aesState.getV( 3, 0 ) ^
                 aesKeyBytes.getV( 3, round * 4 );
aesState.setV( 3, 0, newByte );


newByte = aesState.getV( 0, 1 ) ^
           aesKeyBytes.getV( 0, (round * 4) + 1 );
aesState.setV( 0, 1, newByte );

newByte = aesState.getV( 1, 1 ) ^
           aesKeyBytes.getV( 1, (round * 4) + 1 );
aesState.setV( 1, 1, newByte );

newByte = aesState.getV( 2, 1 ) ^
           aesKeyBytes.getV( 2, (round * 4) + 1 );
aesState.setV( 2, 1, newByte );

newByte = aesState.getV( 3, 1 ) ^
           aesKeyBytes.getV( 3, (round * 4) + 1 );
aesState.setV( 3, 1, newByte );


newByte = aesState.getV( 0, 2 ) ^
           aesKeyBytes.getV( 0, (round * 4) + 2 );
aesState.setV( 0, 2, newByte );

newByte = aesState.getV( 1, 2 ) ^
           aesKeyBytes.getV( 1, (round * 4) + 2 );
aesState.setV( 1, 2, newByte );

newByte = aesState.getV( 2, 2 ) ^
           aesKeyBytes.getV( 2, (round * 4) + 2 );
aesState.setV( 2, 2, newByte );

newByte = aesState.getV( 3, 2 ) ^
           aesKeyBytes.getV( 3, (round * 4) + 2 );
aesState.setV( 3, 2, newByte );


newByte = aesState.getV( 0, 3 ) ^
           aesKeyBytes.getV( 0, (round * 4) + 3 );
aesState.setV( 0, 3, newByte );

newByte = aesState.getV( 1, 3 ) ^
           aesKeyBytes.getV( 1, (round * 4) + 3 );
aesState.setV( 1, 3, newByte );

newByte = aesState.getV( 2, 3 ) ^
           aesKeyBytes.getV( 2, (round * 4) + 3 );
aesState.setV( 2, 3, newByte );

newByte = aesState.getV( 3, 3 ) ^
           aesKeyBytes.getV( 3, (round * 4) + 3 );
aesState.setV( 3, 3, newByte );
}



void Aes::subBytes( void )
{
aesState.setV( 0, 0, subByte(
                     aesState.getV( 0, 0 )));
aesState.setV( 1, 0, subByte(
                     aesState.getV( 1, 0 )));
aesState.setV( 2, 0, subByte(
                     aesState.getV( 2, 0 )));
aesState.setV( 3, 0, subByte(
                     aesState.getV( 3, 0 )));

aesState.setV( 0, 1, subByte(
                     aesState.getV( 0, 1 )));
aesState.setV( 1, 1, subByte(
                     aesState.getV( 1, 1 )));
aesState.setV( 2, 1, subByte(
                     aesState.getV( 2, 1 )));
aesState.setV( 3, 1, subByte(
                     aesState.getV( 3, 1 )));

aesState.setV( 0, 2, subByte(
                     aesState.getV( 0, 2 )));
aesState.setV( 1, 2, subByte(
                     aesState.getV( 1, 2 )));
aesState.setV( 2, 2, subByte(
                     aesState.getV( 2, 2 )));
aesState.setV( 3, 2, subByte(
                     aesState.getV( 3, 2 )));

aesState.setV( 0, 3, subByte(
                     aesState.getV( 0, 3 )));
aesState.setV( 1, 3, subByte(
                     aesState.getV( 1, 3 )));
aesState.setV( 2, 3, subByte(
                     aesState.getV( 2, 3 )));
aesState.setV( 3, 3, subByte(
                     aesState.getV( 3, 3 )));
}



void Aes::shiftRows( void )
{
// The first row is unchanged.
// StateArray[Row, Column]

Uint8 temp = aesState.getV( 1, 0 );

aesState.setV( 1, 0, aesState.getV( 1, 1 ));
aesState.setV( 1, 1, aesState.getV( 1, 2 ));
aesState.setV( 1, 2, aesState.getV( 1, 3 ));
aesState.setV( 1, 3, temp );

temp = aesState.getV( 2, 0 );
Uint8 temp2 = aesState.getV( 2, 1 );

aesState.setV( 2, 0, aesState.getV( 2, 2 ));
aesState.setV( 2, 1, aesState.getV( 2, 3 ));
aesState.setV( 2, 2, temp );
aesState.setV( 2, 3, temp2 );

temp = aesState.getV( 3, 0 );
temp2 = aesState.getV( 3, 1 );
Uint8 temp3 = aesState.getV( 3, 2 );

aesState.setV( 3, 0, aesState.getV( 3, 3 ));
aesState.setV( 3, 1, temp );
aesState.setV( 3, 2, temp2 );
aesState.setV( 3, 3, temp3 );
}




void Aes::mixColumns( void )
{
AesState tempState;

Uint8 val =
  galoisMultiply( 0x02, aesState.getV( 0, 0 )) ^
  galoisMultiply( 0x03, aesState.getV( 1, 0 )) ^
  aesState.getV( 2, 0 ) ^
  aesState.getV( 3, 0 );

tempState.setV( 0, 0, val );


val = aesState.getV( 0, 0 ) ^
  galoisMultiply( 0x02, aesState.getV( 1, 0 )) ^
  galoisMultiply( 0x03, aesState.getV( 2, 0 )) ^
  aesState.getV( 3, 0 );

tempState.setV( 1, 0, val );


val = aesState.getV( 0, 0 ) ^
      aesState.getV( 1, 0 ) ^
 galoisMultiply( 0x02, aesState.getV( 2, 0 )) ^
 galoisMultiply( 0x03, aesState.getV( 3, 0 ));

tempState.setV( 2, 0, val );


val =
  galoisMultiply( 0x03, aesState.getV( 0, 0 )) ^
  aesState.getV( 1, 0 ) ^
  aesState.getV( 2, 0 ) ^
  galoisMultiply( 0x02, aesState.getV( 3, 0 ));

tempState.setV( 3, 0, val );


val =
  galoisMultiply( 0x02, aesState.getV( 0, 1 )) ^
  galoisMultiply( 0x03, aesState.getV( 1, 1 )) ^
  aesState.getV( 2, 1 ) ^
  aesState.getV( 3, 1 );

tempState.setV( 0, 1, val );


val =
  aesState.getV( 0, 1 ) ^
  galoisMultiply( 0x02, aesState.getV( 1, 1 )) ^
  galoisMultiply( 0x03, aesState.getV( 2, 1 )) ^
  aesState.getV( 3, 1 );

tempState.setV( 1, 1, val );


val = aesState.getV( 0, 1 ) ^
      aesState.getV( 0, 1 ) ^
  galoisMultiply( 0x02, aesState.getV( 2, 1 )) ^
  galoisMultiply( 0x03, aesState.getV( 3, 1 ));

tempState.setV( 2, 1, val );


val =
  galoisMultiply( 0x03, aesState.getV( 0, 1 )) ^
  aesState.getV( 1, 1 ) ^
  aesState.getV( 2, 1 ) ^
  galoisMultiply( 0x02, aesState.getV( 3, 1 ));

tempState.setV( 3, 1, val );


val =
  galoisMultiply( 0x02, aesState.getV( 0, 2 )) ^
  galoisMultiply( 0x03, aesState.getV( 1, 2 )) ^
  aesState.getV( 2, 2 ) ^
  aesState.getV( 3, 2 );

tempState.setV( 0, 2, val );


val = aesState.getV( 0, 2 ) ^
  galoisMultiply( 0x02, aesState.getV( 1, 2 )) ^
  galoisMultiply( 0x03, aesState.getV( 2, 2 )) ^
  aesState.getV( 3, 2 );

tempState.setV( 1, 2, val );


val = aesState.getV( 0, 2 ) ^
      aesState.getV( 1, 2 ) ^
  galoisMultiply( 0x02, aesState.getV( 2, 2 )) ^
  galoisMultiply( 0x03, aesState.getV( 3, 2 ));

tempState.setV( 2, 2, val );


val =
  galoisMultiply( 0x03, aesState.getV( 0, 2 )) ^
  aesState.getV( 1, 2 ) ^
  aesState.getV( 2, 2 ) ^
  galoisMultiply( 0x02, aesState.getV( 3, 2 ));

tempState.setV( 3, 2, val );


val =
  galoisMultiply( 0x02, aesState.getV( 0, 3 )) ^
  galoisMultiply( 0x03, aesState.getV( 1, 3 )) ^
  aesState.getV( 2, 3 ) ^
  aesState.getV( 3, 3 );

tempState.setV( 0, 3, val );


val = aesState.getV( 0, 3 ) ^
  galoisMultiply( 0x02, aesState.getV( 1, 3 )) ^
  galoisMultiply( 0x03, aesState.getV( 2, 3 )) ^
  aesState.getV( 3, 3 );

tempState.setV( 1, 3, val );


val = aesState.getV( 0, 3 ) ^
      aesState.getV( 1, 3 ) ^
  galoisMultiply( 0x02, aesState.getV( 2, 3 )) ^
  galoisMultiply( 0x03, aesState.getV( 3, 3 ));

tempState.setV( 2, 3, val );


val =
  galoisMultiply( 0x03, aesState.getV( 0, 3 )) ^
  aesState.getV( 1, 3 ) ^
  aesState.getV( 2, 3 ) ^
  galoisMultiply( 0x02, aesState.getV( 3, 3 ));

tempState.setV( 3, 3, val );

aesState.copy( tempState );
}




void Aes::encryptCharBuf( const CharBuf& inBuf,
                          CharBuf& outBuf,
                          const Int32 where )
{
moveCharBufToState( inBuf, where );

// For the test vectors.
//     if( GetStateString().ToLower() !=
        //  "00112233445566778899aabbccddeeff" )

addRoundKey( 0 );

// This loop is what needs to be optimized.
for( Int32 round = 1; round < numberOfRounds;
                                      round++ )
  {
  // if( !MatchesTestVector( Round, GetStateString()))

  // test = 2;
  subBytes();
  shiftRows();
  mixColumns();
  addRoundKey( round );
  }

// Do the last part without the mixColumns():

subBytes();
shiftRows();
addRoundKey( numberOfRounds );

// if( GetStateString().ToLower() !=
          // "8ea2b7ca516745bfeafc49904b496089" )

moveStateToCharBuf( outBuf );
}





void Aes::setKey( const CharBuf& theKey,
                  const Int32 keySize )
{
// 10 rounds for 128 bit key,
// 12 rounds for 192,
// 14 rounds for 256 bit key.

// Not using 192 bit key size.
if( !((keySize == 16) || (keySize == 32)) )
  throw "Aes key size has to be 16 or 32.";

if( keySize == 16 )
  {
  keyLengthInBytes = 16; // 128 / 8
  numberOfRounds = 10;
  }

if( keySize == 32 )
  {
  keyLengthInBytes = 32; // 256 / 8
  numberOfRounds = 14;
  }

keyWordsSize = 4 * (numberOfRounds + 1);

if( keySize != theKey.getLast())
  throw "Aes keySize doesn't match.";

for( Int32 count = 0; count < keySize; count++ )
aesKey.setV( count, theKey.getU8( count ));

keyExpansion();
}



/*
    // This key is from the FIPS document
    // Appendix A - Key Expansion Examples.
    ///////////
    // Key for the Key Expansion example:
    // byte[] Key = new byte[KeyLengthInBytes]
    //  {
    //  0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe,
    //  0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81,
    //  0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7,
    //  0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4
    //  };
      ////////

    // Key for the test vector example in Appendix C.3.
    // byte[] Key = new byte[KeyLengthInBytes]
     // {
     // 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
     // 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
     // 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
     // 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f
     // };

*/



/*
// This is meant to be used to compare with
// the test vectors
// in Appendix C.3.
internal string GetStateString()
    {
    StringBuilder SBuilder = new StringBuilder();

    // Column-major order:
    for( int Column = 0; Column < 4; Column++ )
      {
      for( int Row = 0; Row < 4; Row++ )
        SBuilder.Append( StateArray[Row, Column].ToString( "X2" ));

      }

    return SBuilder.ToString();
    }
*/



void Aes::encryptTest( void )
{
/*
// InBlock plain text for the test vector
// example in Appendix C.3.
// PLAINTEXT: 00112233445566778899aabbccddeeff
    byte[] TestBlock = new byte[16]
      {
      0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
      0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff
      };
*/

CharBuf plainText;
plainText.appendU8( 0x00, 1024 );
plainText.appendU8( 0x11, 1024 );
plainText.appendU8( 0x22, 1024 );
plainText.appendU8( 0x33, 1024 );
plainText.appendU8( 0x44, 1024 );
plainText.appendU8( 0x55, 1024 );
plainText.appendU8( 0x66, 1024 );
plainText.appendU8( 0x77, 1024 );
plainText.appendU8( 0x88, 1024 );
plainText.appendU8( 0x99, 1024 );
plainText.appendU8( 0xaa, 1024 );
plainText.appendU8( 0xbb, 1024 );
plainText.appendU8( 0xcc, 1024 );
plainText.appendU8( 0xdd, 1024 );
plainText.appendU8( 0xee, 1024 );
plainText.appendU8( 0xff, 1024 );

StIO::putS( "PlainText set." );


//     EncryptBlock( TestBlock, OutBlock );
}



/*
======

Set 3 vector 4
mode=aes-256
key=603deb1015ca71be2b73aef0857d77811f352c073b6108d72d9810a30914dff4
iv=
plain=f69f2445df4f9b17ad2b417be66c3710
cipher=23304b7a39f9f3ff067d8d8f9e24ecc7



Set 3 vector 2
mode=aes-256
key=603deb1015ca71be2b73aef0857d77811f352c073b6108d72d9810a30914dff4
iv=
plain=ae2d8a571e03ac9c9eb76fac45af8e51
cipher=591ccb10d410ed26dc5ba74a31362870
Set 3 vector 3
mode=aes-256
key=603deb1015ca71be2b73aef0857d77811f352c073b6108d72d9810a30914dff4
iv=



Set 1 vector 1
mode=aes-128
key=2b7e151628aed2a6abf7158809cf4f3c
iv=
plain=6bc1bee22e409f96e93d7e117393172a
cipher=3ad77bb40d7a3660a89ecaf32466ef97
Set 1 vector 2
mode=aes-128
key=2b7e151628aed2a6abf7158809cf4f3c
iv=
plain=ae2d8a571e03ac9c9eb76fac45af8e51
cipher=f5d3d58503b9699de785895a96fdbaaf
Set 1 vector 3
mode=aes-128
key=2b7e151628aed2a6abf7158809cf4f3c
iv=


et 1 vector 3
mode=aes-128
key=2b7e151628aed2a6abf7158809cf4f3c
iv=
plain=30c81c46a35ce411e5fbc1191a0a52ef
cipher=43b1cd7f598ece23881b00e3ed030688
Set 1 vector 4
mode=aes-128
key=2b7e151628aed2a6abf7158809cf4f3c
iv=
plain=f69f2445df4f9b17ad2b417be66c3710
cipher=7b0c785e27e8ad3f8223207104725dd4
Set 2 vector 1
mode=aes-192
key=8e73b0f7da0e6452c810f32b809079e562f8ead2522c6b7b
iv=

*/
