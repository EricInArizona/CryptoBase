// Copyright Eric Chauvin 2022.



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html



#include "Aes.h"


// From my C# code.

/*
Aes::Aes( void )
{

}
*/


/*
  private const int NumberOfRounds = 14;
  private const int KeyLengthInBytes = 32; // 256 / 8
  private const int Const0 = 0;
  private const int Const1 = 1;
  private const int Const2 = 2;
  private const int Const3 = 3;
  private ulong Sequence = 0;

  // This corresponds to a 128 bit block.
  // State as in "state machine".
  private byte[,] StateArray = new byte[4, 4];
  private byte[,] TempStateArray = new byte[4, 4];

  // Only the first few of these are actually used.
  // Round Constant Array.
  private byte[] Rcon = new byte[256]
    {
    0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a,
    0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39,
    0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a,
    0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8,
    0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef,
    0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc,
    0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b,
    0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3,
    0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94,
    0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20,
    0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35,
    0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f,
    0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04,
    0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63,
    0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd,
    0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d
    };


    // See the SubBytes() function below.
    private byte[] SBox = new byte[256]
      {
      // 0     1     2    3     4     5     6      7    8      9    a     b     c     d      e     f
      0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76, // 0
      0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0, // 1
      0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15, // 2
      0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75, // 3
      0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84, // 4
      0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf, // 5
      0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8, // 6
      0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2, // 7
      0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73, // 8
      0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb, // 9
      0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79, // a
      0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08, // b
      0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a, // c
      0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e, // d
      0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf, // e
      0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16  // f
      };


    // This key is from the FIPS document
    // Appendix A - Key Expansion Examples.
    ///////////
    // Key for the Key Expansion example:
    byte[] Key = new byte[KeyLengthInBytes]
      {
      0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe,
      0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81,
      0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7,
      0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4
      };
      ////////

    // Key for the test vector example in Appendix C.3.
    byte[] Key = new byte[KeyLengthInBytes]
      {
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
      0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
      0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
      0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f
      };


    // First four bytes of the key: 0x60, 0x3d, 0xeb, 0x10.
    // Expressed as a 32 bit word:
    // w0 = 603deb10

    // The key schedule generates Nb(Nr + 1) words.
    // That is 4 times 15 words.
    uint[] KeyScheduleWords = new uint[4 * (NumberOfRounds + 1)];
    byte[,] KeyScheduleBytes = new byte[4, 4 * (NumberOfRounds + 1)];


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




  internal AESEncryption()
    {
    // MForm = UseForm;

    Sequence = (ulong)DateTime.Now.Ticks;
    }

  /////
  internal AESEncryption( MainForm UseForm )
    {
    MForm = UseForm;

    KeyExpansion();

    Sequence = (ulong)DateTime.Now.Ticks;
    }
    ////////



  // This is called RotWord() in the FIPS standard document.
  private uint RotateWord( uint InWord )
    {
    uint HighByte = InWord >> 24;
    InWord <<= 8;
    InWord |= HighByte;
    return InWord;
    }



  private byte SubByte( byte InByte )
    {
    // Jave doesn't have unsigned values so you'd have to do
    // some tricks with the top bit in a signed byte here.
    // int Index = InByte & 0x7F;
    // if( (InByte & 0x80) == 0x80 )
    //   Index |= 0x80;
    // return SBox[Index];
    return SBox[InByte];
    }




  private uint SubWord( uint InWord )
    {
    uint OutWord = SubByte( (byte)((InWord >> 24) & 0xFF) );
    OutWord <<= 8;
    OutWord |= SubByte( (byte)((InWord >> 16) & 0xFF) );
    OutWord <<= 8;
    OutWord |= SubByte( (byte)((InWord >> 8) & 0xFF) );
    OutWord <<= 8;
    OutWord |= SubByte( (byte)(InWord & 0xFF) );

    return OutWord;
    }




  private void MoveInBlockToState( byte[] InBlock )
    {
    // Column-major order is Fortran-style for people in academia.

    // "AES operates on a 44 column-major order
    // matrix of bytes."
    // http://en.wikipedia.org/wiki/Advanced_Encryption_Standard
    // http://en.wikipedia.org/wiki/Row-major_order

    // Section 3.5:
    // If the state was in 32 bit word form it would be like:
    // W0 = InBlock[0];
    // W0 <<= 8;
    // W0 |= InBlock[1];
    // W0 <<= 8;
    // W0 |= InBlock[2];
    // W0 <<= 8;
    // W0 |= InBlock[3];

    // And so on for W1, W2 and W3.

    // Column-major order:
    // for( int Column = 0; Column < 4; Column++ )
      // {
      // for( int Row = 0; Row < 4; Row++ )
      //   StateArray[Row, Column] = InBlock[Where++];

      // }

    // Since this uses constants for the indexes the compiler
    // can do compile-time range checks and optimizations.
    // You don't have run-time range-checks being done.

    // StateArray[Row, Column]
    StateArray[0, 0] = InBlock[0];
    StateArray[1, 0] = InBlock[1];
    StateArray[2, 0] = InBlock[2];
    StateArray[3, 0] = InBlock[3];
    StateArray[0, 1] = InBlock[4];
    StateArray[1, 1] = InBlock[5];
    StateArray[2, 1] = InBlock[6];
    StateArray[3, 1] = InBlock[7];
    StateArray[0, 2] = InBlock[8];
    StateArray[1, 2] = InBlock[9];
    StateArray[2, 2] = InBlock[10];
    StateArray[3, 2] = InBlock[11];
    StateArray[0, 3] = InBlock[12];
    StateArray[1, 3] = InBlock[13];
    StateArray[2, 3] = InBlock[14];
    StateArray[3, 3] = InBlock[15];
    }




  private void MoveStateToOutBlock( byte[] OutBlock )
    {
    // StateArray[Row, Column]
    OutBlock[0] = StateArray[0, 0];
    OutBlock[1] = StateArray[1, 0];
    OutBlock[2] = StateArray[2, 0];
    OutBlock[3] = StateArray[3, 0];
    OutBlock[4] = StateArray[0, 1];
    OutBlock[5] = StateArray[1, 1];
    OutBlock[6] = StateArray[2, 1];
    OutBlock[7] = StateArray[3, 1];
    OutBlock[8] = StateArray[0, 2];
    OutBlock[9] = StateArray[1, 2];
    OutBlock[10] = StateArray[2, 2];
    OutBlock[11] = StateArray[3, 2];
    OutBlock[12] = StateArray[0, 3];
    OutBlock[13] = StateArray[1, 3];
    OutBlock[14] = StateArray[2, 3];
    OutBlock[15] = StateArray[3, 3];
    }




  private uint MakeUintFromBytes( byte Byte0, byte Byte1, byte Byte2, byte Byte3 )
    {
    // A uint (32 bit word) corresponds to one column in a
    // column-major-order matrix.
    // See the MoveInBlockToState() function  for more comments
    // on why it's in this order.

    // Appendix A - Key Expansion Examples.
    // Also see section 3.5.
    uint Result = Byte0;
    Result <<= 8;
    Result |= Byte1;
    Result <<= 8;
    Result |= Byte2;
    Result <<= 8;
    Result |= Byte3;

    return Result;
    }




  internal void KeyExpansion()
    {
    // int RConTest = Rcon[0];
    // ShowStatus( "RConTest 0: " + RConTest.ToString()); //  "X8" ));
    for( int Count = 0; Count < 8; Count++ )
      {
      // The four bytes: 0x60, 0x3d, 0xeb, 0x10,
      // Become the uint: w0 = 603deb10
      KeyScheduleWords[Count] = MakeUintFromBytes( Key[4 * Count], Key[(4 * Count) + 1], Key[(4 * Count) + 2], Key[(4 * Count) + 3] );
      // int SignedInt = (int)KeyScheduleWords[Count];
      // ShowStatus( Count.ToString() + ") " + SignedInt.ToString()); //  "X8" ));
      }

    // First four bytes of the key: 0x60, 0x3d, 0xeb, 0x10.
    // w0 = 603deb10
    // w1 = 15ca71be
    // w2 = 2b73aef0
    // w3 = 857d7781
    // w4 = 1f352c07
    // w5 = 3b6108d7
    // w6 = 2d9810a3
    // w7 = 0914dff4

    for( int Count = 8; Count < (4 * (14 + 1)); Count++ )
      {
      uint Temp = KeyScheduleWords[Count - 1];
      // ShowStatus( "Temp: " + Temp.ToString( "X8" ));

      // Mod 8 is the same as AND 0x07 (to leave the bottom 3 bits)
      // but the AND is a lot faster.
      // if( (Count % 8) == 0)
      if( (Count & 0x07) == 0)
        {
        // See Appendix A.3 Expansion of a 256-bit Cipher Key.
        // int SignedInt4 = (int)Temp;
        // ShowStatus( "Temp before Rotate: " + SignedInt4.ToString());
        Temp = RotateWord( Temp );
        // int SignedInt4 = (int)Temp;
        // ShowStatus( "Temp after Rotate: " + SignedInt4.ToString());
        Temp = SubWord( Temp );
        // SignedInt4 = (int)Temp;
        // ShowStatus( "Temp after SubWord: " + SignedInt4.ToString());
        uint RConVal = Rcon[Count / 8];

        RConVal <<= 24; // The zero byte is on the left side of the word.  Hmmm.
        // SignedInt4 = (int)RConVal;
        // ShowStatus( "RConVal: " + SignedInt4.ToString());

        Temp = Temp ^ RConVal;
        // int SignedInt = (int)Temp;
        // ShowStatus( "Temp after Rcon: " + SignedInt.ToString());
        }

      // if( (Count % 8) == 4) // else if( 8 > 6 and i mod Nk = 4)
      if( (Count & 0x07) == 4)
        {
        Temp = SubWord( Temp );
        // ShowStatus( "Temp after Subword part 2: " + Temp.ToString( "X8" ));
        // int SignedInt3 = (int)Temp;
        // ShowStatus( "Temp after SubWord: " + SignedInt3.ToString());
        }

      KeyScheduleWords[Count] = KeyScheduleWords[Count - 8] ^ Temp;
      // ShowStatus( Count.ToString() + ") " + KeyScheduleWords[Count].ToString( "X8" ));
      // int SignedInt2 = (int)KeyScheduleWords[Count];
      // ShowStatus( Count.ToString() + ") " + SignedInt2.ToString()); //  "X8" ));
      }

    MoveKeyScheduleWordsToBytes();
    }




  private void MoveKeyScheduleWordsToBytes()
    {
    for( int Count = 0; Count < KeyScheduleWords.Length; Count++ )
      {
      uint AWord = KeyScheduleWords[Count];

      // Column-major order.
      // Compare this to what's in MoveInBlockToState().
      //               Row, Column
      KeyScheduleBytes[0, Count] = (byte)((AWord >> 24) & 0xFF);
      KeyScheduleBytes[1, Count] = (byte)((AWord >> 16) & 0xFF);
      KeyScheduleBytes[2, Count] = (byte)((AWord >> 8) & 0xFF);
      KeyScheduleBytes[3, Count] = (byte)(AWord & 0xFF);
      }
    }




  private void AddRoundKey( int Round )
    {
    // StateArray[Row, Column]
    StateArray[0, 0] = (byte)(StateArray[0, 0] ^ KeyScheduleBytes[0, Round * 4]);
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
*/
