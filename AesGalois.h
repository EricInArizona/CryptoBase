// Copyright Eric Chauvin, 2022.


// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html




#pragma once



#include "../CppBase/BasicTypes.h"
#include "../CppBase/RangeC.h"
#include "../CppBase/StIO.h"


// Galois Counter Mode
// A GF2^128 finite field.
// 128 bits in the block.

// With GCM it is a stream cypher.
// So no decrypt function.
// Right?
// A different IV has to be used each time.



class AesGalois
  {
  private:
  // bool testForCopy = false;

  public:
  void encryptTest( void );

  };
