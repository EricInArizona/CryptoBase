// Copyright Eric Chauvin, 2022.


// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html




#pragma once



#include "../CppBase/BasicTypes.h"


class AesKey
  {
  private:
  // Maximum key size is 32 bytes.
  Uint8 key[32] = { 1, 2, 3 };


  public:
  inline Uint8 getV( const Int32 where ) const
    {
    RangeC::test2( where, 0,
               32 - 1,
             "AesKey.getV() range for where." );

    return key[where];
    }

  inline void setV( const Int32 where,
                    const Uint8 val )
    {
    RangeC::test2( where, 0,
                32 - 1,
                "AesKey.setV() range for where." );

    key[where] = val;
    }


  };
