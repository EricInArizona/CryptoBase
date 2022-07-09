// Copyright Eric Chauvin, 2022.


// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html




#pragma once



#include "../CppBase/BasicTypes.h"



class AesKeyBytes
  {
  private:
  // Max KeyWordsSize times 4.
  Uint8 keyScheduleBytes[60 * 4] = { 1, 2, 3 };

  public:
  inline Uint8 getV( const Int32 row,
                     const Int32 column ) const
    {
    const Int32 where = (row * 4) + column;

    // Comment this out after testing.
    RangeC::test2( where, 0,
                   (60 * 4) - 1,
       "AesKeyBytes.getV() range for where." );

    return keyScheduleBytes[where];
    }

  inline void setV( const Int32 row,
                    const Int32 column,
                    const Uint8 val )
    {
    const Int32 where = (row * 4) + column;

    RangeC::test2( where, 0, (60 * 4) - 1,
       "AesKeyBytes.setV() range for where." );

    keyScheduleBytes[where] = val;
    }

  };
