// Copyright Eric Chauvin 2021 - 2022.



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html


#include "../CppBase/Casting.h"
#include "../CppBase/CharBuf.h"
#include "Randomish.h"

#include "Integer.h"


Integer::Integer( void )
{
setToZero();
}


// The copy constructor.
Integer::Integer( const Integer& in )
{
// Make the compiler think in is being used.
if( in.testforCopy == 7 )
  return;

throw "Don't use the Integer copy constructor.";
}


/*
Integer::~Integer( void )
{
delete[] D;
}
*/



void Integer::setToMaxValue( void )
{
negative = false;
index = ProjConst::digitArraySize - 1;
for( Int32 count = 0; count <=
                    ProjConst::digitArraySize - 1;
                    count++ )
  D[count] = 0xFFFFFF;

}



void Integer::copy( const Integer& from )
{
negative = from.negative;

const Int32 last = from.index;

index = last;
for( Int32 count = 0; count <= last; count++ )
      D[count] = from.D[count];

}



void Integer::copyUpTo( const Integer& from,
                        const Int32 where )
{
// RangeC::test( where, 0,
//               ProjConst::digitArraySize - 1,
//                "Integer.copyUpTo() range." );

negative = from.negative;
index = where;
for( Int32 count = 0; count <= where; count++ )
  D[count] = from.D[count];

}



bool Integer::isEqualToInt24( const Int32 toTest )
                                           const
{
RangeC::test2( toTest, 0, 0xFFFFFF,
              "Integer.isEqualToInt24() size." );

if( negative )
  return false;

if( index > 0 )
  return false;

if( D[0] != toTest )
  return false;

return true;
}



bool Integer::isEqualToInt48( const Int64 toTest )
                                          const
{
if( negative )
  return false;

if( index > 1 )
  return false;

if( D[0] != (toTest & 0xFFFFFF))
  return false;

Int64 shifted = toTest >> 24;
if( index == 0 )
  {
  if( shifted == 0 )
    return true;
  else
    return false;

  }

if( shifted != D[1] )
  return false;

return true;
}



bool Integer::isEqual( const Integer& x ) const
{
if( negative != x.negative )
  return false;

// The quickest way to return false.
if( D[0] != x.D[0] )
  return false;

if( index != x.index )
  return false;

// Starting from 1 because when the numbers
// are close the upper digits are the same, but
// the smaller digits are usually different.
// So it can find digits that are different
// sooner this way.
for( Int32 count = 1; count <= index; count++ )
  {
  if( D[count] != x.D[count] )
    return false;

  }

return true;
}



bool Integer::isLong48( void ) const
{
if( negative )
  return false;

if( index > 1 )
  return false;

return true;
}




Int64 Integer::getAsLong48( void ) const
{
// This returns a number that is at most
// 48 bits.
// This is normally used after calling isLong48().
// It is assumed here that it is a long.
if( index == 0 ) // Then D[1] is undefined.
  return D[0];

Int64 result = D[1] << 24;
result |= D[0];
return result;
}



bool Integer::paramIsGreater( const Integer& x )
                                           const
{
if( negative )
  throw "ParamIsGreater() can't be negative.";

if( x.negative )
  throw "ParamIsGreater() can't be negative (2).";

if( index != x.index )
  {
  if( x.index > index )
    return true;
  else
    return false;

  }

// Indexes are the same:
for( Int32 count = index; count >= 0; count-- )
  {
  if( D[count] != x.D[count] )
    {
    if( x.D[count] > D[count] )
      return true;
    else
      return false;

    }
  }

return false; // It was equal, but not greater.
}



bool Integer::paramIsGreaterOrEq(
                        const Integer& x ) const
{
if( isEqual( x ))
  return true;

return paramIsGreater( x );
}



void Integer::increment( void )
{
D[0] += 1;
if( (D[0] >> 24) == 0 )
  {
  // If there's nothing to Carry then no
  // reorganization is needed.
  return;
  }

Int64 carry = D[0] >> 24;
D[0] = D[0] & 0xFFFFFF;
for( Int32 count = 1; count <= index; count++ )
  {
  Int64 total = carry + D[count];
  D[count] = total & 0xFFFFFF;
  carry = total >> 24;
  }

if( carry != 0 )
  {
  index++;
  if( index >= ProjConst::digitArraySize )
    throw  "Integer.Increment() overflow.";

  D[index] = carry;
  }
}



