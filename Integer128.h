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


/*
  inline void cleanCarrys( void )
    {
    Integer64 movePart;

    movePart.copy( baseL );
    baseL.setBase( movePart );

    movePart.shiftR64();
    movePart.accumAll( baseH );
    baseH.setBase( movePart );

    movePart.shiftR64();
    movePart.accumAll( carryL );
    carryL.setBase( movePart );

    movePart.shiftR64();
    movePart.accumAll( carryH );
    carryH.setBase( movePart );

    movePart.shiftR64();
    if( !movePart.isZero())
      throw "Overflow in cleanCarrys.";

    }
*/


/*
  inline void add( const Integer128& x,
                   const Integer128& y )
    {
    // For testing:
    if( !x.carryL.isZero() )
      throw "128 carry add not zero.";

    if( !y.carryL.isZero() )
      throw "128 carry add not zero.";

    baseL.add( x.baseL, y.baseL );
    baseH.add( x.baseH, y.baseH );

    cleanCarrys();
    }


  inline void mult( const Integer128& x,
                    const Integer128& y )
    {
    Integer64 lowPart;
    Integer64 highPart;
    Integer64 mid1;
    Integer64 mid1L;
    Integer64 mid1H;
    Integer64 mid2;
    Integer64 mid2L;
    Integer64 mid2H;

    lowPart.mult( x.baseL, y.baseL );
    mid1.mult( x.baseH, y.baseL );
    mid2.mult( x.baseL, y.baseH );
    highPart.mult( x.baseH, y.baseH );

    mid1L.setBase( mid1 );
    mid1H.copy( mid1 );
    mid1H.shiftR32();

    mid2L.setBase( mid2 );
    mid2H.copy( mid2 );
    mid2H.shiftR32();

    baseL.setBase( lowPart );

    lowPart.shiftR64();
    baseH.setBase( lowPart );
    baseH.accumAll( mid1L );
    baseH.accumAll( mid2L );

    carryL.setBase( highPart );
    carryL.accumAll( mid1H );
    carryL.accumAll( mid2H );

    highPart.shiftR64();
    carryH.setBase( highPart );

    cleanCarrys();
    }

*/

  };
