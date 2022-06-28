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
  const char padC = '=';

  // Percent encode would be its own class.
  // Search news source code.
  // Percent encoded values: + is %2B.
  // / is %2F and = is %3D.

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
    // letters of the alphabet, so it has to
    // be rejected, unless otherwise specified.
    // After this point it returns a byte with
    // the highest two bits set to 1.
    // So it can't be a 6 bit encoding.
    // But it can decide to ignore it or reject
    // the whole thing if, for example, 0xF1
    // is returned.

    // Padding char.
    // RFC 4648 says:
    // "Implementations MUST include appropriate
    // pad characters at the end of encoded data
    // unless the specification referring to this
    // document explicitly states otherwise."

    if( letter == '=')
      return 0xF0;


    // MIME encoding limits line length
    // to 76 characters, then there's a line feed
    // and/or CR.
    // PEM encoding has a line length limit
    // of 64 characters.

    // RFC 4648 says:
    // "Implementations MUST NOT add line feeds
    // to base-encoded data unless the
    // specification referring to this document
    // explicitly directs base encoders to add
    // line feeds after a specific number of
    // characters."

    if( (letter == '\n') || (letter == '\r'))
      {
      return 0xF1;
      }

    if( letter == ' ' )
      {
      return 0xF2;
      }

    // Extra bits have to be set to zero.

    // RFC 4648 says:
    // "Implementations MUST reject the encoded
    // data if it contains characters outside the
    // base alphabet when interpreting base-encoded
    // data, unless the specification referring
    // to this document explicitly states
    // otherwise.  Such specifications may
    // instead state, as MIME does, that
    // characters outside the base encoding
    // alphabet should simply be ignored when
    // interpreting data ("be liberal in what
    // you accept").  Note that this means that
    // any adjacent carriage return/line feed
    // (CRLF) characters constitute
    // 'non-alphabet characters' and are ignored.
    // Furthermore, such specifications MAY
    // ignore the pad character, "=", treating
    // it as non-alphabet data, if it is present
    // before the end of the encoded data.  If
    // more than the allowed number of pad
    // characters is found at the end of the
    // string (e.g., a base 64 string terminated
    // with "==="), the excess pad characters
    // MAY also be ignored."

    // It's some other invalid letter.
    return 0xFF;
    }

  void test( void );
  bool bytesToCode( CharBuf& inBuf,
                    CharBuf& outBuf );
  bool codeToBytes( CharBuf& inBuf,
                    CharBuf& outBuf );

  bool lettersToBits( CharBuf& outBuf,
                      const char char1,
                      const char char2,
                      const char char3,
                      const char char4 );



  };