void Integer::addLong48( Int64 toAdd )
{
RangeC::test2( toAdd, 0, 0xFFFFFFFFFFFFL,
        "Integer.addLong48() toAdd range." );

D[0] += toAdd & 0xFFFFFF;
if( index == 0 ) // Then D[1] would be an
  {              // undefined value.
  D[1] = toAdd >> 24;
  if( D[1] != 0 )
    index = 1;

  }
else
  {
  D[1] += toAdd >> 24;
  }

if( (D[0] >> 24) == 0 )
  {
  if( index == 0 )
    return; // Nothing to Carry.

  if( (D[1] >> 24) == 0 )
    return; // Nothing to Carry.

  }

Int64 carry = D[0] >> 24;
D[0] = D[0] & 0xFFFFFF;

const Int32 max = index;
for( Int32 count = 1; count <= max; count++ )
  {
  Int64 total = carry + D[count];
  D[count] = total & 0xFFFFFF;
  carry = total >> 24;
  }

if( carry != 0 )
  {
  index++;
  if( index >= ProjConst::digitArraySize )
    throw  "Integer.addLong48() overflow.";

  D[index] = carry;
  }
}



void Integer::add( const Integer& toAdd )
{
// There is a separate IntegerMath.Add() that
// is a wrapper to handle negative numbers too.
// if( negative )
// if( toAdd.negative )

if( toAdd.isLong48() )
  {
  addLong48( toAdd.getAsLong48() );
  return;
  }

const Int32 thisIndex = index;
const Int32 toAddIndex = toAdd.index;
if( thisIndex < toAddIndex )
  {
  for( Int32 count = thisIndex + 1;
                  count <= toAddIndex; count++ )
    D[count] = toAdd.D[count];

  for( Int32 count = 0; count <= thisIndex;
                                      count++ )
    D[count] += toAdd.D[count];

  index = toAddIndex;
  }
else
  {
  for( Int32 count = 0; count <= toAddIndex;
                                         count++ )
    D[count] += toAdd.D[count];

  }

Int64 carry = D[0] >> 24;
D[0] = D[0] & 0xFFFFFF;

const Int32 max = index;
for( Int32 count = 1; count <= max; count++ )
  {
  Int64 total = carry + D[count];
  D[count] = total & 0xFFFFFF;
  carry = total >> 24;
  }

if( carry != 0 )
  {
  index++;
  if( index >= ProjConst::digitArraySize )
    throw  "Integer.add() overflow.";

  D[index] = carry;
  }
}



// This is an optimization for small squares.
void Integer::square0( void )
{
// If this got called then Index is 0.
Int64 square = D[0] * D[0];
D[0] = square & 0xFFFFFF;
D[1] = square >> 24;
if( D[1] != 0 )
  index = 1;

}



void Integer::square1( void )
{
// If this got called then Index is 1.
Int64 D0 = D[0];
Int64 D1 = D[1];

// If you were multiplying 23 * 23 on paper
// it would look like:
//                            2     3
//                            2     3
//                           3*2   3*3
//                     2*2   2*3

// And then you add up the columns.
//                           D1    D0
//                           D1    D0
//                         M1_0  M0_0
//                   M2_1  M1_1

// Top row:
Int64 M0_0 = D0 * D0;
Int64 M1_0 = D0 * D1;

// Second row:
// Int64 M1_1 = M1_0; // Avoiding D1 * D0 again.
Int64 M2_1 = D1 * D1;

// Add them up:
D[0] = M0_0 & 0xFFFFFF;
Int64 carry = M0_0 >> 24;

Int64 M0Right = M1_0 & 0xFFFFFF;
Int64 M0Left = M1_0 >> 24;

// Avoiding a redundancy:
// M1_1 is the same as M1_0.

Int64 total = M0Right + M0Right + carry;
D[1] = total & 0xFFFFFF;
carry = total >> 24;
carry += M0Left + M0Left;
total = (M2_1 & 0xFFFFFF) + carry;
D[2] = total & 0xFFFFFF;
carry = total >> 24;
carry += (M2_1 >> 24);
index = 2;
if( carry != 0 )
  {
  index++;
  D[3] = carry;
  }

// Bitwise multiplication with two bits is:
//       1  1
//       1  1
//     ------
//       1  1
//    1  1
// ----------
// 1  0  0  1
// Biggest bit is at position 3 (zero based index).
// Adding Indexes: (1 + 1) + 1.

//       1  0
//       1  0
//       0  0
//    1  0
//    1  0  0
// Biggest bit is at 2.
// Adding Indexes: (1 + 1).

// 7 * 7 = 49
//                 1  1  1
//                 1  1  1
//                --------
//                 1  1  1
//              1  1  1
//           1  1  1
//          --------------
//        1  1  0  0  0  1
//       32 16           1 = 49
// Biggest bit is at 5 (2 + 2) + 1.
// The highest bit is at either index +
// index or it's at index + index + 1.

// For this Integer class the Index might have to
// be incremented once for a Carry, but not more
// than once.
}



