// Copyright Eric Chauvin 2021 - 2022.
// This copyright notice has to stay at the top.


// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html



#include "../LinuxApi/Casting.h"
#include "../LinuxApi/CharBuf.h"

#include "IntegerMath.h"
#include "Base10Number.h"
#include "Division.h"



IntegerMath::IntegerMath( void )
{
scratch = new Int64[ProjConst::digitArraySize];
}



IntegerMath::IntegerMath( const IntegerMath& in )
{
scratch = new Int64[ProjConst::digitArraySize];

// Make the compiler think in is being used.
if( in.testForCopy == 7 )
  return;

throw "Copy constructor: IntegerMath.";
}


IntegerMath::~IntegerMath( void )
{
delete[] scratch;
}


// static
Int64 IntegerMath::find64SqrRoot2(
                          const Int64 toMatch )
{
// The result has to be a 24 bit number max.
// So toMatch can't be bigger than this.
RangeC::test2( toMatch, 0, 0xFFFFFFFFFFFFL,
      "IntegerMath.find64SqrRoot() toMatch." );

if( toMatch == 0 )
  return 0;

// 3 = 0b011
if( toMatch < 4 )
  return 1;

if( toMatch < 9 )
  return 2;

// 4 * 4 = 16
// 4 =    0b0100
// 16 = 0b010000
if( toMatch < 16 )
  return 3;

// For 24 bits.
Int64 oneBit = 0x800000;

Int64 result = 0;
for( Int32 count = 0; count < 24; count++ )
  {
  Int64 toTry = result | oneBit;
  if( (toTry * toTry) <= toMatch )
    result |= oneBit; // Then I want the bit.

  oneBit >>= 1;
  }

// It can't be more than 24 bits.
RangeC::test2( result, 0, 0xFFFFFFL,
    "intMath Find64SqrRoot() Result range." );

RangeC::test2( result * result, 0, toMatch,
        "intMath Find64SqrRoot() too high." );

// 0xFF FFFFFF FFFFFFL is a lot bigger than
// this can be.  So it is only checking against
// the low of toMatch + 1.

RangeC::test2( (result + 1) * (result + 1),
             toMatch + 1, 0xFFFFFFFFFFFFFFL,
        "intMath FindL64SqrRoot() too low." );

return result;
}



Int32 IntegerMath::isDivisibleBySmallPrime(
                         const Integer& toTest,
                         const SPrimes& sPrimes )
{
if( (toTest.getD( 0 ) & 1) == 0 )
  return 2; // It's divisible by 2.

// Don't make this smaller than primesArraySize.
// Primality testing and other things depend
// on it.
for( Int32 count = 1; count <
             ProjConst::primesArraySize; count++ )
  {
  Int32 prime = sPrimes.getPrimeAt( count );
  if( 0 == getMod24( toTest, prime ))
    return prime;

  }

// No small primes divide it.
return 0;
}



void IntegerMath::subLong48( Integer& result,
                             const Int64 toSub )
{
if( toSub < 0 )
  throw "subLong48() toSub < 0.";

if( toSub > 0xFFFFFFFFFFFFL )
  throw "subLong48() toSub > 0xFFFFFFFFFFFF.";

if( result.getNegative())
  throw "subLong48() result is negative.";

if( result.isLong48())
  {
  Int64 resultL = result.getAsLong48();
  if( toSub == resultL )
    {
    result.setToZero();
    return;
    }

  if( toSub > resultL )
    throw "IsLong48() and toSub > resultL.";

  resultL = resultL - toSub;
  result.setD( 0, resultL & 0xFFFFFF );
  result.setD( 1, resultL >> 24 );
  if( result.getD( 1 ) == 0 )
    result.setIndex( 0 );
  else
    result.setIndex( 1 );

  return;
  }

// If it got this far then Index is at least 2.
// So it can't be equal to or less than the
// long48.

scratch[0] = result.getD( 0 ) -
             (toSub & 0xFFFFFF);
scratch[1] = result.getD( 1 ) -
             (toSub >> 24);
if( (scratch[0] >= 0) && (scratch[1] >= 0) )
  {
  // No need to reorganize it.
  result.setD( 0, scratch[0] );
  result.setD( 1, scratch[1] );
  return;
  }

// last is at least 2 here.
const Int32 last = result.getIndex();

for( Int32 count = 2; count <= last; count++ )
  scratch[count] = result.getD( count );

for( Int32 count = 0; count < last; count++ )
  {
  if( scratch[count] < 0 )
    {
    // For subtracting on paper this would be
    // called Borrowing from the next higher
    // digit.
    scratch[count] += 0xFFFFFF + 1;
    scratch[count + 1]--;
    }
  }

// This can't happen because result is bigger
// than toSub.
if( scratch[last] < 0 )
  throw "Overflow SubLong48() scratch[last] < 0.";

for( Int32 count = 0; count <= last; count++ )
  result.setD( count, scratch[count] );

for( Int32 count = last; count >= 0; count-- )
  {
  if( result.getD( count ) != 0 )
    {
    result.setIndex( count );
    return;
    }
  }

// If this was zero it wouldn't find a nonzero
// digit to set the Index to and it would end
// up down here.
result.setIndex( 0 );
}



