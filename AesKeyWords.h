// Copyright Eric Chauvin, 2022.


// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html



#pragma once



#include "../CppBase/BasicTypes.h"
#include "../CppBase/RangeC.h"


class AesKeyWords
  {
  private:
  // The key schedule generates Nb(Nr + 1) words.
  // The maximum size is 4 * (14 + 1) = 60.
  Uint32 keyScheduleWords[60] = { 1, 2, 3 };



  public:
  inline Uint32 getV( const Int32 where ) const
    {
    // Comment these out after testing.
    RangeC::test2( where, 0,
                  60 - 1,
            "AesKeyWords.getV() range for where." );

    return keyScheduleWords[where];
    }

  inline void setV( const Int32 where,
                    const Uint32 val )
    {
    RangeC::test2( where, 0,
                  60 - 1,
            "AesKeyWords.setV() range for where." );

    keyScheduleWords[where] = val;
    }

  };
