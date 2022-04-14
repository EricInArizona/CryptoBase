// Copyright Eric Chauvin, 2022.



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html


#pragma once



#include "../LinuxApi/BasicTypes.h"
#include "../LinuxApi/RangeC.h"
#include "../LinuxApi/FileIO.h"

#include "Integer.h"
#include "IntegerMath.h"
#include "Mod.h"



class TonelliShanks
  {
  private:
  // Int32 testForCopy = 123;

  public:
  // TonelliShanks( void );
  // TonelliShanks( const TonelliShanks& in );
  // ~TonelliShanks( void );

  static bool modRoot( const Integer& A,
                       Integer& result,
                       const Integer& prime,
                       IntegerMath& intMath,
                       Mod& mod );

  static bool modRoot3( const Integer& A,
                        Integer& result,
                        const Integer& prime,
                        IntegerMath& intMath,
                        Mod& mod );

  };
