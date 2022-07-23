// Copyright Eric Chauvin, 2022.


// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html




#pragma once



#include "../CppBase/BasicTypes.h"
#include "../CppBase/RangeC.h"
#include "../CppBase/StIO.h"


// RFC 5288 - AES Galois Counter Mode (GCM)
// Cipher Suites for TLS

// RFC 5116 - An Interface and Algorithms for
// Authenticated encryption with Associated- Data
// TLS_AES_128_GCM_SHA256    {0x13,0x01}



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
