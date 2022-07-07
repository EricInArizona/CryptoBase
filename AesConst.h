// Copyright Eric Chauvin, 2022.


// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html




#pragma once



#include "../CppBase/BasicTypes.h"


class AesConst
  {
  public:
  // 10 rounds for 128 bit key, 12 rounds for 192,
  // 14 rounds for 256 bit key.
  // static const Int32 NumberOfRounds128 = 10;
  // static const Int32 NumberOfRounds192 = 12;
  static const Int32 NumberOfRounds256 = 14;
  static const Int32 KeyLengthInBytes256 = 32;
                                     // 256 / 8

  // The key schedule generates Nb(Nr + 1) words.
  // That is 4 times 15 words.

  static const Int32 KeyWordsSize =
           4 * (NumberOfRounds256 + 1);

  static const Int32 KeyBytesSize =
                           KeyWordsSize * 4;



  };