void IntegerMath::add( Integer& result,
                       const Integer& toAdd )
{
if( toAdd.isZero())
  return;

// The most common form.  They are both positive.
if( !result.getNegative() &&
    !toAdd.getNegative() )
  {
  result.add( toAdd );
  return;
  }

Integer add1;
Integer add2;

if( !result.getNegative() &&
     toAdd.getNegative() )
  {
  add1.copy( toAdd );
  add1.setNegative( false );
  if( add1.isEqual( result ))
    {
    result.setToZero();
    return;
    }

  if( add1.paramIsGreater( result ))
    {
    subtract( result, add1 );
    return;
    }
  else
    {
    subtract( add1, result );
    result.copy( add1 );
    result.setNegative( true );
    return;
    }
  }

if( result.getNegative() &&
    !toAdd.getNegative() )
  {
  add1.copy( result );
  add1.setNegative( false );
  if( add1.isEqual( toAdd ))
    {
    result.setToZero();
    return;
    }

  add2.copy( toAdd );
  if( add1.paramIsGreater( add2 ))
    {
    subtract( add2, add1 );
    result.copy( add2 );
    return;
    }
  else
    {
    subtract( add1, add2 );
    result.copy( add2 );
    result.setNegative( true );
    return;
    }
  }

if( result.getNegative() &&
    toAdd.getNegative() )
  {
  add1.copy( result );
  add1.setNegative( false );
  add2.copy( toAdd );
  add2.setNegative( false );
  add1.add( add2 );
  result.copy( add1 );
  result.setNegative( true );
  return;
  }
}




void IntegerMath::subtract( Integer& result,
                         const Integer& toSub )
{
if( toSub.isZero())
  return;

// This checks that the sign is equal too.
if( result.isEqual( toSub ))
  {
  //  3 -  3 = 0
  // -3 - -3 = 0
  result.setToZero();
  return;
  }

// Equal absolute value but opposite sign.
//  3 - -3 = 6
// -3 - 3 = -6

Integer tempSub1;
Integer tempSub2;

// paramIsGreater() handles positive and
// negative values, so if the parameter is
// more toward the positive side then it's
// true.  It's greater.
// The most common form.  They are both positive.
if( !result.getNegative() &&
    !toSub.getNegative() )
  {
  if( toSub.paramIsGreater( result ))
    {
    subtractPositive( result, toSub );
    return;
    }

  // toSub is bigger.
  tempSub1.copy( result );
  tempSub2.copy( toSub );
  subtractPositive( tempSub2, tempSub1 );
  result.copy( tempSub2 );
  result.setNegative( true );
  return;
  }

if( result.getNegative() &&
    !toSub.getNegative() )
  {
  tempSub1.copy( result );
  tempSub1.setNegative( false );
  tempSub1.add( toSub );
  result.copy( tempSub1 );
  result.setNegative( true );
  return;
  }

if( !result.getNegative() &&
    toSub.getNegative() )
  {
  tempSub1.copy( toSub );
  tempSub1.setNegative( false );
  result.add( tempSub1 );
  return;
  }

if( result.getNegative() &&
    toSub.getNegative() )
  {
  tempSub1.copy( result );
  tempSub1.setNegative( false );
  tempSub2.copy( toSub );
  tempSub2.setNegative( false );
  // -12 - -7 = -12 + 7 = -5
  // Comparing the positive numbers here.
  if( tempSub2.paramIsGreater( tempSub1 ))
    {
    subtractPositive( tempSub1, tempSub2 );
    result.copy( tempSub1 );
    result.setNegative( true );
    return;
    }

  // -7 - -12 = -7 + 12 = 5
  subtractPositive( tempSub2, tempSub1 );
  result.copy( tempSub2 );
  result.setNegative( false );
  return;
  }
}



