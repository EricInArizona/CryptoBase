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
// For the 256 bit keys.
// 8 = AesConst::KeyLengthInBytes256 / 4

for( Int32 count = 0; count < 8; count++ )
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

// First four bytes of the key: 0x60, 0x3d,
// 0xeb, 0x10.
// w0 = 603deb10

// 8 = AesConst::KeyLengthInBytes256 / 4

for( Int32 count = 8;
                count < AesConst::KeyWordsSize;
                count++ )
  {
  Uint32 temp = aesKeyWords.getV( count - 1 );

  // if( (count % 8) == 0)
  if( (count & 0x07) == 0)
    {
    // See Appendix A.3 Expansion of a
    // 256-bit Cipher Key.
    temp = rotateWord( temp );
    temp = subWord( temp );
    Uint32 rConVal = aesRCon.getV( count / 8 );

    rConVal <<= 24;
    temp = temp ^ rConVal;
    }

  // if( (Count % 8) == 4)
  // else if( 8 > 6 and i mod Nk = 4)
  if( (count & 0x07) == 4)
    {
    temp = subWord( temp );
    }

  Uint32 prevWord = aesKeyWords.getV( count - 8 );
  aesKeyWords.setV( count, prevWord ^ temp );
  }

moveKeyScheduleWordsToBytes();
}



