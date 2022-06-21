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
  // bool negative = false;
  Integer32 baseL;
  Integer32 baseH;
  Integer32 carryL;
  Integer32 carryH;

  public:
  inline Integer64( void )
    {
    }

  inline Integer64( const Integer64& in )
    {
    if( in.testForCopy )
      throw "Integer64 copy constructor.";

    }

  inline ~Integer64( void )
    {
    }


  inline void cleanCarrys( void )
    {
    Integer32 movePart;

    movePart.copy( baseL );
    baseL.setBase( movePart );

    movePart.shiftR32();
    movePart.accumAll( baseH );
    baseH.setBase( movePart );

    movePart.shiftR32();
    movePart.accumAll( carryL );
    carryL.setBase( movePart );

    movePart.shiftR32();
    movePart.accumAll( carryH );
    carryH.setBase( movePart );

    movePart.shiftR32();
    if( !movePart.isZero())
      throw "Overflow in cleanCarrys.";

    }


  inline void add( const Integer64& x,
                   const Integer64& y )
    {
    // For testing:
    if( !x.carryL.isZero() )
      throw "64 carry add not zero.";

    if( !y.carryL.isZero() )
      throw "64 carry add not zero.";

    baseL.add( x.baseL, y.baseL );
    baseH.add( x.baseH, y.baseH );

    cleanCarrys();
    }


  inline void mult( const Integer64& x,
                    const Integer64& y )
    {
    Integer32 lowPart;
    Integer32 highPart;
    Integer32 mid1;
    Integer32 mid1L;
    Integer32 mid1H;
    Integer32 mid2;
    Integer32 mid2L;
    Integer32 mid2H;

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

    lowPart.shiftR32();
    baseH.setBase( lowPart );
    baseH.accumAll( mid1L );
    baseH.accumAll( mid2L );

    carryL.setBase( highPart );
    carryL.accumAll( mid1H );
    carryL.accumAll( mid2H );

    highPart.shiftR32();
    carryH.setBase( highPart );

    cleanCarrys();
    }


  };
