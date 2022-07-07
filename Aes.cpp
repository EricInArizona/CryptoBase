// Copyright Eric Chauvin 2022.



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html



#include "Aes.h"



// This is only partially done.
// Still translating from my old C# code.


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

  // Column-major order.
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




  private void AddRoundKey( int Round )
    {
    // StateArray[Row, Column]
    StateArray[0, 0] = (byte)(StateArray[0, 0] ^
                 KeyScheduleBytes[0, Round * 4]);
    StateArray[1, 0] = (byte)(StateArray[1, 0] ^ KeyScheduleBytes[1, Round * 4]);
    StateArray[2, 0] = (byte)(StateArray[2, 0] ^ KeyScheduleBytes[2, Round * 4]);
    StateArray[3, 0] = (byte)(StateArray[3, 0] ^ KeyScheduleBytes[3, Round * 4]);

    StateArray[0, 1] = (byte)(StateArray[0, 1] ^ KeyScheduleBytes[0, (Round * 4) + 1]);
    StateArray[1, 1] = (byte)(StateArray[1, 1] ^ KeyScheduleBytes[1, (Round * 4) + 1]);
    StateArray[2, 1] = (byte)(StateArray[2, 1] ^ KeyScheduleBytes[2, (Round * 4) + 1]);
    StateArray[3, 1] = (byte)(StateArray[3, 1] ^ KeyScheduleBytes[3, (Round * 4) + 1]);

    StateArray[0, 2] = (byte)(StateArray[0, 2] ^ KeyScheduleBytes[0, (Round * 4) + 2]);
    StateArray[1, 2] = (byte)(StateArray[1, 2] ^ KeyScheduleBytes[1, (Round * 4) + 2]);
    StateArray[2, 2] = (byte)(StateArray[2, 2] ^ KeyScheduleBytes[2, (Round * 4) + 2]);
    StateArray[3, 2] = (byte)(StateArray[3, 2] ^ KeyScheduleBytes[3, (Round * 4) + 2]);

    StateArray[0, 3] = (byte)(StateArray[0, 3] ^ KeyScheduleBytes[0, (Round * 4) + 3]);
    StateArray[1, 3] = (byte)(StateArray[1, 3] ^ KeyScheduleBytes[1, (Round * 4) + 3]);
    StateArray[2, 3] = (byte)(StateArray[2, 3] ^ KeyScheduleBytes[2, (Round * 4) + 3]);
    StateArray[3, 3] = (byte)(StateArray[3, 3] ^ KeyScheduleBytes[3, (Round * 4) + 3]);
    }



  // This is meant to be used to compare with the test vectors
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



  private void SubBytes()
    {
    // Even though you could do it like this:
    // StateArray[0, 0] = SBox[StateArray[0, 0]];
    // It can be a lot faster to call the SubByte() function instead
    // because it allows the compiler to optimize it.
    // For example the compiler can figure out the range-checking
    // in an easier way when it is inside the SubByte() function
    // by itself.

    StateArray[0, 0] = SubByte( StateArray[0, 0] );
    StateArray[1, 0] = SubByte( StateArray[1, 0] );
    StateArray[2, 0] = SubByte( StateArray[2, 0] );
    StateArray[3, 0] = SubByte( StateArray[3, 0] );
    StateArray[0, 1] = SubByte( StateArray[0, 1] );
    StateArray[1, 1] = SubByte( StateArray[1, 1] );
    StateArray[2, 1] = SubByte( StateArray[2, 1] );
    StateArray[3, 1] = SubByte( StateArray[3, 1] );
    StateArray[0, 2] = SubByte( StateArray[0, 2] );
    StateArray[1, 2] = SubByte( StateArray[1, 2] );
    StateArray[2, 2] = SubByte( StateArray[2, 2] );
    StateArray[3, 2] = SubByte( StateArray[3, 2] );
    StateArray[0, 3] = SubByte( StateArray[0, 3] );
    StateArray[1, 3] = SubByte( StateArray[1, 3] );
    StateArray[2, 3] = SubByte( StateArray[2, 3] );
    StateArray[3, 3] = SubByte( StateArray[3, 3] );
    }



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



  // From: http://en.wikipedia.org/wiki/Rijndael_mix_columns
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
    TempStateArray[0, 0] = (byte)(GaloisMultiply( 0x02, StateArray[0, Const0]) ^ GaloisMultiply( 0x03, StateArray[1, Const0]) ^ StateArray[2, Const0] ^ StateArray[3, Const0]);
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

    // I added this part that makes a randomish throw-away block
    // for the first block.
    ulong Ticks = (ulong)DateTime.Now.Ticks;
    PlainBlock[0] = (byte)(Ticks & 0xFF);
    PlainBlock[1] = (byte)((Ticks >> 8) & 0xFF);
    PlainBlock[2] = (byte)((Ticks >> 16) & 0xFF);
    PlainBlock[3] = (byte)((Ticks >> 24) & 0xFF);
    PlainBlock[4] = (byte)((Ticks >> 32) & 0xFF);
    PlainBlock[5] = (byte)((Ticks >> 40) & 0xFF);
    PlainBlock[6] = (byte)((Ticks >> 48) & 0xFF);
    PlainBlock[7] = (byte)(Sequence & 0xFF);
    PlainBlock[8] = (byte)((Sequence >> 8) & 0xFF);
    PlainBlock[9] = (byte)((Sequence >> 16) & 0xFF);
    PlainBlock[10] = (byte)((Sequence >> 24) & 0xFF);
    PlainBlock[11] = PlainBlock[0];
    PlainBlock[12] = PlainBlock[1];
    PlainBlock[13] = PlainBlock[2];
    PlainBlock[14] = PlainBlock[3];
    PlainBlock[15] = PlainBlock[4];

    Sequence++;

    for( int Count = 0; Count < 16; Count++ )
      OutBuffer[Count] = (byte)(PlainBlock[Count] ^ PreviousBlock[Count]);

    EncryptBlock( PlainBlock, PreviousBlock );


    // This part of this function (the for-loop below) was
    // originally from the Diamond algorithm by Michael Paul
    // Johnson.  It came from his Master of Science thesis in
    // Electrical Engineering, in 1989.  He says it is "hereby
    // dedicated to the Public Domain by the author and
    // inventor, Michael Paul Johnson."

    // But I've modified to work with the randomish block that
    // starts this.

    int BytePos = 0;
    for( int Count = 0; Count < HowMany; Count++ )
      {
      PlainBlock[BytePos] = InBuffer[Count];
      OutBuffer[Count + 16] = (byte)(InBuffer[Count] ^ PreviousBlock[BytePos]);
      BytePos++;
      if( BytePos > 15 )
        {
        EncryptBlock( PlainBlock, PreviousBlock );
        BytePos = 0;
        }
      }

    return true;
    }




  internal string GetHash( byte[] InBuffer, int HowMany  )
    {
    if( HowMany > InBuffer.Length )
      return "";

    SetUpInitVectorForHash(); // Sets the first PreviousBlock.

    int BytePos = 0;
    for( int Count = 0; Count < HowMany; Count++ )
      {
      PlainBlock[BytePos] = InBuffer[Count];
      // OutBuffer[Count + 16] = (byte)(InBuffer[Count] ^ PreviousBlock[BytePos]);
      BytePos++;
      if( BytePos > 15 )
        {
        EncryptBlock( PlainBlock, PreviousBlock );
        BytePos = 0;
        }
      }

    return BytesToLetterString( PreviousBlock );
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




  internal string BytesToLetterString( byte[] InBytes )
    {
    StringBuilder SBuilder = new StringBuilder();
    for( int Count = 0; Count < InBytes.Length; Count++ )
      {
      uint ByteHigh = InBytes[Count];
      uint ByteLow = ByteHigh & 0x0F;
      ByteHigh >>= 4;
      SBuilder.Append( (char)('A' + (char)ByteHigh) );
      SBuilder.Append( (char)('A' + (char)ByteLow) );
      // MForm.ShowStatus( SBuilder.ToString() );
      }

    return SBuilder.ToString();
    }






  private bool IsInLetterRange( uint Letter )
    {
    const uint MaxLetter = (uint)('A') + 15;
    const uint MinLetter = (uint)'A';

    if( Letter > MaxLetter )
      {
      // MForm.ShowStatus( "Letter > MaxLetter" );
      return false;
      }

    if( Letter < MinLetter )
      {
      // MForm.ShowStatus( "Letter < MinLetter" );
      return false;
      }

    return true;
    }




  internal byte[] LetterStringToBytes( string InString )
    {
    if( InString == null )
      return null;

    if( InString.Length < 2 )
      return null;

    byte[] OutBytes;

    try
    {
    OutBytes = new byte[InString.Length >> 1];
    }
    catch( Exception )
      {
      return null;
      }

    int Where = 0;
    for( int Count = 0; Count < OutBytes.Length; Count++ )
      {
      uint Letter = InString[Where];
      if( !IsInLetterRange( Letter ))
        return null;

      uint ByteHigh = Letter - (uint)'A';
      ByteHigh <<= 4;
      Where++;
      Letter = InString[Where];
      if( !IsInLetterRange( Letter ))
        return null;

      uint ByteLow = Letter - (uint)'A';
      Where++;

      OutBytes[Count] = (byte)(ByteHigh | ByteLow);
      }

    return OutBytes;
    }



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


*/
