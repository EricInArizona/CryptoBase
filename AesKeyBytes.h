// Copyright Eric Chauvin, 2022.


// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html




#pragma once



#include "../CppBase/BasicTypes.h"
#include "AesConst.h"


class AesKeyBytes
  {
  private:
  // bool testForCopy = false;
  Uint8 keyScheduleBytes[AesConst::KeyBytesSize] =
                                  { 1, 2, 3 };

  public:
  inline Uint8 getV( const Int32 row,
                     const Int32 column ) const
    {
    // This is colum Major order because
    // consecutive elements in a column are
    // next to each other.
    const Int32 where = (row * 4) + column;

    // This range checking can be commented out.
    RangeC::test2( where, 0,
                   AesConst::KeyBytesSize,
       "AesKeyBytes.getV() range for where." );

    return keyScheduleBytes[where];
    }

  inline void setV( const Int32 row,
                    const Int32 column,
                    const Uint8 val )
    {
    const Int32 where = (row * 4) + column;

    RangeC::test2( where, 0, AesConst::KeyBytesSize,
       "AesKeyBytes.setV() range for where." );

    keyScheduleBytes[where] = val;
    }

  };
