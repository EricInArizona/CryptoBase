// Copyright Eric Chauvin, 2022.


// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html



#pragma once



#include "../CppBase/BasicTypes.h"
#include "AesConst.h"
#include "../CppBase/RangeC.h"


class AesKeyWords
  {
  private:
  // bool testForCopy = false;

  // The key schedule generates Nb(Nr + 1) words.
  // That is 4 times 15 words.

  Uint32 keyScheduleWords[AesConst::KeyWordsSize]
                                    = { 1, 2, 3 };



  public:
  inline Uint32 getV( const Int32 where ) const
    {
    RangeC::test2( where, 0, 
                  AesConst::KeyWordsSize - 1,
            "AesKeyWords.getV() range for where." );

    return keyScheduleWords[where];
    }

  inline void setV( const Int32 where,
                    const Uint32 val )
    {
    RangeC::test2( where, 0, 
                  AesConst::KeyWordsSize - 1,
            "AesKeyWords.setV() range for where." );

    keyScheduleWords[where] = val;
    }

  };
