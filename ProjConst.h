// Copyright Eric Chauvin, 2021 - 2022.
// This copyright notice has to stay at the top.


// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html





#pragma once


// These are constants that would rarely change.
// If you want to use Integers with a bigger
// maximum siae you can change these.



#include "../LinuxApi/BasicTypes.h"


class ProjConst
  {
  private:

  public:
  // digitArraySize = 100 is 2,400 bits.
  // If the prime is x bits long then the
  // modulus is 2 times X bits long and the
  // modulus squared is 4 times X bits long.
  // A prime of 2K bits requires 8K sized
  // numbers.
  // NumbSys is not exact.  It's a little
  // bigger than the modulus.  So a little
  // over that size.

  // See Integer.h:
  // 5 times 2,400 bits.
  static const Int32 digitArraySize = 500;

  // The Integers have 24 bit wide digits so
  // it's easy to calculate how wide they have
  // to be for a certain number of bits, like
  // for a 1024 bit RSA prime.  But the Crt
  // numbers have digits that vary in width
  // from the smallest prime to the biggest
  // prime that is at the SPrimes value at
  // index crtDigitArraySize.
  // So for example if the biggest prime used
  // in Crt numbers is 5279, that's how wide
  // the digit in that position is.  So about
  // 13 bits wide at that point.

  // Just test how big this can be without
  // overflowing an Integer.  CrtMath can use
  // very large Integers for its base values
  // so it will throw an exception when it
  // initializes if it overflows an Integer.
  static const Int32 crtDigitArraySize =
                     digitArraySize + 200;
                     // digitArraySize * 2;

  // This is for 24 bit primes.
  static const Int32 primesArraySize =
                                     1024 * 32;


  // See Exponents.h:
  // To match this with a full size Integer it
  // should be digitArraySize * 24 bits.
  static const Int32 exponentsLast = 2000;

  // See Ellip.h:
  // The number of EPoints in an array.
  static const Int32 ellipseLast = 2000;


  };
