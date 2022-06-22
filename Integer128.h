// Copyright Eric Chauvin 2022


// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html



#include "../CppBase/BasicTypes.h"
#include "../CryptoBase/Integer64.h"


class Integer128
  {
  private:
  bool testForCopy = false;
  // bool negative = false;
  Integer64 baseL;
  Integer64 baseH;
  Integer64 carryL;
  Integer64 carryH;

  public:
  inline Integer128( void )
    {
    }

  inline Integer128( const Integer128& in )
    {
    if( in.testForCopy )
      throw "Integer128 copy constructor.";

    }

  inline ~Integer128( void )
    {
    }


  inline void cleanCarrys( void )
    {
    Integer64 movePart;

    movePart.copy( baseL );
    baseL.setBase( movePart );

    movePart.shiftRAll();
    movePart.accumAll( baseH );
    baseH.setBase( movePart );

    movePart.shiftRAll();
    movePart.accumAll( carryL );
    carryL.setBase( movePart );

    movePart.shiftRAll();
    movePart.accumAll( carryH );
    carryH.setBase( movePart );

    movePart.shiftRAll();
    if( !movePart.isZero())
      throw "Overflow in cleanCarrys.";

    }



  };