void Integer::square2( void )
{
// If this got called then Index is 2.
Int64 D0 = D[0];
Int64 D1 = D[1];
Int64 D2 = D[2];

//                   M2_0   M1_0  M0_0
//            M3_1   M2_1   M1_1
//     M4_2   M3_2   M2_2

// Top row:
Int64 M0_0 = D0 * D0;
Int64 M1_0 = D0 * D1;
Int64 M2_0 = D0 * D2;

// Second row:
// M1_1 = M1_0;
Int64 M2_1 = D1 * D1;
Int64 M3_1 = D1 * D2;

// Third row:
// M2_2 = M2_0;
// M3_2 = M3_1;
Int64 M4_2 = D2 * D2;

// Add them up:
D[0] = M0_0 & 0xFFFFFF;
Int64 carry = M0_0 >> 24;

// D[1]
Int64 M0Right = M1_0 & 0xFFFFFF;
Int64 M0Left = M1_0 >> 24;
// M1Right = M1_1 & 0xFFFFFF;
// M1Left = M1_1 >> 24;
Int64 total = M0Right + M0Right + carry;
D[1] = total & 0xFFFFFF;
carry = total >> 24;
carry += M0Left + M0Left;

// D[2]
M0Right = M2_0 & 0xFFFFFF;
M0Left = M2_0 >> 24;
Int64 M1Right = M2_1 & 0xFFFFFF;
Int64 M1Left = M2_1 >> 24;
// M2Right = M2_2 & 0xFFFFFF;
// M2Left = M2_2 >> 24;
total = M0Right + M1Right + M0Right + carry;
D[2] = total & 0xFFFFFF;
carry = total >> 24;
carry += M0Left + M1Left + M0Left;

// D[3]
M1Right = M3_1 & 0xFFFFFF;
M1Left = M3_1 >> 24;
// M2Right = M3_2 & 0xFFFFFF;
// M2Left = M3_2 >> 24;
total = M1Right + M1Right + carry;
D[3] = total & 0xFFFFFF;
carry = total >> 24;
carry += M1Left + M1Left;

// D[4]
Int64 M2Right = M4_2 & 0xFFFFFF;
Int64 M2Left = M4_2 >> 24;
total = M2Right + carry;
D[4] = total & 0xFFFFFF;
carry = total >> 24;
carry += M2Left;
index = 4;
if( carry != 0 )
  {
  index++;
  D[5] = carry;
  }
}



void Integer::shiftLeft( const Int32 shiftBy )
{
// This one is not meant to shift more than 24
// bits at a time.  Obviously you could call it
// several times.
// Or put a wrapper function around this that
// calls it several times.
if( shiftBy > 24 )
      throw "ShiftBy > 24 on ShiftLeft.";

Int64 carry = 0;
for( Int32 count = 0; count <= index; count++ )
  {
  Int64 digit = D[count];
  digit <<= shiftBy;
  D[count] = digit & 0xFFFFFF;
  D[count] |= carry;
  carry = digit >> 24;
  }

if( carry != 0 )
  {
  index++;
  if( index >= ProjConst::digitArraySize )
    throw "ShiftLeft overflowed.";

  D[index] = carry;
  }
}




void Integer::shiftRight( const Int32 shiftBy )
{
if( shiftBy > 24 )
  throw "ShiftBy > 24 on ShiftRight.";

Int64 carry = 0;
for( Int32 count = index; count >= 0; count-- )
  {
  Int64 digit = D[count] << 24;
  digit >>= shiftBy;
  D[count] = digit >> 24;
  D[count] |= carry;
  carry = digit & 0xFFFFFF;
  }

if( D[index] == 0 )
  {
  if( index > 0 )
    index--;

  }

// Let it shift bits over the edge.
// if( Carry != 0 )
  // throw "ShiftRight() Carry not zero.";

}




