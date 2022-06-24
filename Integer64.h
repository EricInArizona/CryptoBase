// Copyright Eric Chauvin 2022


// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html



#include "../CppBase/BasicTypes.h"
#include "../CryptoBase/Integer32.h"


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



  inline void setZero( void )
    {
    baseL.setZero();
    baseH.setZero();
    }

  inline bool isZero( void ) const
    {
    if( baseL.isZero() && baseH.isZero() )
      return true;

    return false;
    }


  inline void cleanCarrys( void )
    {
    Integer32 movePart;

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

    if( x.baseL.isZero() || y.baseL.isZero())
      lowPart.setZero();
    else
      lowPart.mult( x.baseL, y.baseL );

// ======
    mid1.mult( x.baseH, y.baseL );
    mid2.mult( x.baseL, y.baseH );
    highPart.mult( x.baseH, y.baseH );

    mid1L.setBase( mid1 );
    mid1H.copy( mid1 );
    mid1H.shiftRAll();

    mid2L.setBase( mid2 );
    mid2H.copy( mid2 );
    mid2H.shiftRAll();

    baseL.setBase( lowPart );

    lowPart.shiftRAll();
    baseH.setBase( lowPart );
    baseH.accumAll( mid1L );
    baseH.accumAll( mid2L );

    carryL.setBase( highPart );
    carryL.accumAll( mid1H );
    carryL.accumAll( mid2H );

    highPart.shiftRAll();
    carryH.setBase( highPart );

    cleanCarrys();
    }


  inline void shiftRAll( void )
    {
    baseL.setBase( carryL );
    baseH.setBase( carryH );
    carryL.setZero();
    carryH.setZero();
    }


  inline void copy( Integer64& in )
    {
    baseL.copy( in.baseL );
    baseH.copy( in.baseH );
    carryL.copy( in.carryL );
    carryH.copy( in.carryH );
    }

  inline void setBase( const Integer64& x )
    {
    baseL.copy( x.baseL );
    baseH.copy( x.baseH );
    carryL.setZero();
    carryH.setZero();
    }


  inline void accumAll( const Integer64& x )
    {
    // Carry can be non zero here.

    baseL.accumAll( x.baseL );
    baseH.accumAll( x.baseH );
    carryL.accumAll( x.carryL );
    carryH.accumAll( x.carryH );

    cleanCarrys();
    }

  };