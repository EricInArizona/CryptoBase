// Copyright Eric Chauvin, 2022.


// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html




#pragma once



#include "../CppBase/BasicTypes.h"
#include "../CppBase/RangeC.h"


class AesBlock
  {
  private:
  // 16 times 8 bytes is a 128 bit block.
  Uint8 aR[16] = { 1, 2, 3 };


  public:
  inline Uint8 getV( const Int32 where ) const
    {
    // This range checking can be commented out.
    RangeC::test2( where, 0, 15,
       "AesBlock.getV() range for where." );

    return aR[where];
    }

  inline void setV( const Int32 where,
                    const Uint8 val )
    {
    RangeC::test2( where, 0, 15,
       "AesBlock.setV() range for where." );

    aR[where] = val;
    }

  };
