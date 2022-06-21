// Copyright Eric Chauvin 2022


// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html




#include "../CppBase/BasicTypes.h"


class Integer32
  {
  private:
  bool testForCopy = false;
  // bool negative = false;
  Int64 base = 0;
  Int64 carry = 0;

  public:
  inline Integer32( void )
    {
    setZero();
    }

  inline Integer32( const Integer32& in )
    {
    if( in.testForCopy )
      throw "Integer32 copy constructor.";

    }

  inline ~Integer32( void )
    {
    }

  inline void setZero( void )
    {
    base = 0;
    carry = 0;
    }

  inline bool isZero( void ) const
    {
    if( (base == 0) && (carry == 0))
      return true;

    return false;
    }

  inline void setOne( void )
    {
    base = 1;
    carry = 0;
    }

  inline void copy( const Integer32& x )
    {
    base = x.base;
    carry = x.carry;
    }

  inline void setBase( const Integer32& x )
    {
    base = x.base;
    carry = 0;
    }

  inline void clearCarry( void )
    {
    carry = 0;
    }

  inline void shiftR32( void )
    {
    base = carry;
    carry = 0;
    }

  inline void mult( const Integer32& x,
                    const Integer32& y )
    {
    // For testing:
    if( x.carry != 0 )
      throw "Calling mult with x carry set.";

    if( y.carry != 0 )
      throw "Calling mult with y carry set.";

    // if( (0xFFFFFFFFULL * 0xFFFFFFFFULL) ==
    //                0xFFFFFFFE00000001ULL)
               //   0xFFFF FFFE 0000 0001
      // throw "This is always true.";

    base = x.base * y.base;
    carry = base >> 32;
    base = base & 0xFFFFFFFF;
    }

  inline void add( const Integer32& x,
                   const Integer32& y )
    {
    // For testing:
    if( x.carry != 0 )
      throw "Calling add with x carry set.";

    if( y.carry != 0 )
      throw "Calling add with y carry set.";

    base = x.base + y.base;
    carry = base >> 32;
    base = base & 0xFFFFFFFF;
    }

  inline void accumAll( const Integer32& x )
    {
    // Carry can be non zero here.

    base += x.base;
    carry += base >> 32;
    base = base & 0xFFFFFFFF;
    carry += x.carry;
    }

  // inline void multS(
  // inline void addSign(


  };
