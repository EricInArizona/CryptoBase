// Copyright Eric Chauvin 2022


// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html



#include "../CppBase/BasicTypes.h"



class Integer64
  {
  private:
  bool testForCopy = false;
  bool negative = false;
  // It is the normal behavior of unsigned
  // integers to overflow with no exception
  // or warning.  It is mod 2^32.

  // This is always 8.  What changes is the
  // size of the D values.
  static const Int32 digitArSize = 8;

  U32Array D;
  Int32 index = 0;

  public:
  inline Integer64( void )
    {
    D.setSize( digitArSize );
    setZero();
    }

  inline Integer64( const Integer64& in )
    {
    if( in.testForCopy )
      return;

    throw "Integer64 copy constructor.";
    }

  inline ~Integer64( void )
    {
    }


  inline void setZero( void )
    {
    negative = false;
    index = 0;
    D[0] = 0;
    }

  inline void setOne( void )
    {
    negative = false;
    index = 0;
    D[0] = 1;
    }

  inline bool isZero( void ) const
    {
    if( index != 0 )
      return false;

    if( D[0] != 0 )
      return false;

    if( negative )
      throw "Integer64 Zero is negative.";

    return true;
    }

  inline bool isOne( void ) const
    {
    if( negative )
      return false;

    if( index != 0 )
      return false;

    if( D[0] != 1 )
      return false;

    return true;
    }

  inline bool isNegOne( void ) const
    {
    if( !negative )
      return false;

    if( index != 0 )
      return false;

    if( D[0] != 1 )
      return false;

    return true;
    }


  inline bool getNegative( void ) const
    {
    return negative;
    }

  inline bool isULong( void ) const
    {
    if( negative )
      return false;

    if( index > 3 )
      return false;

    return true;
    }

  inline Uint64 getAsULong( void ) const
    {
    // Assumes the caller has alredy checked
    // to see with isULong().
    if( index > 3 )
      throw "getAsULong() too long.";

    Uint64 result = D[3];
    result <<= 16;
    result |= D[2];
    result <<= 16;
    result |= D[1];
    result <<= 16;
    result |= D[0];

    return result;
    }

  void setFromULong( const Uint64 toSet );
  void addULong( const Uint64 toAdd );
  void cleanCarry( void );
  void add( const Integer64& toAdd );
  void copy( const Integer64& from );
  void copyLow( const Integer64& from );
  void copyHigh( const Integer64& from );


  };
