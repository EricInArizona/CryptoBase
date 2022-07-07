// Copyright Eric Chauvin 2022


// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html



#pragma once



// This is supposed to be cryptographically
// random.  It is random enough for now
// to test an algorithm.

// I'm not writing a long and in-depth book
// on randomness just yet.
// The more complex something is, the more
// random it looks.  The generated key used in
// TLS is complex. SHA 256 makes a random looking
// series of bytes.
// Is the Universe just a
// series of meaningless random events?
// Or is it complex?
// I won't be answering that question any
// time soon.


#include "../CppBase/BasicTypes.h"
#include "../CppBase/CharBuf.h"


class Randomish
  {
  private:

  public:
  static void makeRandomBytes( CharBuf& cBuf,
                             const Int32 howMany );

  };