void IntegerMath::subtractPositive(
                           Integer& result,
                           const Integer& toSub )
{
if( toSub.isLong48() )
  {
  subLong48( result, toSub.getAsLong48());
  return;
  }

if( toSub.getIndex() > result.getIndex() )
  throw "In Subtract() ToSub.Index > Index.";

Int32 const last = toSub.getIndex();
for( Int32 count = 0; count <= last; count++ )
  scratch[count] = result.getD( count )
                 - toSub.getD( count );

const Int32 lastR = result.getIndex();

for( Int32 count = last + 1; count <= lastR;
                                       count++ )
  scratch[count] = result.getD( count );

for( Int32 count = 0; count < lastR; count++ )
  {
  if( scratch[count] < 0 )
    {
    scratch[count] += 0xFFFFFF + 1;
    scratch[count + 1]--;
    }
  }

if( scratch[lastR] < 0 )
  throw "Subtract() overflow.";

for( Int32 count = 0; count <= lastR; count++ )
  result.setD( count, scratch[count] );

for( Int32 count = lastR; count >= 0; count-- )
  {
  if( result.getD( count ) != 0 )
    {
    result.setIndex( count );
    return;
    }
  }

// If it never found a non-zero digit it
// would get down to here.
result.setIndex( 0 );
}



void IntegerMath::multiplyInt( Integer& result,
                             const Int64 toMul )
{
RangeC::test2( toMul, 0, 0xFFFFFF,
   "IntegerMath.multiplyInt( toMul range." );

if( toMul == 0 )
  {
  result.setToZero();
  return;
  }

if( toMul == 1 )
  return;

// If result was negative then it would just
// stay negative.
const Int32 last = result.getIndex();
for( Int32 column = 0; column <= last; column++ )
  {
  M.setV( column, 0, toMul *
                      result.getD( column ));
  }

// Add these up with a carry.
result.setD( 0, M.getV( 0, 0 ) & 0xFFFFFF );
Int64 carry = M.getV( 0, 0 ) >> 24;
for( Int32 column = 1; column <= last; column++ )
  {
  Int64 mValue = M.getV( column, 0 );
  Int64 totalRight = mValue & 0xFFFFFF;
  Int64 totalLeft = mValue >> 24;

  totalRight += carry;
  result.setD( column, totalRight & 0xFFFFFF );
  carry = totalRight >> 24;
  carry += totalLeft;
  }

if( carry != 0 )
  {
  result.incrementIndex();
  result.setD( result.getIndex(), carry );
  }
}



Int32 IntegerMath::multInt24FromCopy(
                          Integer& result,
                          const Integer& from,
                          const Int64 toMul )
{
RangeC::test2( toMul, 0, 0xFFFFFF,
  "IntegerMath.multInt24FromCopy() toMul range." );

const Int32 fromCopyIndex = from.getIndex();
result.setIndex( fromCopyIndex );
for( Int32 column = 0; column <= fromCopyIndex;
                                       column++ )
  scratch[column] = toMul * from.getD( column );

// Add these up with a carry.
result.setD( 0, scratch[0] & 0xFFFFFF );
Int64 carry = scratch[0] >> 24;
for( Int32 column = 1; column <= fromCopyIndex;
                                       column++ )
  {
  Int64 total = scratch[column] + carry;
  result.setD( column, total & 0xFFFFFF );
  carry = total >> 24;
  }

if( carry != 0 )
  {
  result.incrementIndex();
  result.setD( fromCopyIndex + 1, carry );
  }

return result.getIndex();
}




