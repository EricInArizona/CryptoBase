// Copyright Eric Chauvin 2022


// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html



#include "../CppBase/BasicTypes.h"
#include "../CryptoBase/Integer64.h"



class Integer128
  {
  private:
  bool testForCopy = false;

/*
  // bool negative = false;
  Integer64 baseL;
  Integer64 baseH;
  Integer64 carryL;
  Integer64 carryH;
*/

  public:
  inline Integer128( void )
    {
    }

  inline Integer128( const Integer128& in )
    {
    if( in.testForCopy )
      return;

    throw "Integer128 copy constructor.";
    }

  inline ~Integer128( void )
    {
    }

  void cleanCarry( void );

  };
