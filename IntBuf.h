// Copyright Eric Chauvin, 2021 - 2022.



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html



#pragma once


// This is for copies of Integer.cpp objects,
// but it has a dynamically allocated array.
// As opposed to the array being on the stack.

#include "../CppBase/BasicTypes.h"
#include "../CppBase/RangeC.h"

#include "ProjConst.h"


class IntBuf
  {
  private:
  Int32 testForCopy = 123;
  Int32 index = 0;
  static const Int32 last =
                      ProjConst::digitArraySize;
  Int32* D;

  public:
  IntBuf( void );
  IntBuf( const IntBuf& in );
  ~IntBuf( void );
  inline Int32 getIndex( void ) const
    {
    return index;
    }

  inline void setIndex( const Int32 setTo )
    {
    // RangeC::test( setTo, 0, last - 1,
       //        "IntBuf.setIndex() range." );

    index = setTo;
    }

  inline Int32 getD( const Int32 where ) const
    {
    // RangeC::test( where, 0, last - 1,
       //           "IntBuf.getD() range." );

    return D[where];
    }


  inline void setD( const Int32 where,
                    const Int32 toSet )
    {
    // RangeC::test( where, 0, last - 1,
       //              "IntBuf.setD() range." );

    // 24 bits.
    // RangeC::test( toSet, 0, 0xFFFFFF,
       //        "IntBuf.setD() toSet size." );

    D[where] = toSet;
    }

  };