void IntegerMath::multiplyLong48( Integer& result,
                            const Int64 toMul )
{
RangeC::test2( toMul, 0, 0xFFFFFFFFFFFFL,
  "IntegerMath.multiplyLong48() toMul range." );

if( result.isZero())
  return;

if( toMul == 0 )
  {
  result.setToZero();
  return;
  }

if( toMul == 1 )
  return;

Int64 B0 = toMul & 0xFFFFFF;
Int64 B1 = toMul >> 24;
if( B1 == 0 )
  {
  multiplyInt( result, B0 );
  return;
  }

// Since B1 is not zero:
if( (result.getIndex() + 1) >=
                        ProjConst::digitArraySize )
  throw "Overflow in MultiplyLong48().";

Int32 countTo = result.getIndex();
for( Int32 column = 0; column <= countTo;
                                       column++ )
  {
  Int64 digit = result.getD( column );
  M.setV( column, 0, B0 * digit );
  // Column + 1 and Row is 1, so it's just like
  // pen and paper.
  M.setV( column + 1, 1, B1 * digit );
  }

// Since B1 is not zero, the index is set one
// higher.
result.incrementIndex();
M.setV( result.getIndex(), 0, 0 );

// Add these up with a carry.
result.setD( 0, M.getV( 0, 0 ) & 0xFFFFFF );
Int64 carry = M.getV( 0, 0 ) >> 24;
countTo = result.getIndex();
for( Int32 column = 1; column <= countTo;
                                       column++ )
  {
  Int64 totalLeft = 0;
  Int64 totalRight = 0;
  // There are only the two rows for this.
  for( Int32 row = 0; row <= 1; row++ )
    {
    Int64 mValue = M.getV( column, row );
    totalRight += mValue & 0xFFFFFF;
    totalLeft += mValue >> 24;
    }

  totalRight += carry;
  result.setD( column, totalRight & 0xFFFFFF );
  carry = totalRight >> 24;
  carry += totalLeft;
  }

if( carry != 0 )
  {
  result.incrementIndex();
  result.setD( result.getIndex(), carry );
  }
}



void IntegerMath::setMultiplySign(
                           Integer& result,
                           const Integer& toMul )
{
if( result.getNegative() ==
    toMul.getNegative() )
  result.setNegative( false );
else
  result.setNegative( true );

}



void IntegerMath::multiply( Integer& result,
                         const Integer& toMul )
{
if( result.isZero())
  return;

if( toMul.isLong48())
  {
  multiplyLong48( result, toMul.getAsLong48());
  setMultiplySign( result, toMul );
  return;
  }

// It could never get here if ToMul is zero
// because getAsLong48()
// would be true for zero.
const Int32 totalIndex = result.getIndex() +
                                toMul.getIndex();
if( totalIndex >= ProjConst::digitArraySize )
  throw "Multiply() overflow.";

const Int32 countTo = toMul.getIndex();
for( Int32 row = 0; row <= countTo; row++ )
  {
  if( toMul.getD( row ) == 0 )
    {
    const Int32 countZeros = result.getIndex();
    for( Int32 column = 0; column <= countZeros;
                                        column++ )
      {
      M.setV( column + row, row, 0 );
      }
    }
  else
    {
    const Int32 countMult = result.getIndex();
    for( Int32 column = 0; column <= countMult;
                                        column++ )
      {
      M.setV( column + row, row,
                           toMul.getD( row ) *
                           result.getD( column ));
      }
    }
  }

// Add the columns up with a carry.
const Int64 zeroPart = M.getV( 0, 0 );
result.setD( 0, zeroPart & 0xFFFFFF );
Int64 carry = zeroPart >> 24;
const Int32 resultIndex = result.getIndex();
const Int32 mulIndex = toMul.getIndex();
for( Int32 column = 1; column <= totalIndex;
                                      column++ )
  {
  Int64 totalLeft = 0;
  Int64 totalRight = 0;
  for( Int32 row = 0; row <= mulIndex; row++ )
    {
    if( row > column )
      break;

    if( column > (resultIndex + row) )
      continue;

    const Int64 totalPart =
                          M.getV( column, row );
    totalRight += totalPart & 0xFFFFFF;
    totalLeft += totalPart >> 24;
    }

  totalRight += carry;
  result.setD( column, totalRight & 0xFFFFFF );
  carry = totalRight >> 24;
  carry += totalLeft;
  }

result.setIndex( totalIndex );
if( carry != 0 )
  {
  result.incrementIndex();
  result.setD( result.getIndex(), carry );
  }

setMultiplySign( result, toMul );
}



/*
void IntegerMath::setFromStr( Integer& result,
                              const Str& in )
{
if( in.getSize() < 1 )
  {
  result.setToZero();
  return;
  }

Base10Number base10N( in );
Integer tens;
Integer onePart;
base10N.setFromStr( in );
result.setFromULong( base10N.getD( 0 ));
tens.setFromULong( 10 );
const Uint32 last = base10N.getIndex();
for( Uint32 count = 1; count <= last; count++ )
  {
  onePart.setFromULong( base10N.getD( count ));
  multiply( onePart, tens );
  result.add( onePart );
  multiplyULong( tens, 10 );
  }
}
*/


