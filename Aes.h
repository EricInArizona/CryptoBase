// Copyright Eric Chauvin 2022.



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html



// Make it work first, then make it work fast.



#pragma once



// There are Intel and AMD AES-NI instruction
// set extensions.  Those would be _much_ faster
// than this.
// And see the CLMUL instruction set for
// things like Galois Counter Mode.


#include "../CppBase/BasicTypes.h"
#include "../CppBase/Casting.h"
#include "../CppBase/CharBuf.h"
#include "../CppBase/StIO.h"


// None of these Aes #include files make their
// own separate compilation unit.  Only Aes.cpp
// makes it's own compilation unit.
// Use the compiler option to generate assembly
// language output and see what Aes.cpp produces.

#include "AesState.h"
#include "AesSBox.h"
#include "AesRCon.h"
#include "AesKeyWords.h"
#include "AesKeyBytes.h"
#include "AesKey.h"



class Aes
  {
  private:
  bool testForCopy = false;
  AesState aesState;
  AesSBox aesSBox;
  AesRCon aesRCon;
  AesKey aesKey;
  AesKeyWords aesKeyWords;
  AesKeyBytes aesKeyBytes;

  // AES has a fixed block size of 128 bits.
  // Nb = Number of columns, 32 bit words, in
  // the state.  It is always 4.
  // Nk = Number of 32 bit words in the key.
  // It is 4, 6 or 8.
  // Nr = Number of rounds.


  // 10 rounds for 128 bit key, 12 rounds for 192,
  // 14 rounds for 256 bit key.
  Int32 numberOfRounds = 14;
  Int32 keyLengthInBytes = 32; // 256 / 8
  Int32 keyWordsSize = 4 * (numberOfRounds + 1);



  // This is called RotWord() in the FIPS
  // standard document.
  inline Uint32 rotateWord( const Uint32 inWord )
                                             const
    {
    Uint32 outWord = (inWord << 8) |
                     (inWord >> 24); // 32 - 8
    return outWord;
    }


  inline Uint8 subByte( const Uint8 inByte ) const
    {
    return aesSBox.getV( inByte );
    }


  inline Uint32 subWord( const Uint32 inWord ) const
    {
    Uint32 outWord = subByte(
                       (inWord >> 24) & 0xFF );

    outWord <<= 8;
    outWord |= subByte( (inWord >> 16) & 0xFF );
    outWord <<= 8;
    outWord |= subByte( (inWord >> 8) & 0xFF );
    outWord <<= 8;
    outWord |= subByte( inWord & 0xFF );

    return outWord;
    }


  // This galoisMultiply() could use optimization.
  // It's a loop within a loop.
  // It's also very interesting and worth
  // exploring this more.

  // Some expressions with constant values
  // like this galoisMultiply( 0x02, x )
  // can be optimized in a table.
  // There are only a few of those constant
  // values.

  inline Uint8 galoisMultiply( Uint8 A, Uint8 B )
    {
    // Multiplication in the field GF2^8.
    // The bits are the coefficients of a
    // polynomial.
    // Multiplication is mod an irreducible
    // polynomial x^8 + x^4 + x^3 + x + 1.

    Uint8 product = 0;
    for( Int32 count = 0; count < 8; count++ )
      {
      if( (B & 1) != 0 )
        product ^= A; // Addition.

      // Get the high bit before shifting it off.
      Uint8 highBit = A & 0x80;
      A <<= 1;
      if( highBit != 0 )
        A ^= 0x1B; // x^8 + x^4 + x^3 + x + 1

      B >>= 1;
      }

    return product;
    }


  inline static Uint32 makeUintFromBytes(
                          const Uint8 byte0,
                          const Uint8 byte1,
                          const Uint8 byte2,
                          const Uint8 byte3 )
    {
    // Appendix A - Key Expansion Examples.
    // Also see section 3.5.

    Uint32 result = byte0;
    result <<= 8;
    result |= byte1;
    result <<= 8;
    result |= byte2;
    result <<= 8;
    result |= byte3;

    return result;
    }

  void keyExpansion( void );

  inline void moveCharBufToState(
                           const CharBuf& buf,
                           const Int32 where )
    {
    const Int32 last = buf.getLast();
    if( (where + 15) >= last )
      throw "Aes moveCharBufToState range.";

    // Section 3.5:
    // Big endian.
    // W0 = InBlock[0];
    // W0 <<= 8;
    // W0 |= InBlock[1];
    // W0 <<= 8;
    // W0 |= InBlock[2];
    // W0 <<= 8;
    // W0 |= InBlock[3];

    // And so on for W1, W2 and W3.

    // StateArray[Row, Column]
    aesState.setV( 0, 0, buf.getU8( where ));
    aesState.setV( 1, 0, buf.getU8( where + 1 ));
    aesState.setV( 2, 0, buf.getU8( where + 2 ));
    aesState.setV( 3, 0, buf.getU8( where + 3 ));
    aesState.setV( 0, 1, buf.getU8( where + 4 ));
    aesState.setV( 1, 1, buf.getU8( where + 5 ));
    aesState.setV( 2, 1, buf.getU8( where + 6 ));
    aesState.setV( 3, 1, buf.getU8( where + 7 ));
    aesState.setV( 0, 2, buf.getU8( where + 8 ));
    aesState.setV( 1, 2, buf.getU8( where + 9 ));
    aesState.setV( 2, 2, buf.getU8( where + 10 ));
    aesState.setV( 3, 2, buf.getU8( where + 11 ));
    aesState.setV( 0, 3, buf.getU8( where + 12 ));
    aesState.setV( 1, 3, buf.getU8( where + 13 ));
    aesState.setV( 2, 3, buf.getU8( where + 14 ));
    aesState.setV( 3, 3, buf.getU8( where + 15 ));
    }


  inline void moveStateToCharBuf( CharBuf& buf )
    {
    buf.appendU8( aesState.getV( 0, 0 ), 1024 );
    buf.appendU8( aesState.getV( 1, 0 ), 1024 );
    buf.appendU8( aesState.getV( 2, 0 ), 1024 );
    buf.appendU8( aesState.getV( 3, 0 ), 1024 );
    buf.appendU8( aesState.getV( 0, 1 ), 1024 );
    buf.appendU8( aesState.getV( 1, 1 ), 1024 );
    buf.appendU8( aesState.getV( 2, 1 ), 1024 );
    buf.appendU8( aesState.getV( 3, 1 ), 1024 );
    buf.appendU8( aesState.getV( 0, 2 ), 1024 );
    buf.appendU8( aesState.getV( 1, 2 ), 1024 );
    buf.appendU8( aesState.getV( 2, 2 ), 1024 );
    buf.appendU8( aesState.getV( 3, 2 ), 1024 );
    buf.appendU8( aesState.getV( 0, 3 ), 1024 );
    buf.appendU8( aesState.getV( 1, 3 ), 1024 );
    buf.appendU8( aesState.getV( 2, 3 ), 1024 );
    buf.appendU8( aesState.getV( 3, 3 ), 1024 );
    }

  void moveKeyScheduleWordsToBytes( void );
  void addRoundKey( Int32 round );
  void subBytes( void );
  void shiftRows( void );
  void mixColumns( void );


  public:
  inline Aes( void )
    {
    }

  inline Aes( const Aes& in )
    {
    if( in.testForCopy )
      return;

    throw "Copy constructor for Aes.";
    }

  inline ~Aes( void )
    {

    }

  void setKey( const CharBuf& theKey,
               const Int32 keySize );

  void encryptCharBuf( const CharBuf& inBuf,
                       CharBuf& outBuf,
                       const Int32 where );

  void encryptTest( void );


  };
