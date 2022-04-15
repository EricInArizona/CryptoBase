// Copyright Eric Chauvin, 2021 - 2022.



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html


#pragma once


#include "../LinuxApi/BasicTypes.h"
// #include "../LinuxApi/FileIO.h"

#include "Integer.h"
#include "IntegerMath.h"
#include "Mod.h"


// A point on an elliptic curve.


class EPoint
  {
  private:
  bool infin = false; // Infinite
  Integer X;
  Integer Y;

  public:
  EPoint( void );
  EPoint( const EPoint& in );
  EPoint( const Integer& setX,
          const Integer& setY );

  // Domain parameters: (p, a, b, G, n, h )
  // The A and B coeffients define the curve.
  // The curve E: y^2 = x^3 + ax + b
  // a = 0
  // b = 7
  // The curve used in Bitcoin: y^2 = x^3 + 7
  static const Int32 coefA = 0;
  static const Int32 coefB = 7;


  inline bool getInfin( void ) const
    {
    return infin;
    }

  inline void setInfin( bool setTo )
    {
    infin = setTo;
    }

  void setValues( const Integer& setX,
                  const Integer& setY );

  void copy( const EPoint& p );
  bool isEqual( const EPoint& in ) const;
  bool isConjugate( const EPoint& p,
                    const Integer& modulus,
                    Mod& mod,
                    IntegerMath& intMath ) const;
  bool isOnCurve( const Integer& modulus,
              Mod& mod, IntegerMath& intMath );
  void add( const EPoint& p, const EPoint& q,
            const Integer& modulus,
            Mod& mod,
            IntegerMath& intMath  );
  void doubleP( const EPoint& p,
                const Integer& modulus,
                Mod& mod,
                IntegerMath& intMath );


  };
