// Copyright Eric Chauvin, 2022.


// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html


#pragma once



#include "../CppBase/BasicTypes.h"
#include "Integer.h"
#include "IntegerMath.h"
#include "Mod.h"



class MakePrime
  {
  private:
  Int32 testForCopy = 123;

  public:
  MakePrime( void );
  MakePrime( const MakePrime& in );
  ~MakePrime( void  );
  void makeIt( Integer& result,
               const Int32 indexSize,
               const SPrimes& sPrimes,
               IntegerMath& intMath,
               Mod& mod );

  };
