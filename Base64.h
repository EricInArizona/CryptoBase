// Copyright Eric Chauvin 2022


// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html



// RFC 4648 - The Base16, Base32, and Base64
// Data Encodings



#include "../CppBase/BasicTypes.h"
#include "../CppBase/CharBuf.h"
#include "../CppBase/BoolArray.h"



class Base64
  {
  private:
  bool testForCopy = false;

  static const Int32 boolAlphaSize = 'z' + 1;

  // This character set is called base64.
  const char* Letters64 =
       "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
       "abcdefghijklmnopqrstuvwxyz"
       "0123456789+/";

  // This character set is called base64url.
  // const char* Letters64Url =
    //   "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    //   "abcdefghijklmnopqrstuvwxyz0123456789-_";

  BoolArray boolAlpha;
  CharBuf charSet;
  static const char padC = '=';

  public:
  inline Base64( void )
    {
    charSet.clear();
    charSet.appendCharPt( Letters64 );
    setupBoolAlpha();
    }

  inline Base64( const Base64& in )
    {
    if( in.testForCopy )
      return;

    throw "Base64 copy constructor.";
    }

  inline ~Base64( void )
    {
    }

  void setupBoolAlpha( void );

  inline char bits6ToChar( const Uint32 theByte )
                                            const
    {
    // getC() would catch this, but if the index
    // is > 63 then it's not a 6 bit number.
    return charSet.getC( Casting::u32ToI32(
                                     theByte ));
    }

  inline Uint32 charTo6Bits( const char letter,
                       bool useBaseUrl ) const
    {
    //   "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    //   "abcdefghijklmnopqrstuvwxyz"
    //   "0123456789+/";

    if( (letter >= 'A') && (letter <= 'Z') )
      return Casting::i32ToU32( letter - 'A' );

    if( (letter >= 'a') && (letter <= 'z'))
      return Casting::i32ToU32(
                           letter + 26 - 'a' );

    if( (letter >= '0') && (letter <= '9'))
      return Casting::i32ToU32(
                           letter + 52 - '0' );

    if( useBaseUrl )
      {
      if( letter == '-' ) // minus
        return 62;

      if( letter == '_')
        return 63;

      }
    else
      { // regular base64
      if( letter == '+' )
        return 62;

      if( letter == '/')
        return 63;

      }

    // At this point it is not one of the
    // letters of the alphabet.

    // Padding char.

    if( letter == '=')
      return 0xF0;

    // RFC 4648 says:
    // "Implementations MUST reject the encoded
    // data if it contains characters outside the
    // base alphabet...".

    // It's some other invalid letter.
    return 0xFF;
    }

  void test( void );
  bool bytesToCode( CharBuf& inBuf,
                    CharBuf& outBuf );
  bool codeToBytes( CharBuf& inBuf,
                    CharBuf& outBuf );

  Int32 lettersToBits( CharBuf& outBuf,
                       const char char1,
                       const char char2,
                       const char char3,
                       const char char4 );

  void filterCharSet( CharBuf& inBuf,
                      CharBuf& outBuf );


  };
