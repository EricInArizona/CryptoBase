// Copyright Eric Chauvin, 2021 - 2022.



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html



#pragma once



#include "../LinuxApi/BasicTypes.h"
#include "../LinuxApi/BoolArray.h"
#include "../LinuxApi/RangeC.h"
#include "../LinuxApi/FileIO.h"

#include "ProjConst.h"
#include "SPrimes.h"



class QuadRes
  {
  private:
  Int32 testForCopy = 123;

  static const Int32 last = ProjConst::crtDigitArraySize;

  BoolArray* bArrays;

  public:
  QuadRes( void );
  QuadRes( const QuadRes& in );
  ~QuadRes( void );
  void init( const SPrimes& sPrimes, FileIO& mainIO );
  inline bool getVal( const Int32 primeAt,
                      const Int32 where ) const
    {
    RangeC::test2( primeAt, 0, last - 1,
            "QuadRes.getVal() primeAt range." );

    return bArrays[primeAt].getVal( where );
    }

  // Maybe it will inline it, maybe it won't.
  // Look at the assembly language to see.
  static inline bool bytesQR( const Int32 test )
    {
    // Is this number congruent to a square mod 2^8?
    // (Quadratic residue mod 2^8)

    Int32 firstByte = test & 0xFF;

    // The bottom 4 bits can only be 0, 1, 4 or 9
    // 0000, 0001, 0100 or 1001
    // The bottom 2 bits can only be 00 or 01

    switch( firstByte )
      {
      case 0x00: return true;
      case 0x01: return true;
      case 0x04: return true;
      case 0x09: return true;
      case 0x10: return true;
      case 0x11: return true;
      case 0x19: return true;
      case 0x21: return true;
      case 0x24: return true;
      case 0x29: return true;
      case 0x31: return true;
      case 0x39: return true;
      case 0x40: return true;
      case 0x41: return true;
      case 0x44: return true;
      case 0x49: return true;
      case 0x51: return true;
      case 0x59: return true;
      case 0x61: return true;
      case 0x64: return true;
      case 0x69: return true;
      case 0x71: return true;
      case 0x79: return true;
      case 0x81: return true;
      case 0x84: return true;
      case 0x89: return true;
      case 0x90: return true;
      case 0x91: return true;
      case 0x99: return true;
      case 0xA1: return true;
      case 0xA4: return true;
      case 0xA9: return true;
      case 0xB1: return true;
      case 0xB9: return true;
      case 0xC1: return true;
      case 0xC4: return true;
      case 0xC9: return true;
      case 0xD1: return true;
      case 0xD9: return true;
      case 0xE1: return true;
      case 0xE4: return true;
      case 0xE9: return true;
      case 0xF1: return true;
      case 0xF9: return true;  // 44 out of 256.

      default: return false;
      }
    }


  // static bool bytesQR( const Int32 test );

  static bool isSmallQR( const Int32 number );
  static bool isQR17( const Int32 number );
  static bool isQR19( const Int32 number );
  static bool isQR23( const Int32 number );
  static bool isQR29( const Int32 number );

  };