// This is used in some algorithms to set one
// particular digit and have all other digits set
// to zero.
void Integer::setDigitAndClear( const Int32 where,
                                const Int64 toSet )
{
// For testing:
// This would lead to an undefined number that's
// zero but not zero since the Index isn't zero.
if( (toSet == 0) && (where != 0) )
  throw "setDigitAndClear() with bad zero.";

if( (toSet >> 24) != 0 )
  throw "setDigitAndClear() Bad stuff...";

index = where;
D[index] = toSet;
for( Int32 count = 0; count < index; count++ )
  D[count] = 0;

}



bool Integer::makeRandomOdd(
                        const Int32 setToIndex )
{
negative = false;
if( setToIndex > (ProjConst::digitArraySize - 3))
  throw "MakeRandomOdd index is too high.";

Int32 howManyBytes = (setToIndex * 3) + 3;

Uint16Array cArray;
Randomish::makeRandomBytes( cArray,
                            howManyBytes );

index = setToIndex;
Int32 where = 0;
for( Int32 count = 0; count <= setToIndex;
                                        count++ )
  {
  Int64 digit = Casting::U16ToI32(
                           cArray.getVal( where ));
  // Test that it is getting that top bit
  // in the byte in some of them.
  // if( (digit & 0x80) != 0 )
    // throw "Yes, it got the bit.";

  digit <<= 8;
  digit |= Casting::U16ToI32(
                     cArray.getVal( where + 1 ));
  digit <<= 8;
  digit |= Casting::U16ToI32(
                     cArray.getVal( where + 2 ));

  D[count] = digit;
  where += 3;
  }

// Make sure there isn't a zero at the top.
if( D[index] == 0 )
  throw "zero at top of random bytes.";
  // return false;

if( (D[index] >> 16) == 0 )
  throw "zero at very top of random bytes.";
  // return false;

// Test:
for( Int32 count = 0; count <= index; count++ )
  {
  if( (D[count] >> 24) != 0 )
    throw "(D[Count] >> 24) != 0 MakeRandom().";

  }

D[0] |= 1; // Make it odd.
return true;
}