Str IntegerMath::toString10( const Integer& from )
{
if( from.isLong48())
  {
  Int64 N = from.getAsLong48();
  Str nS( N );
  if( from.getNegative() )
    {
    Str sign( "-" );
    Str both( sign, nS );
    return both;
    }
  else
    {
    return nS;
    }
  }

Integer toDivide;
toDivide.copy( from );

Integer quotient;
CharBuf cBuf;
while( !toDivide.isZero())
  {
  Int32 digit = Casting::i64ToI32( Division::
       shortDivideRem( toDivide, 10, quotient ));
  toDivide.copy( quotient );
  // Ascii values go from '0' up to '9'.
  cBuf.appendChar( Casting::i32ToChar(
                              '0' + digit ));
  }

if( from.getNegative() )
  cBuf.appendChar( '-' );

Str result = cBuf.getStr();
result.reverse();
return result;
}



void IntegerMath::square( Integer& toSquare )
{
if( toSquare.isZero())
  return;

// If it's negative x then x^2 is positive.
toSquare.setNegative( false );

const Int32 sqrIndex = toSquare.getIndex();
if( sqrIndex == 0 )
  {
  toSquare.square0();
  return;
  }

if( sqrIndex == 1 )
  {
  toSquare.square1();
  return;
  }

if( sqrIndex == 2 )
  {
  toSquare.square2();
  return;
  }

// Now Index is at least 3:
const Int32 doubleIndex = sqrIndex << 1;
if( doubleIndex >= ProjConst::digitArraySize )
  throw "Square() overflowed.";

const Int32 last = sqrIndex;
for( Int32 row = 0; row <= last; row++ )
  {
  if( toSquare.getD( row ) == 0 )
    {
    for( Int32 column = 0; column <= last;
                                       column++ )
      {
      M.setV( column + row, row, 0 );
      }
    }
  else
    {
    for( Int32 column = 0; column <= last;
                                      column++ )
      {
      M.setV( column + row, row,
                      toSquare.getD( row ) *
                      toSquare.getD( column ));
      }
    }
  }

// Add the columns up with a carry.
toSquare.setD( 0, M.getV( 0, 0 ) & 0xFFFFFF );
Int64 carry = M.getV( 0, 0 ) >> 24;
for( Int32 column = 1; column <= doubleIndex;
                                       column++ )
  {
  Int64 totalLeft = 0;
  Int64 totalRight = 0;
  for( Int32 row = 0; row <= column; row++ )
    {
    if( row > last )
      break;

    if( column > (last + row) )
      continue;

    Int64 totalPart = M.getV( column, row );
    totalRight += totalPart & 0xFFFFFF;
    totalLeft += totalPart >> 24;
    }

  totalRight += carry;
  toSquare.setD( column, totalRight & 0xFFFFFF );
  carry = totalRight >> 24;
  carry += totalLeft;
  }

toSquare.setIndex( doubleIndex );
if( carry != 0 )
  {
  toSquare.incrementIndex();
  toSquare.setD( toSquare.getIndex(), carry );
  }
}




// This is an optimization for multiplying when
// only the top digit of a number has been set
// and all of the other digits are zero.
void IntegerMath::multiplyTop( Integer& result,
                          const Integer& toMul )
{
Int32 totalIndex = result.getIndex() +
                             toMul.getIndex();
if( totalIndex >= ProjConst::digitArraySize )
  throw "MultiplyTop() overflow.";

// Just like Multiply() except that all the
// other rows are zero:
Int32 toMulIndex = toMul.getIndex();
Int32 resultIndex = result.getIndex();
for( Int32 column = 0; column <= toMulIndex;
                                      column++ )
  M.setV( column + resultIndex,
          resultIndex,
          result.getD( resultIndex ) *
          toMul.getD( column ));

for( Int32 column = 0; column < resultIndex;
                                       column++ )
  result.setD( column, 0 );

Int64 carry = 0;
for( Int32 column = 0; column <= toMulIndex;
                                       column++ )
  {
  Int64 total = M.getV( column + resultIndex,
                         resultIndex) + carry;
  result.setD( column + resultIndex,
                           total & 0xFFFFFF );
  carry = total >> 24;
  }

result.setIndex( totalIndex );
if( carry != 0 )
  {
  result.setIndex( result.getIndex() + 1 );
  if( result.getIndex() >=
                        ProjConst::digitArraySize )
    throw "MultiplyTop() overflow.";

  result.setD( result.getIndex(), carry );
  }
}



