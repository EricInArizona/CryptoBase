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

    byte[] OutBlock = new byte[16];
    byte[] PlainBlock = new byte[16];
    byte[] PreviousBlock = new byte[16];

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

Uint8 val = galoisMultiply( 0x02,
                aesState.getV( 0, 0 )) ^
            galoisMultiply( 0x03,
                aesState.getV( 1, 0 )) ^
            aesState.getV( 2, 0 ) ^
            aesState.getV( 3, 0 );

tempState.setV( 0, 0, val );


val = aesState.getV( 0, 0 ) ^
      galoisMultiply( 0x02,
                 aesState.getV( 1, 0 )) ^
      galoisMultiply( 0x03,
                 aesState.getV( 2, 0 )) ^
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



/*

  private void EncryptBlock( byte[] InBlock, byte[] OutBlock )
    {
    int Test = 1;
    try
    {
    MoveInBlockToState( InBlock );

    //////
    // For the test vectors.
    if( GetStateString().ToLower() != "00112233445566778899aabbccddeeff" )
      {
      MessageBox.Show( "Initial state doesn't match test vector.", "Rad Net Server", MessageBoxButtons.OK );
      return;
      }
      ///////

    // ShowStatus( "Initial State:" );
    // ShowStatus( GetStateString() );
    // ShowStatus( " " );

    AddRoundKey( 0 );
    // ShowStatus( "After AddRoundKey:" );
    // ShowStatus( GetStateString() );

    // This loop is what needs to be optimized.
    for( int Round = 1; Round < NumberOfRounds; Round++ )
      {
      // What is it that can be optimized by
      // using a lookup table of 32 bit words?

      // ShowStatus( " " );
      // ShowStatus( "Round: " + Round.ToString());
      //////
      if( !MatchesTestVector( Round, GetStateString()))
        {
        ShowStatus( "Didn't match the test vector." );
        return;
        }
        ///////

      Test = 2;
      SubBytes();
      // ShowStatus( "After SubBytes:" );
      // ShowStatus( GetStateString() );

      Test = 3;
      ShiftRows();
      // ShowStatus( "After ShiftRows:" );
      // ShowStatus( GetStateString() );

      Test = 4;
      MixColumns();
      // ShowStatus( "After MixColumns:" );
      // ShowStatus( GetStateString() );

      Test = 5;
      AddRoundKey( Round );
      // ShowStatus( "After AddRoundKey:" );
      // ShowStatus( GetStateString() );
      }

    // ShowStatus( " " );
    // ShowStatus( "Round: 14" );
    //////////
    if( !MatchesTestVector( 14, GetStateString()))
      {
      ShowStatus( "Didn't match the test vector." );
      return;
      }
      /////////


    // Do the last part without the MixColumns():
    Test = 6;
    SubBytes();
    Test = 7;
    ShiftRows();
    Test = 8;
    AddRoundKey( NumberOfRounds );
    Test = 9;

    // round[14].output 8ea2b7ca516745bfeafc49904b496089
    //////
    if( GetStateString().ToLower() != "8ea2b7ca516745bfeafc49904b496089" )
      {
      ShowStatus( "Didn't match the final output." );
      return;
      }
      //////

    MoveStateToOutBlock( OutBlock );

    // ShowStatus( " " );
    // ShowStatus( "It passed the tests." );
    // ShowStatus( " " );
    }
    catch( Exception Except )
      {
      MessageBox.Show( "Error in EncryptBlock. Test is: " + Test.ToString() + "\r\n" + Except.Message, MainForm.MessageBoxTitle, MessageBoxButtons.OK, MessageBoxIcon.Stop );
      // return false;
      }
    }





  internal void SetKey( string TheKey )
    {
    for( int Count = 0; Count < 32; Count++ )
      Key[Count] = 0;

    if( TheKey == null )
      return;

    int HowMany = TheKey.Length;
    if( HowMany > 32 )
      HowMany = 32;

    for( int Count = 0; Count < HowMany; Count++ )
      Key[Count] = (byte)(TheKey[Count]);

    KeyExpansion();
    }




  // http://en.wikipedia.org/wiki/Initialization_vector
  private void SetUpInitVector()
    {
    byte[] InitVector = new byte[16];

    for( int Count = 0; Count < 16; Count++ )
      InitVector[Count] = (byte)Count;

    // Give it a starting PreviousBlock.
    // If you don't know the key, how can you figure
    // out what's in the PreviousBlock?
    EncryptBlock( InitVector, PreviousBlock );
    }




  private void SetUpInitVectorForHash()
    {
    byte[] InitVector = new byte[16];

    for( int Count = 0; Count < 16; Count++ )
      InitVector[Count] = (byte)Count;

    // Give it a starting PreviousBlock.
    // If you don't know the key, how can you figure
    // out what's in the PreviousBlock?
    EncryptBlock( InitVector, PreviousBlock );
    }




  internal bool CFBEncrypt( byte[] InBuffer, byte[] OutBuffer, int HowMany  )
    {
    if( HowMany > InBuffer.Length )
      return false;

    if( (HowMany + 16) > OutBuffer.Length )
      return false;

    SetUpInitVector();


    int BytePos = 0;
    for( int Count = 0; Count < HowMany; Count++ )
      {
      PlainBlock[BytePos] = InBuffer[Count];
      OutBuffer[Count + 16] = (byte)(InBuffer[Count] ^
                     PreviousBlock[BytePos]);
      BytePos++;
      if( BytePos > 15 )
        {
        EncryptBlock( PlainBlock, PreviousBlock );
        BytePos = 0;
        }
      }

    return true;
    }



  internal bool CFBDecrypt( byte[] InBuffer, byte[] OutBuffer, int HowMany )
    {
    int Test = 1;
    try
    {
    if( HowMany > InBuffer.Length )
      return false;

    if( (HowMany - 16) > OutBuffer.Length )
      return false;

    SetUpInitVector();
    Test = 2;

    for( int Count = 0; Count < 16; Count++ )
      {
      byte ThrowAway = (byte)(InBuffer[Count] ^ PreviousBlock[Count]);
      PlainBlock[Count] = ThrowAway;
      }

    Test = 3;
    EncryptBlock( PlainBlock, PreviousBlock );

    Test = 4;

    int BytePos = 0;
    for( int Count = 0; Count < (HowMany - 16); Count++ )
      {
      OutBuffer[Count] = (byte)(InBuffer[Count + 16] ^ PreviousBlock[BytePos]);
      PlainBlock[BytePos] = OutBuffer[Count];
      BytePos++;
      if( BytePos > 15 )
        {
        // The code does EncryptBlock and not
        // DecryptBlock here.  Doing the exclusive or on it
        // again puts it back like it was.
        EncryptBlock( PlainBlock, PreviousBlock );
        BytePos = 0;
        }
      }

    return true;

    }
    catch( Exception Except )
      {
      MessageBox.Show( "Error in CFBDecrypt. Test is: " + Test.ToString() + "\r\n" + Except.Message, MainForm.MessageBoxTitle, MessageBoxButtons.OK, MessageBoxIcon.Stop );
      return false;
      }
    }




  internal string EncryptString( string InString )
    {
    byte[] StrBytes = UTF8Strings.StringToBytes( InString );
    if( StrBytes == null )
      return "";

    byte[] OutBytes = new byte[StrBytes.Length + 16];

    CFBEncrypt( StrBytes, OutBytes, StrBytes.Length  );
    return BytesToLetterString( OutBytes );
    }




  internal string DecryptString( string InString )
    {
    int Test = 1;
    try
    {
    byte[] StrBytes = LetterStringToBytes( InString );
    if( StrBytes == null )
      return "";

    byte[] OutBytes = new byte[StrBytes.Length - 16];

    Test = 2;
    CFBDecrypt( StrBytes, OutBytes, StrBytes.Length );

    Test = 3;
    return UTF8Strings.BytesToString( OutBytes, OutBytes.Length );
    }
    catch( Exception Except )
      {
      MessageBox.Show( "Error in AESEncryption DecryptString. Test is: " + Test.ToString() + "\r\n" + Except.Message, MainForm.MessageBoxTitle, MessageBoxButtons.OK, MessageBoxIcon.Stop );
      return "";
      }
    }



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



  ///////////
  internal void EncryptTestBlock()
    {
    ShowStatus( " " );
    ShowStatus( "Encrypt Test Block:" );
    ShowStatus( " " );

    EncryptBlock( TestBlock, OutBlock );

    ShowStatus( " " );
    ShowStatus( " " );
    }
    //////////



*/