/*
void Integer::setOneDValueFromChar( Uint64 toSet,
                Uint32 position, Uint32 offset )
{
// These are ASCII values so they're between 32
// and 127.
if( position >= ProjConst::digitArraySize )
  return;

if( offset == 1 )
  toSet <<= 8;

if( offset == 2 )
  toSet <<= 16;

if( offset == 3 )
  toSet <<= 24;

// This assumes I'm setting them from zero upward.
if( offset == 0 )
  D[position] = toSet;
else
  D[position] |= toSet;

if( index < position )
  index = position;

}



char Integer::getOneCharFromDValue(
                Uint32 position, Uint32 offset )
{
// These are ASCII values so they're between 32
// and 127.
if( position >= ProjConst::digitArraySize )
  return (char)0;

if( offset == 0 )
  return (char)(D[position] & 0xFF);

if( offset == 1 )
  return (char)((D[position] >> 8) & 0xFF);

if( offset == 2 )
  return (char)((D[position] >> 16) & 0xFF);

if( offset == 3 )
  return (char)((D[position] >> 24) & 0xFF);

return (char)0; // This should never happen.
}



bool Integer::setFromAsciiStr( Str& in )
{
isNegative = false;
index = 0;
const Uint32 inLen = in.getSize();

if( inLen > (ProjConst::digitArraySize - 3))
  return false;

for( Uint32 count = 0; count <
                        ProjConst::digitArraySize;
                        count++ )
  D[count] = 0;

Uint32 oneChar = 0;
Uint32 position = 0;
Uint32 offset = 0;
for( Uint32 count = 0; count < inLen; count++ )
  {
  oneChar = in.charAt( count );
  setOneDValueFromChar( oneChar, position,
                                        offset );
  if( offset == 3 )
    position++;

  offset++;
  offset = offset % 4;
  // offset = offset & 0x3;
  }

return true;
}



Str Integer::getAsciiStr( void )
{
CharBuf cBuf;
for( Uint32 count = 0; count <= index; count++ )
  {
  Uint32 offset = 0;
  char oneChar = getOneCharFromDValue( count,
                                         offset );
  if( oneChar >= ' ' )
    cBuf.appendChar( oneChar );

  offset = 1;
  oneChar = getOneCharFromDValue( count, offset );
  // It could be missing upper characters at
  // the top, so they'd be zero.
  if( oneChar >= ' ' )
    cBuf.appendChar( oneChar );

  offset = 2;
  oneChar = getOneCharFromDValue( count, offset );
  if( oneChar >= ' ' )
    cBuf.appendChar( oneChar );

  offset = 3;
  oneChar = getOneCharFromDValue( count, offset );
  if( oneChar >= ' ' )
    cBuf.appendChar( oneChar );

  }

// StIO::printFS(
//        "Before two copy constructors.\n" );

return cBuf.getStr();
}




void Integer::setOneDValueFromByte( Uint64 toSet,
                  Uint32 setIndex, Uint32 offset )
{
if( setIndex >= ProjConst::digitArraySize )
  throw "SetIndex >= digitArraySize. setOneVal";

if( offset == 1 )
  toSet <<= 8;

if( offset == 2 )
  toSet <<= 16;

if( offset == 3 )
  toSet <<= 24;

// This assumes I'm setting them from zero upward.
// So if the position is zero it's not ORed with
// the value at D.
if( offset == 0 )
  D[setIndex] = toSet;
else
  D[setIndex] |= toSet;

if( index < setIndex )
  index = setIndex;

}



char Integer::getOneByteFromDValue(
                  Uint32 atIndex, Uint32 offset )
{
if( atIndex >= ProjConst::digitArraySize )
  throw "atIndex >= digitArraySize getOneByte.";

if( offset == 0 )
  return (char)(D[atIndex] & 0xFF);

if( offset == 1 )
  return (char)((D[atIndex] >> 8) & 0xFF);

if( offset == 2 )
  return (char)((D[atIndex] >> 16) & 0xFF);

if( offset == 3 )
  return (char)((D[atIndex] >> 24) & 0xFF);

throw "offset not right in GetOneByteFromDValue.";
}




// Because of the standards used with TLS,
// this will typically have one leading zero
// byte so that it doesn't get confused with
// a negative sign bit.  Sometimes it will,
// sometimes it won't.
void Integer::setFromBigEndianByteArray(
                              Uint8Array& U8Ar )
{
if( U8Ar.isAllZero())
  {
  setToZero();
  return;
  }

const Uint32 howMany = U8Ar.getLast();

if( howMany > (ProjConst::digitArraySize - 3))
  throw "digitArraySize SetFromBigEndian";

isNegative = false;
index = 0;

for( Uint32 count = 0; count <
                       ProjConst::digitArraySize;
                       count++ )
  D[count] = 0;

U8Ar.reverse(); // Now the least significant
                // byte is at zero.
Uint32 offset = 0;
Uint32 setIndex = 0;
for( Uint32 count = 0; count < howMany; count++ )
  {
  Uint32 toSet = U8Ar.valAt( count );
  setOneDValueFromByte( toSet, setIndex, offset );
  offset++;
  if( (offset & 3) == 0 )
    {
    offset = 0;
    setIndex++;
    }
  }

// Make sure it doesn't have leading zeros.
for( Int32 count = index; count >= 0; count-- )
  {
  if( D[count] != 0 )
    {
    index = count;
    break;
    }
  }
}


void Integer::getBigEndianByteArray(
                             Uint8Array& toGet )
{
toGet.clear();

for( Uint32 count = 0; count <= index; count++ )
  {
  for( Uint32 offset = 0; offset < 4; offset++ )
    {
    Uint8 oneByte = getOneByteFromDValue( count,
                                       offset );
    toGet.appendVal( oneByte );
    }
  }

toGet.reverse();

// Now the most significant byte is at Result[0].
// toGet might have leading zero bytes here.
}

*/



void Integer::copyFromIntBuf(
                          const IntBuf& intBuf )
{
negative = false;
index = intBuf.getIndex();
const Int32 max = index;
for( Int32 count = 0; count <= max; count++ )
  D[count] = intBuf.getD( count );

}


void Integer::copyToIntBuf( IntBuf& intBuf ) const

{
intBuf.setIndex( index );
for( Int32 count = 0; count <= index; count++ )
  intBuf.setD( count, Casting::i64ToI32(
                                   D[count] ));

}




void Integer::cleanUp( void )
{
Int64 carry = D[0] >> 24;
D[0] = D[0] & 0xFFFFFF;
for( Int32 count = 1; count <= index; count++ )
  {
  Int64 total = carry + D[count];
  D[count] = total & 0xFFFFFF;
  carry = total >> 24;
  }

if( carry != 0 )
  {
  index++;
  if( index >= ProjConst::digitArraySize )
    throw  "Integer.cleanUp() overflow.";

  D[index] = carry;
  }
}
