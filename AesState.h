// Copyright Eric Chauvin, 2022.


// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html




#pragma once



#include "../CppBase/BasicTypes.h"
#include "../CppBase/RangeC.h"


// This corresponds to a 128 bit block.
// 16 times 8 bits.
// AES has a fixed block size of 128 bits.
// State as in "state machine".


class AesState
  {
  private:
  bool testForCopy = false;
  Uint8 aR[16] = { 1, 2, 3 };


  public:
  inline AesState( void )
    {

    }

  inline AesState( const AesState& in )
    {
    if( in.testForCopy )
      return;

    throw "Copy constructor for AesState.";
    }

  inline ~AesState( void )
    {

    }

  inline void copy( AesState& in )
    {
    for( Int32 count = 0; count < 16; count++ )
      aR[count] = in.aR[count];

    }


  inline Uint8 getV( const Int32 row,
                     const Int32 column ) const
    {
    // This is colum Major order because
    // consecutive elements in a column are
    // next to each other.
    const Int32 where = (row * 4) + column;

    // This range checking can be commented out.
    RangeC::test2( where, 0, 15,
       "AesState.getV() range for where." );

    return aR[where];
    }

  inline void setV( const Int32 row,
                    const Int32 column,
                    const Uint8 val )
    {
    const Int32 where = (row * 4) + column;

    RangeC::test2( where, 0, 15,
       "AesState.setV() range for where." );

    aR[where] = val;
    }


  };