// This is another optimization.  This is used
// when the top digit is 1 and all of the other
// digits are zero.  This is effectively just a
// shift-left operation.
void IntegerMath::multiplyTopOne(
                          Integer& result,
                          const Integer& toMul )
{
Int32 totalIndex = result.getIndex() +
                                toMul.getIndex();
if( totalIndex >= ProjConst::digitArraySize )
  throw "MultiplyTopOne() overflow.";

Int32 toMulIndex = toMul.getIndex();
Int32 resultIndex = result.getIndex();
for( Int32 column = 0; column <= toMulIndex;
                                       column++ )
  result.setD( column + resultIndex,
                            toMul.getD( column ));

for( Int32 column = 0; column < resultIndex;
                                       column++ )
  result.setD( column, 0 );

// No Carrys need to be done.
result.setIndex( totalIndex );
}




// In the SquareRoot() method SqrRoot.Index is
// half of Square.Index.  Compare this to the
// Square() method where the Carry might or
// might not increment the index to an odd number.
// (So if the Index was 5 its square root would
// have an Index of 5 / 2 = 2.)

void IntegerMath::setsqrRootTop(
                          const Integer& fromSqr,
                          Integer& sqrRoot )
{
Int64 toMatch;

// for( Int32 count = 0; count < 3; count++ )
  // {
  Int32 testIndex = fromSqr.getIndex() >> 1;
  sqrRoot.setDigitAndClear( testIndex, 1 );
  if( (testIndex << 1) >
                    (fromSqr.getIndex() - 1) )
    {
    toMatch = fromSqr.getD( fromSqr.getIndex());
    }
  else
    {
    toMatch = fromSqr.getD(
                      fromSqr.getIndex()) << 24;
    toMatch |= fromSqr.getD(
                      fromSqr.getIndex() - 1 );
    }

  // It could be setting this to zero.
  Int64 topD = find64SqrRoot2( toMatch );
  if( topD == 0 )
    throw
      "This needs work. Need a new squareRoot().";

  sqrRoot.setD( testIndex, topD );
  // }

}



bool IntegerMath::squareRoot(
                          const Integer& fromSqr,
                          Integer& sqrRoot )
{
Int64 toMatch;

if( fromSqr.isLong48() )
  {
  toMatch = fromSqr.getAsLong48();
  if( toMatch == 0 )
    {
    sqrRoot.setToZero();
    return true; // 0 is the square root of zero
                 // I guess is true.
    }

  if( toMatch == 1 )
    {
    sqrRoot.setToOne();
    return true;
    }

  if( toMatch == 2 )
    {
    sqrRoot.setToOne();
    return false;
    }

  if( toMatch == 3 )
    {
    sqrRoot.setToOne();
    return false;
    }

  // find64SqrRoot2 would throw an exception if
  // its result was more than 24 bits.
  sqrRoot.setD( 0, find64SqrRoot2( toMatch ));
  sqrRoot.setIndex( 0 );
  if( (sqrRoot.getD(0 ) * sqrRoot.getD( 0 )) ==
                                        toMatch )
    return true;
  else
    return false;

  }

setsqrRootTop( fromSqr, sqrRoot );


Int32 testIndex = fromSqr.getIndex() >> 1;

/*
sqrRoot.setDigitAndClear( testIndex, 1 );
if( (testIndex << 1) > (fromSqr.getIndex() - 1) )
  {
  toMatch = fromSqr.getD( fromSqr.getIndex());
  }
else
  {
  toMatch = fromSqr.getD(
                      fromSqr.getIndex()) << 24;
  toMatch |= fromSqr.getD(
                      fromSqr.getIndex() - 1 );
  }

// It could be setting this to zero.
Int64 topD = find64SqrRoot2( toMatch );
if( topD == 0 )
  throw
    "This needs work. Need a new squareRoot().";

sqrRoot.setD( testIndex, topD );
*/

testIndex--;
Int32 sqrIndex = 0;

while( true )
  {
  sqrIndex = sqrRoot.getIndex();
  if( sqrRoot.getD( sqrIndex ) == 0 )
    {
    if( sqrIndex != 0 )
      sqrRoot.setIndex( sqrIndex - 1 );

    }

  searchSqrtXPart( testIndex, fromSqr, sqrRoot );
  if( testIndex == 0 )
    break;

  testIndex--;
  }


sqrIndex = sqrRoot.getIndex();
if( sqrRoot.getD( sqrIndex ) == 0 )
  {
  if( sqrIndex != 0 )
    sqrRoot.setIndex( sqrIndex - 1 );

  }

// Avoid squaring the whole thing to see if
// it's an exact square root:
Int64 digitB = sqrRoot.getD( 0 );
if( ((digitB * digitB) &
               0xFFFFFF) != fromSqr.getD( 0 ))
  return false;

Integer testForSquareRoot;

testForSquareRoot.copy( sqrRoot );
square( testForSquareRoot );
if( fromSqr.isEqual( testForSquareRoot ))
  return true;
else
  return false;

}




