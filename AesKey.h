// Copyright Eric Chauvin, 2022.


// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html




#pragma once



#include "../CppBase/BasicTypes.h"
#include "AesConst.h"


class AesKey
  {
  private:
  Uint8 key[AesConst::KeyLengthInBytes256] =
                               { 1, 2, 3 };


  public:
  inline Uint8 getV( const Int32 where ) const
    {
    RangeC::test2( where, 0,
               AesConst::KeyLengthInBytes256 - 1,
             "AesKey.getV() range for where." );

    return key[where];
    }

  inline void setV( const Int32 where,
                    const Uint8 val )
    {
    RangeC::test2( where, 0,
                AesConst::KeyLengthInBytes256 - 1,
                "AesKey.setV() range for where." );

    key[where] = val;
    }


  };
