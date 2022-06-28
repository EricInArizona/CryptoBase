// Copyright Eric Chauvin 2022.


// This was only partially done.


// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html


#pragma once


#include "../CppBase/BasicTypes.h"
#include "../CppBase/CharBuf.h"


class Base16Number
  {
  private:
  static const Int32 digitArraySize =
                                   1024 * 32;
  Int32 index = 0;
  Int32* D;

  public:
  Base16Number( void );
  Base16Number( const CharBuf& toSet );
  Base16Number( const Base16Number& obj );
  ~Base16Number( void );
  inline Int32 getD( const Int32 where )
    {
    if( where >= digitArraySize )
      throw "getD() index out of bounds.";

    return D[where];
    }

  inline Int32 getIndex( void )
    {
    return index;
    }

  static inline Int32 convertDigit(
                             const Int32 digit )
    {
    switch( digit )
      {
      case '0': return 0;
      case '1': return 1;
      case '2': return 2;
      case '3': return 3;
      case '4': return 4;
      case '5': return 5;
      case '6': return 6;
      case '7': return 7;
      case '8': return 8;
      case '9': return 9;

      case 'a': return 10;
      case 'b': return 11;
      case 'c': return 12;
      case 'd': return 13;
      case 'e': return 14;
      case 'f': return 15;

      case 'A': return 10;
      case 'B': return 11;
      case 'C': return 12;
      case 'D': return 13;
      case 'E': return 14;
      case 'F': return 15;
      }

    return 126;
    }

  static inline bool isDigit( const Int32 digit )
    {
    switch( digit )
      {
      case '0': return true;
      case '1': return true;
      case '2': return true;
      case '3': return true;
      case '4': return true;
      case '5': return true;
      case '6': return true;
      case '7': return true;
      case '8': return true;
      case '9': return true;

      case 'a': return true;
      case 'b': return true;
      case 'c': return true;
      case 'd': return true;
      case 'e': return true;
      case 'f': return true;

      case 'A': return true;
      case 'B': return true;
      case 'C': return true;
      case 'D': return true;
      case 'E': return true;
      case 'F': return true;
      }

    return false;
    }


  void setFromCharBuf( const CharBuf& toSet );
  void setFromBytes( const CharBuf& charBuf );

  };