void IntegerMath::searchSqrtXPart(
                          const Int32 testIndex,
                          const Integer& fromSqr,
                          Integer& sqrRoot )
{
// B is the Big part of the number that has
// already been found.
// S = (B + x)^2
// S = B^2 + 2Bx + x^2
// S - B^2 = 2Bx + x^2
// R = S - B^2  a square minus a square if
//              S is a square.
// R = 2Bx + x^2
// R = x(2B + x)
// What are the two factors of R?

Integer sqrtXPartTest1;
Integer sqrtXPartTest2;
Integer sqrtXPartDiff;
Integer sqrtXPartTwoB;
Integer sqrtXPartR2;

sqrtXPartTest1.copy( sqrRoot ); // B
square( sqrtXPartTest1 ); // B^2
sqrtXPartDiff.copy( fromSqr );
subtract( sqrtXPartDiff, sqrtXPartTest1 );
                                      // S - B^2
sqrtXPartTwoB.copy( sqrRoot ); // B
sqrtXPartTwoB.shiftLeft( 1 ); // Times 2 for 2B.
sqrtXPartTest1.copy( sqrtXPartTwoB );
Int64 testBits = sqrtXPartTest1.getD(
                      sqrtXPartTest1.getIndex());
Int32 shiftBy = Division::findShiftBy( testBits );
sqrtXPartR2.copy( sqrtXPartDiff );
// Multiply the numerator and the denominator
sqrtXPartR2.shiftLeft( shiftBy );
sqrtXPartTest1.shiftLeft( shiftBy );

Int64 highest = 0;
if( sqrtXPartR2.getIndex() == 0 )
  {
  highest = sqrtXPartR2.getD(
                         sqrtXPartR2.getIndex());
  }
else
  {
  highest = sqrtXPartR2.getD(
                    sqrtXPartR2.getIndex()) << 32;
  highest |= sqrtXPartR2.getD(
                    sqrtXPartR2.getIndex() - 1 );
  }

Int64 denom = sqrtXPartTest1.getD(
                     sqrtXPartTest1.getIndex());
if( denom == 0 )
  highest = 0xFFFFFF;
else
  highest = highest / denom;

if( highest == 0 )
  {
  sqrRoot.setD( testIndex, 0 );
  return;
  }

if( highest > 0xFFFFFF )
  highest = 0xFFFFFF;

Int32 bitTest = 0x800000;
Int64 xDigit = 0;
Int64 tempXDigit = 0;
for( Int32 bitCount = 0; bitCount < 24;
                                      bitCount++ )
  {
  tempXDigit = xDigit | bitTest;
  if( tempXDigit > highest )
    {
    bitTest >>= 1;
    continue;
    }

  sqrtXPartTest1.copy( sqrtXPartTwoB );
  sqrtXPartTest1.setD( testIndex, tempXDigit );
                                       // 2B + x
  sqrtXPartTest2.setDigitAndClear( testIndex,
                           tempXDigit ); // Set X.
  multiplyTop( sqrtXPartTest2, sqrtXPartTest1 );
  if( sqrtXPartTest2.paramIsGreaterOrEq(
                                 sqrtXPartDiff ))
    xDigit |= bitTest; // Then keep the bit.

  bitTest >>= 1;
  }

sqrRoot.setD( testIndex, xDigit );
}