void Aes::moveKeyScheduleWordsToBytes( void )
{
for( Int32 count = 0; count <
                 AesConst::KeyWordsSize; count++ )
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



/*
  private void ShiftRows()
    {
    // The first row is unchanged.
    // StateArray[Row, Column]
    byte Temp = StateArray[1, 0];
    StateArray[1, 0] = StateArray[1, 1];
    StateArray[1, 1] = StateArray[1, 2];
    StateArray[1, 2] = StateArray[1, 3];
    StateArray[1, 3] = Temp;

    Temp = StateArray[2, 0];
    byte Temp2 = StateArray[2, 1];
    StateArray[2, 0] = StateArray[2, 2];
    StateArray[2, 1] = StateArray[2, 3];
    StateArray[2, 2] = Temp;
    StateArray[2, 3] = Temp2;

    Temp = StateArray[3, 0];
    Temp2 = StateArray[3, 1];
    byte Temp3 = StateArray[3, 2];
    StateArray[3, 0] = StateArray[3, 3];
    StateArray[3, 1] = Temp;
    StateArray[3, 2] = Temp2;
    StateArray[3, 3] = Temp3;
    }



  private byte GaloisMultiply( byte A, byte B )
    {
    byte Product = 0;
    for( int Count = 0; Count < 8; Count++ )
      {
      if( (B & 1) != 0 )
        Product ^= A;

      // Get the high bit before shifting it.
      int HighBit = A & 0x80;
      A <<= 1;
      if( HighBit != 0 )
        A ^= 0x1B; // x^8 + x^4 + x^3 + x + 1

      B >>= 1;
      }

    return Product;
    }




  private void MixColumns()
    {
    TempStateArray[0, 0] = (byte)(GaloisMultiply(
            0x02, StateArray[0, Const0]) ^
            GaloisMultiply( 0x03, StateArray[1,
        Const0]) ^ StateArray[2, Const0] ^
        StateArray[3, Const0]);


    TempStateArray[1, 0] = (byte)(StateArray[0, Const0] ^ GaloisMultiply( 0x02, StateArray[1, Const0]) ^ GaloisMultiply( 0x03, StateArray[2, Const0]) ^ StateArray[3, Const0]);
    TempStateArray[2, 0] = (byte)(StateArray[0, Const0] ^ StateArray[1, Const0] ^ GaloisMultiply( 0x02, StateArray[2, Const0]) ^ GaloisMultiply( 0x03, StateArray[3, Const0]));
    TempStateArray[3, 0] = (byte)(GaloisMultiply( 0x03, StateArray[0, Const0]) ^ StateArray[1, Const0] ^ StateArray[2, Const0] ^ GaloisMultiply( 0x02, StateArray[3, Const0]));

    TempStateArray[0, 1] = (byte)(GaloisMultiply( 0x02, StateArray[0, Const1]) ^ GaloisMultiply( 0x03, StateArray[1, Const1]) ^ StateArray[2, Const1] ^ StateArray[3, Const1]);
    TempStateArray[1, 1] = (byte)(StateArray[0, Const1] ^ GaloisMultiply( 0x02, StateArray[1, Const1]) ^ GaloisMultiply( 0x03, StateArray[2, Const1]) ^ StateArray[3, Const1]);
    TempStateArray[2, 1] = (byte)(StateArray[0, Const1] ^ StateArray[1, Const1] ^ GaloisMultiply( 0x02, StateArray[2, Const1]) ^ GaloisMultiply( 0x03, StateArray[3, Const1]));
    TempStateArray[3, 1] = (byte)(GaloisMultiply( 0x03, StateArray[0, Const1]) ^ StateArray[1, Const1] ^ StateArray[2, Const1] ^ GaloisMultiply( 0x02, StateArray[3, Const1]));

    TempStateArray[0, 2] = (byte)(GaloisMultiply( 0x02, StateArray[0, Const2]) ^ GaloisMultiply( 0x03, StateArray[1, Const2]) ^ StateArray[2, Const2] ^ StateArray[3, Const2]);
    TempStateArray[1, 2] = (byte)(StateArray[0, Const2] ^ GaloisMultiply( 0x02, StateArray[1, Const2]) ^ GaloisMultiply( 0x03, StateArray[2, Const2]) ^ StateArray[3, Const2]);
    TempStateArray[2, 2] = (byte)(StateArray[0, Const2] ^ StateArray[1, Const2] ^ GaloisMultiply( 0x02, StateArray[2, Const2]) ^ GaloisMultiply( 0x03, StateArray[3, Const2]));
    TempStateArray[3, 2] = (byte)(GaloisMultiply( 0x03, StateArray[0, Const2]) ^ StateArray[1, Const2] ^ StateArray[2, Const2] ^ GaloisMultiply( 0x02, StateArray[3, Const2]));

    TempStateArray[0, 3] = (byte)(GaloisMultiply( 0x02, StateArray[0, Const3]) ^ GaloisMultiply( 0x03, StateArray[1, Const3]) ^ StateArray[2, Const3] ^ StateArray[3, Const3]);
    TempStateArray[1, 3] = (byte)(StateArray[0, Const3] ^ GaloisMultiply( 0x02, StateArray[1, Const3]) ^ GaloisMultiply( 0x03, StateArray[2, Const3]) ^ StateArray[3, Const3]);
    TempStateArray[2, 3] = (byte)(StateArray[0, Const3] ^ StateArray[1, Const3] ^ GaloisMultiply( 0x02, StateArray[2, Const3]) ^ GaloisMultiply( 0x03, StateArray[3, Const3]));
    TempStateArray[3, 3] = (byte)(GaloisMultiply( 0x03, StateArray[0, Const3]) ^ StateArray[1, Const3] ^ StateArray[2, Const3] ^ GaloisMultiply( 0x02, StateArray[3, Const3]));

    CopyTempStateToState();
    }




  private void CopyTempStateToState()
    {
    // The compiler can optimize this when it's done as
    // a separate function.
    StateArray[0, 0] = TempStateArray[0, 0];
    StateArray[1, 0] = TempStateArray[1, 0];
    StateArray[2, 0] = TempStateArray[2, 0];
    StateArray[3, 0] = TempStateArray[3, 0];
    StateArray[0, 1] = TempStateArray[0, 1];
    StateArray[1, 1] = TempStateArray[1, 1];
    StateArray[2, 1] = TempStateArray[2, 1];
    StateArray[3, 1] = TempStateArray[3, 1];
    StateArray[0, 2] = TempStateArray[0, 2];
    StateArray[1, 2] = TempStateArray[1, 2];
    StateArray[2, 2] = TempStateArray[2, 2];
    StateArray[3, 2] = TempStateArray[3, 2];
    StateArray[0, 3] = TempStateArray[0, 3];
    StateArray[1, 3] = TempStateArray[1, 3];
    StateArray[2, 3] = TempStateArray[2, 3];
    StateArray[3, 3] = TempStateArray[3, 3];
    }



  // You'll see below in the CFBDecrypt() function why a
  // DecryptBlock() function is never defined in this code.

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

    for( int Round = 1; Round < NumberOfRounds; Round++ )
      {
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

    // The big weakness in this implementation:
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