Int32 IntegerMath::getMod24( const Integer& in,
                             const Int64 divisor )
{
if( divisor <= 0 )
  throw "getMod24: divisor <= 0.";

if( (divisor >> 24) != 0 )
  throw "getMod24: (divisor >> 24) != 0.";

if( in.getIndex() == 0 )
  {
  Int32 result = Casting::i64ToI32(
                      in.getD( 0 ) % divisor );
  return result;
  }

const Int32 last = in.getIndex();

for( Int32 count = 0; count <= last; count++ )
  scratch[count] = in.getD( count );

// If you had a base-10 number and you wanted
// to find the remainder when divided by a
// single digit from 1 to 9 then you would
// be doing this same thing.
// 7123 divided by 6 is
// 7 / 6 leaves remainder 1.
// 1123 / 6 is
// 11 / 6 leaves remainder 5.
// 523 / 6 is
// 52 / 6 leaves remainder 4.
// 43 / 6 leave remainder 1.


Int64 remainder = 0;
if( divisor <= scratch[last] )
  {
  Int64 oneDigit = scratch[last];
  remainder = oneDigit % divisor;
  scratch[last] = remainder;
  }

for( Int32 count = last; count >= 1; count-- )
  {
  Int64 twoDigits = scratch[count];
  twoDigits <<= 24;
  twoDigits |= scratch[count - 1];
  remainder = twoDigits % divisor;

  // This is not necessary for just a remainder:
  // scratch[count] = 0;

  scratch[count - 1] = remainder;
  }

return Casting::i64ToI32( remainder );
}



Int64 IntegerMath::mod48FromTwoLongs( Int64 p1,
                                  Int64 p0,
                                  Int64 divisor )
{
RangeC::test2( p0, 0, 0xFFFFFFFFFFFFL,
        "IntegerMath.mod48FromTwoLongs() p0." );

RangeC::test2( p1, 0, 0xFFFFFFFFFFFFL,
        "IntegerMath.mod48FromTwoLongs() p1." );

if( divisor <= 0xFFFFFF )
  throw "mod48FromTwoLongs divisor <= ";

p0 = p0 % divisor;

if( p1 == 0 )
  return p0;

// p1 times 2^48
// p1 * (2^12 + 2^12 + 2^12 + 2^12)
// (p1 * 2^12) + (p1 * 2^12) + (p1 * 2^12) +
//                           (p1 * 2^12)

// 48 + 12 = 60 so it fits in 63 bits.

Int64 part1 = p1 % divisor;
part1 <<= 12;  // Times 2^12
part1 = part1 % divisor;
part1 <<= 12;  // Times 2^12     24
part1 = part1 % divisor;
part1 <<= 12;  // Times 2^12     36
part1 = part1 % divisor;
part1 <<= 12;  // Times 2^12     48
part1 = part1 % divisor;

return (part1 + p0) % divisor;
}



Int64 IntegerMath::getMod48( const Integer& in,
                             const Int64 divisor )
{
if( divisor == 0 )
  throw "getMod48: divisor == 0.";

if( (divisor >> 24 ) == 0 )
  return getMod24( in, divisor );


const Int32 last = in.getIndex();

if( last == 0 )
  return in.getD( 0 ); // The divisor is bigger.

Int64 digit1 = 0;
Int64 digit0 = 0;
Int64 remainder = 0;
if( last == 2 )
  {
  digit1 = in.getD( 2 );
  digit0 = in.getD( 1 ) << 24;
  digit0 |= in.getD( 0 );
  return mod48FromTwoLongs( digit1, digit0,
                                      divisor );
  }

if( last == 3 )
  {
  digit1 = in.getD( 3 ) << 24;
  digit1 |= in.getD( 2 );
  digit0 = in.getD( 1 ) << 24;
  digit0 |= in.getD( 0 );
  return mod48FromTwoLongs( digit1, digit0,
                                     divisor );
  }

for( Int32 count = 0; count <= last; count++ )
  scratch[count] = in.getD( count );

Int32 where = last;
while( true )
  {
  if( where <= 3 )
    {
    if( where < 2 ) // This can't happen.
      throw "GetMod48(): Where < 2.";

    if( where == 2 )
      {
      digit1 = scratch[2];
      digit0 = scratch[1] << 24;
      digit0 |= scratch[0];
      return mod48FromTwoLongs( digit1, digit0,
                                        divisor );
      }

    if( where == 3 )
      {
      digit1 = scratch[3] << 24;
      digit1 |= scratch[2];
      digit0 = scratch[1] << 24;
      digit0 |= scratch[0];
      return mod48FromTwoLongs( digit1, digit0,
                                       divisor );
      }
    }
  else
    {
    // The index is bigger than 3.
    // This part would get called at least once.
    digit1 = scratch[where] << 24;
    digit1 |= scratch[where - 1];
    digit0 = scratch[where - 2] << 24;
    digit0 |= scratch[where - 3];
    remainder = mod48FromTwoLongs( digit1, digit0,
                                  divisor );
    scratch[where] = 0;
    scratch[where - 1] = 0;
    scratch[where - 2] = remainder >> 24;
    scratch[where - 3] = remainder & 0xFFFFFF;
    }

  where -= 2;
  }
}
