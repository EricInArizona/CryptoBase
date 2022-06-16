// Copyright Eric Chauvin 2022


// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html


#include "Sha256.h"


// RFC 6234



/*
Process one char array of 64 bytes.
That's 512 bits.

void Sha256::processMessageBlock( 
                  const CharBuf& charBuf,
                  const Int32 where )
{

//  int        t, t4;     // Loop counter
//  uint32_t   temp1, temp2;  // Temporary word value
Uint32 W[64];      // Word sequence

//  uint32_t   A, B, C, D, E, F, G, H;
                     // Word buffers

// Initialize the first 16 words in the array W.
=====
for( Int32 count = 0; count < 16; count++ )
  W[t] = charBuf.getUint32( where + count );



(Message_Block[t4]) << 24) |
           (Message_Block[t4 + 1]) << 16) |
           (Message_Block[t4 + 2]) &lt;&lt; 8) |
           (((uint32_t)context-&gt;Message_Block[t4 + 3]));




  for (t = 16; t < 64; t++)
    W[t] = SHA256_sigma1(W[t-2]) + W[t-7] +
        SHA256_sigma0(W[t-15]) + W[t-16];

  A = context-&gt;Intermediate_Hash[0];
  B = context-&gt;Intermediate_Hash[1];
  C = context-&gt;Intermediate_Hash[2];
  D = context-&gt;Intermediate_Hash[3];
  E = context-&gt;Intermediate_Hash[4];
  F = context-&gt;Intermediate_Hash[5];
  G = context-&gt;Intermediate_Hash[6];
  H = context-&gt;Intermediate_Hash[7];

  for (t = 0; t &lt; 64; t++) {
    temp1 = H + SHA256_SIGMA1(E) + SHA_Ch(E,F,G) + K[t] + W[t];
    temp2 = SHA256_SIGMA0(A) + SHA_Maj(A,B,C);
    H = G;
    G = F;
    F = E;
    E = D + temp1;
    D = C;
    C = B;
    B = A;
    A = temp1 + temp2;
  }

  context-&gt;Intermediate_Hash[0] += A;
  context-&gt;Intermediate_Hash[1] += B;
  context-&gt;Intermediate_Hash[2] += C;
  context-&gt;Intermediate_Hash[3] += D;
  context-&gt;Intermediate_Hash[4] += E;
  context-&gt;Intermediate_Hash[5] += F;
  context-&gt;Intermediate_Hash[6] += G;
  context-&gt;Intermediate_Hash[7] += H;

  context-&gt;Message_Block_Index = 0;
}
*/




// All SHA functions return one of these values.

enum {
    shaSuccess = 0,
    shaNull,  // Null pointer parameter
    shaInputTooLong,
    shaStateError,
    shaBadParam
};


enum {
    SHA1_Message_Block_Size = 64,
    SHA224_Message_Block_Size = 64,
    SHA256_Message_Block_Size = 64,
    SHA384_Message_Block_Size = 128,
    SHA512_Message_Block_Size = 128,
    USHA_Max_Message_Block_Size =
                       SHA512_Message_Block_Size,
    SHA1HashSize = 20,
    SHA224HashSize = 28,
    SHA256HashSize = 32,
    SHA384HashSize = 48,
    SHA512HashSize = 64,
    USHAMaxHashSize = SHA512HashSize,
    SHA1HashSizeBits = 160,
    SHA224HashSizeBits = 224,
    SHA256HashSizeBits = 256,
    SHA384HashSizeBits = 384,
    SHA512HashSizeBits = 512,
    USHAMaxHashSizeBits = SHA512HashSizeBits
};


typedef enum SHAversion {
    SHA1, SHA224, SHA256, SHA384, SHA512
} SHAversion;



typedef struct SHA256Context
  {
  Uint32 Intermediate_Hash[SHA256HashSize/4];
                       // Message Digest
  Uint32 Length_High;
              // Message length in bits
  Uint32 Length_Low;  // Message length in bits

  Uint32 Message_Block_Index;
               // Message_Block array index
               // 512-bit message blocks
  Uint8 Message_Block[SHA256_Message_Block_Size];

  Int32 Computed; // Is the hash computed?
  Int32 Corrupted; // Cumulative corruption code

} SHA256Context;



typedef struct SHA512Context
  {
  Uint64 Intermediate_Hash[SHA512HashSize/8];
                             // Message Digest
  Uint64 Length_High;
  Uint64 Length_Low; // Message length in bits

  Int32 Message_Block_Index;
                  // Message_Block array index

  // 1024-bit message blocks
  Uint8 Message_Block[SHA512_Message_Block_Size];

  Int32 Computed; // Is the hash computed?
  Int32 Corrupted; // Cumulative corruption code
} SHA512Context;



typedef struct USHAContext
  {
  Int32 whichSha;     //which SHA is being used
  union
    {
    SHA256Context sha256Context;
    SHA512Context sha512Context;
    } ctx;

} USHAContext;



/*
typedef struct HMACContext {
    int whichSha;    // which SHA is being used
    int hashSize;    //hash size of SHA being used
    int blockSize;   // block size of SHA being used

    USHAContext shaContext; // SHA context
    unsigned char k_opad[USHA_Max_Message_Block_Size];
          // outer padding - key XORd with opad
    int Computed;   // Is the MAC computed?
    int Corrupted;   // Cumulative corruption code

} HMACContext;
*/



/*
typedef struct HKDFContext {
    int whichSha;   // which SHA is being used
    HMACContext hmacContext;
    int hashSize;     // hash size of SHA being used

    unsigned char prk[USHAMaxHashSize];
      // pseudo-random key - output of hkdfInput
    int Computed; //  Is the key material computed?
    int Corrupted; // Cumulative corruption code
} HKDFContext;
*/



// SHA-256
extern Int32 SHA256Reset( SHA256Context * );

extern Int32 SHA256Input( SHA256Context *,
                       const Uint8 *bytes,
                       Uint32 bytecount );

// extern Int32 SHA256FinalBits( SHA256Context *,
//                   Uint8 bits,
//                   Uint32 bit_count );

extern Int32 SHA256Result( SHA256Context *,
            Uint8 Message_Digest[SHA256HashSize] );

/*
// SHA-512
extern Int32 SHA512Reset(SHA512Context *);
extern Int32 SHA512Input(SHA512Context *, const uint8_t *bytes,
                       unsigned int bytecount);
extern Int32 SHA512FinalBits(SHA512Context *, uint8_t bits,
                           unsigned int bit_count);
extern Int32 SHA512Result(SHA512Context *,
                        uint8_t Message_Digest[SHA512HashSize]);
*/




/*
// Unified SHA functions, chosen by whichSha
extern int USHAReset(USHAContext *context, SHAversion whichSha);
extern int USHAInput(USHAContext *context,
                     const uint8_t *bytes, unsigned int bytecount);
extern int USHAFinalBits(USHAContext *context,
                         uint8_t bits, unsigned int bit_count);
extern int USHAResult(USHAContext *context,
                      uint8_t Message_Digest[USHAMaxHashSize]);
extern int USHABlockSize(enum SHAversion whichSha);
extern int USHAHashSize(enum SHAversion whichSha);
extern int USHAHashSizeBits(enum SHAversion whichSha);
extern const char *USHAHashName(enum SHAversion whichSha);


 * HMAC Keyed-Hashing for Message Authentication, <a href="https://datatracker.ietf.org/doc/html/rfc2104">RFC 2104</a>,
 * for all SHAs.
 * This interface allows a fixed-length text input to be used.

extern int hmac(SHAversion whichSha,
// which SHA algorithm to use
    const unsigned char *text,
// pointer to data stream
    int text_len,      // length of data stream
    const unsigned char *key,
// pointer to authentication key
    int key_len,
// length of authentication key
    uint8_t digest[USHAMaxHashSize]);
// caller digest to fill in

*
 * HMAC Keyed-Hashing for Message Authentication, <a href="https://datatracker.ietf.org/doc/html/rfc2104">RFC 2104</a>,
 * for all SHAs.
 * This interface allows any length of text input to be used.

extern int hmacReset(HMACContext *context, enum SHAversion whichSha,
                     const unsigned char *key, int key_len);
extern int hmacInput(HMACContext *context, const unsigned char *text,
                     int text_len);
extern int hmacFinalBits(HMACContext *context, uint8_t bits,
                         unsigned int bit_count);
extern int hmacResult(HMACContext *context,
                      uint8_t digest[USHAMaxHashSize]);

*
 * HKDF HMAC-based Extract-and-Expand Key Derivation Function,
 * <a href="https://datatracker.ietf.org/doc/html/rfc5869">RFC 5869</a>, for all SHAs.

extern int hkdf(SHAversion whichSha, const unsigned char *salt,
                int salt_len, const unsigned char *ikm, int ikm_len,
                const unsigned char *info, int info_len,
                uint8_t okm[ ], int okm_len);
extern int hkdfExtract(SHAversion whichSha, const unsigned char *salt,
                       int salt_len, const unsigned char *ikm,
                       int ikm_len, uint8_t prk[USHAMaxHashSize]);
extern int hkdfExpand(SHAversion whichSha, const uint8_t prk[ ],
                      int prk_len, const unsigned char *info,
                      int info_len, uint8_t okm[ ], int okm_len);

*
 * HKDF HMAC-based Extract-and-Expand Key Derivation Function,
 * <a href="https://datatracker.ietf.org/doc/html/rfc5869">RFC 5869</a>, for all SHAs.
 * This interface allows any length of text input to be used.
 *
extern int hkdfReset(HKDFContext *context, enum SHAversion whichSha,
                     const unsigned char *salt, int salt_len);



extern int hkdfInput(HKDFContext *context, const unsigned char *ikm,
                     int ikm_len);
extern int hkdfFinalBits(HKDFContext *context, uint8_t ikm_bits,
                         unsigned int ikm_bit_count);
extern int hkdfResult(HKDFContext *context,
                      uint8_t prk[USHAMaxHashSize],
                      const unsigned char *info, int info_len,
                      uint8_t okm[USHAMaxHashSize], int okm_len);

*/
////////////////////////////////////












/////////////////////////
// ******* sha224-256.c **

// Don't need SHA224/256FinalBits() because I
// only use bytes.  Not some odd number of bits.



// static uint32_t addTemp;
// #define SHA224_256AddLength(context, length)               \
//  (addTemp = (context)->Length_Low,
//             (context)->Corrupted =
//             (((context)->Length_Low +=
//             (length)) < addTemp) &&
//             (++(context)->Length_High == 0)
//             ? shaInputTooLong :
//             (context)->Corrupted )


/*
static Int32 SHA224_256Reset(
                       SHA256Context *context,
                       Uint32 *H0 );
*/

// static void SHA224_256ProcessMessageBlock(
  //                 SHA256Context *context );


/*
static void SHA224_256Finalize(
                SHA256Context *context,
                Uint8 Pad_Byte );

static void SHA224_256PadMessage(
                         SHA256Context *context,
                         Uint8 Pad_Byte );

static Int32 SHA224_256ResultN(
                          SHA256Context *context,
                          Uint8 Message_Digest[ ],
                          Int32 HashSize );

*/

// Initial Hash Values: FIPS 180-3 <a

/*
static Uint32 SHA224_H0[SHA256HashSize/4] = {
    0xC1059ED8, 0x367CD507, 0x3070DD17, 0xF70E5939,
    0xFFC00B31, 0x68581511, 0x64F98FA7, 0xBEFA4FA4
};


// Initial Hash Values: FIPS 180-3

static Uint32 SHA256_H0[SHA256HashSize/4] = {
  0x6A09E667, 0xBB67AE85, 0x3C6EF372, 0xA54FF53A,
  0x510E527F, 0x9B05688C, 0x1F83D9AB, 0x5BE0CD19
};

*/


/*
Int32 SHA256Input( SHA256Context *context,
                   const Uint8 *message_array,
                   Uint32 length )
{
if( !context )
  return shaNull;

if( !length )
  return shaSuccess;

if( !message_array )
  return shaNull;

if( context->Computed )
  return context->Corrupted = shaStateError;

if( context->Corrupted )
  return context->Corrupted;

while( length-- )
  {
  context->Message_Block[
              context->Message_Block_Index++] =
              *message_array;

  if( (SHA224_256AddLength(context, 8) ==
                    shaSuccess ) &&
      (context->Message_Block_Index ==
                         SHA256_Message_Block_Size))
    {
    SHA224_256ProcessMessageBlock( context );
    }

  message_array++;
  }

return context->Corrupted;
}



*
 * SHA256FinalBits
 *
 * Description:
 *   This function will add in any final bits of the message.
 *
 * Parameters:
 *   context: [in/out]
 *     The SHA context to update.
 *   message_bits: [in]
 *     The final bits of the message, in the upper portion of the
 *     byte.  (Use 0b###00000 instead of 0b00000### to input the
 *     three bits ###.)
 *   length: [in]
 *     The number of bits in message_bits, between 1 and 7.
 *
 * Returns:
 *   sha Error Code.
 *

int SHA256FinalBits(SHA256Context *context,
                    uint8_t message_bits, unsigned int length)
{
  static uint8_t masks[8] = {
       0x00,
            0x80,
       0xC0,  0xE0,
       0xF0,  0xF8,
       0xFC,  0xFE
  };
  static uint8_t markbit[8] = {
       0x80,  0x40,
       0x20, 0x10,
       0x08,  0x04,
       0x02,  0x01
  };

  if (!context) return shaNull;
  if (!length) return shaSuccess;
  if (context-&gt;Corrupted) return context-&gt;Corrupted;
  if (context-&gt;Computed) return context-&gt;Corrupted = shaStateError;
  if (length &gt;= 8) return context-&gt;Corrupted = shaBadParam;

  SHA224_256AddLength(context, length);
  SHA224_256Finalize(context, (uint8_t)
    ((message_bits &amp; masks[length]) | markbit[length]));

  return context-&gt;Corrupted;
}

*
 * SHA256Result
 *
 * Description:
 *   This function will return the 256-bit message digest
 *   into the Message_Digest array provided by the caller.
 *   NOTE:
 *    The first octet of hash is stored in the element with index 0,
 *    the last octet of hash in the element with index 31.
 *
 * Parameters:
 *   context: [in/out]
 *     The context to use to calculate the SHA hash.
 *   Message_Digest[ ]: [out]
 *     Where the digest is returned.
 *
 * Returns:
 *   sha Error Code.
 *

int SHA256Result(SHA256Context *context,
                 uint8_t Message_Digest[SHA256HashSize])
{
  return SHA224_256ResultN(context, Message_Digest, SHA256HashSize);
}

*
 * SHA224_256Reset
 *
 * Description:
 *   This helper function will initialize the SHA256Context in
 *   preparation for computing a new SHA-224 or SHA-256 message digest.
 *
 * Parameters:
 *   context: [in/out]
 *     The context to reset.
 *   H0[ ]: [in]
 *     The initial hash value array to use.
 *
 * Returns:
 *   sha Error Code.
 *

static int SHA224_256Reset(SHA256Context *context, uint32_t *H0)
{
  if (!context) return shaNull;

  context-&gt;Length_High = context-&gt;Length_Low = 0;
  context-&gt;Message_Block_Index  = 0;

  context-&gt;Intermediate_Hash[0] = H0[0];
  context-&gt;Intermediate_Hash[1] = H0[1];
  context-&gt;Intermediate_Hash[2] = H0[2];
  context-&gt;Intermediate_Hash[3] = H0[3];
  context-&gt;Intermediate_Hash[4] = H0[4];
  context-&gt;Intermediate_Hash[5] = H0[5];
  context-&gt;Intermediate_Hash[6] = H0[6];
  context-&gt;Intermediate_Hash[7] = H0[7];

  context-&gt;Computed  = 0;
  context-&gt;Corrupted = shaSuccess;

  return shaSuccess;
}
*/


/*
static void SHA224_256ProcessMessageBlock(
                       SHA256Context *context)
{
// Constants defined in FIPS 180-3


  int        t, t4;     // Loop counter
  uint32_t   temp1, temp2;  // Temporary word value
  uint32_t   W[64];      // Word sequence
  uint32_t   A, B, C, D, E, F, G, H;
                     // Word buffers

  *
   * Initialize the first 16 words in the array W
   *


  for (t = t4 = 0; t < 16; t++, t4 += 4)
    W[t] = (Message_Block[t4]) << 24) |
           (((uint32_t)context-&gt;Message_Block[t4 + 1]) &lt;&lt; 16) |
           (((uint32_t)context-&gt;Message_Block[t4 + 2]) &lt;&lt; 8) |
           (((uint32_t)context-&gt;Message_Block[t4 + 3]));




  for (t = 16; t &lt; 64; t++)
    W[t] = SHA256_sigma1(W[t-2]) + W[t-7] +
        SHA256_sigma0(W[t-15]) + W[t-16];

  A = context-&gt;Intermediate_Hash[0];
  B = context-&gt;Intermediate_Hash[1];
  C = context-&gt;Intermediate_Hash[2];
  D = context-&gt;Intermediate_Hash[3];
  E = context-&gt;Intermediate_Hash[4];
  F = context-&gt;Intermediate_Hash[5];
  G = context-&gt;Intermediate_Hash[6];
  H = context-&gt;Intermediate_Hash[7];

  for (t = 0; t &lt; 64; t++) {
    temp1 = H + SHA256_SIGMA1(E) + SHA_Ch(E,F,G) + K[t] + W[t];
    temp2 = SHA256_SIGMA0(A) + SHA_Maj(A,B,C);
    H = G;
    G = F;
    F = E;
    E = D + temp1;
    D = C;
    C = B;
    B = A;
    A = temp1 + temp2;
  }

  context-&gt;Intermediate_Hash[0] += A;
  context-&gt;Intermediate_Hash[1] += B;
  context-&gt;Intermediate_Hash[2] += C;
  context-&gt;Intermediate_Hash[3] += D;
  context-&gt;Intermediate_Hash[4] += E;
  context-&gt;Intermediate_Hash[5] += F;
  context-&gt;Intermediate_Hash[6] += G;
  context-&gt;Intermediate_Hash[7] += H;

  context-&gt;Message_Block_Index = 0;
}
*/




/*
*
 * SHA224_256Finalize
 *
 * Description:
 *   This helper function finishes off the digest calculations.
 *
 * Parameters:
 *   context: [in/out]
 *     The SHA context to update.
 *   Pad_Byte: [in]



 *     The last byte to add to the message block before the 0-padding
 *     and length.  This will contain the last bits of the message
 *     followed by another single bit.  If the message was an
 *     exact multiple of 8-bits long, Pad_Byte will be 0x80.
 *
 * Returns:
 *   sha Error Code.
 *
static void SHA224_256Finalize(SHA256Context *context,
    uint8_t Pad_Byte)
{
  int i;
  SHA224_256PadMessage(context, Pad_Byte);
  // message may be sensitive, so clear it out

  for (i = 0; i &lt; SHA256_Message_Block_Size; ++i)
    context-&gt;Message_Block[i] = 0;
  context-&gt;Length_High = 0;   // and clear length
  context-&gt;Length_Low = 0;
  context-&gt;Computed = 1;
}

*
 * SHA224_256PadMessage
 *
 * Description:
 *   According to the standard, the message must be padded to the next
 *   even multiple of 512 bits.  The first padding bit must be a '1'.
 *   The last 64 bits represent the length of the original message.
 *   All bits in between should be 0.  This helper function will pad
 *   the message according to those rules by filling the
 *   Message_Block array accordingly.  When it returns, it can be
 *   assumed that the message digest has been computed.
 *
 * Parameters:
 *   context: [in/out]
 *     The context to pad.
 *   Pad_Byte: [in]
 *     The last byte to add to the message block before the 0-padding
 *     and length.  This will contain the last bits of the message
 *     followed by another single bit.  If the message was an
 *     exact multiple of 8-bits long, Pad_Byte will be 0x80.
 *
 * Returns:
 *   Nothing.
 *
static void SHA224_256PadMessage(SHA256Context *context,
    uint8_t Pad_Byte)
{


*
   * Check to see if the current message block is too small to hold
   * the initial padding bits and length.  If so, we will pad the
   * block, process it, and then continue padding into a second
   * block.
   *
  if (context-&gt;Message_Block_Index &gt;= (SHA256_Message_Block_Size-8)) {
    context-&gt;Message_Block[context-&gt;Message_Block_Index++] = Pad_Byte;
    while (context-&gt;Message_Block_Index &lt; SHA256_Message_Block_Size)
      context-&gt;Message_Block[context-&gt;Message_Block_Index++] = 0;
    SHA224_256ProcessMessageBlock(context);
  } else
    context-&gt;Message_Block[context-&gt;Message_Block_Index++] = Pad_Byte;

  while (context-&gt;Message_Block_Index &lt; (SHA256_Message_Block_Size-8))
    context-&gt;Message_Block[context-&gt;Message_Block_Index++] = 0;

  *
   * Store the message length as the last 8 octets
   *
  context-&gt;Message_Block[56] = (uint8_t)(context-&gt;Length_High &gt;&gt; 24);
  context-&gt;Message_Block[57] = (uint8_t)(context-&gt;Length_High &gt;&gt; 16);
  context-&gt;Message_Block[58] = (uint8_t)(context-&gt;Length_High &gt;&gt; 8);
  context-&gt;Message_Block[59] = (uint8_t)(context-&gt;Length_High);
  context-&gt;Message_Block[60] = (uint8_t)(context-&gt;Length_Low &gt;&gt; 24);
  context-&gt;Message_Block[61] = (uint8_t)(context-&gt;Length_Low &gt;&gt; 16);
  context-&gt;Message_Block[62] = (uint8_t)(context-&gt;Length_Low &gt;&gt; 8);
  context-&gt;Message_Block[63] = (uint8_t)(context-&gt;Length_Low);

  SHA224_256ProcessMessageBlock(context);
}

*
 * SHA224_256ResultN
 *
 * Description:
 *   This helper function will return the 224-bit or 256-bit message
 *   digest into the Message_Digest array provided by the caller.
 *   NOTE:
 *    The first octet of hash is stored in the element with index 0,
 *    the last octet of hash in the element with index 27/31.
 *
 * Parameters:
 *   context: [in/out]
 *     The context to use to calculate the SHA hash.
 *   Message_Digest[ ]: [out]
 *     Where the digest is returned.
 *   HashSize: [in]


 *     The size of the hash, either 28 or 32.
 *
 * Returns:
 *   sha Error Code.
 *

static int SHA224_256ResultN(SHA256Context *context,
    uint8_t Message_Digest[ ], int HashSize)
{
  int i;

  if (!context) return shaNull;
  if (!Message_Digest) return shaNull;
  if (context-&gt;Corrupted) return context-&gt;Corrupted;

  if (!context-&gt;Computed)
    SHA224_256Finalize(context, 0x80);

  for (i = 0; i &lt; HashSize; ++i)
    Message_Digest[i] = (uint8_t)
      (context-&gt;Intermediate_Hash[i&gt;&gt;2] &gt;&gt; 8 * ( 3 - ( i &amp; 0x03 ) ));

  return shaSuccess;
}
*/
////////////////////////////////////

















/*
************************* sha384-512.c ***
 * Description:
 *   This file implements the Secure Hash Algorithms SHA-384 and
 *   SHA-512 as defined in the U.S. National Institute of Standards
 *   and Technology Federal Information Processing Standards
 *   Publication (FIPS PUB) 180-3 published in October 2008
 *   and formerly defined in its predecessors, FIPS PUB 180-1
 *   and FIP PUB 180-2.
 *
 *   A combined document showing all algorithms is available at
 *       <a href="http://csrc.nist.gov/publications/fips/">http://csrc.nist.gov/publications/fips/</a>
 *              fips180-3/fips180-3_final.pdf
 *
 *   The SHA-384 and SHA-512 algorithms produce 384-bit and 512-bit
 *   message digests for a given data stream.  It should take about
 *   2**n steps to find a message with the same digest as a given



 *   message and 2**(n/2) to find any two messages with the same
 *   digest, when n is the digest size in bits.  Therefore, this
 *   algorithm can serve as a means of providing a
 *   "fingerprint" for a message.
 *
 * Portability Issues:
 *   SHA-384 and SHA-512 are defined in terms of 64-bit "words",
 *   but if USE_32BIT_ONLY is #defined, this code is implemented in
 *   terms of 32-bit "words".  This code uses &lt;stdint.h&gt; (included
 *   via "sha.h") to define the 64-, 32- and 8-bit unsigned integer
 *   types.  If your C compiler does not support 64-bit unsigned
 *   integers and you do not #define USE_32BIT_ONLY, this code is
 *   not appropriate.
 *
 * Caveats:
 *   SHA-384 and SHA-512 are designed to work with messages less
 *   than 2^128 bits long.  This implementation uses SHA384/512Input()
 *   to hash the bits that are a multiple of the size of an 8-bit
 *   octet, and then optionally uses SHA384/256FinalBits()
 *   to hash the final few bits of the input.
 *
 *

#include "sha.h"

#ifdef USE_32BIT_ONLY
*
 * Define 64-bit arithmetic in terms of 32-bit arithmetic.
 * Each 64-bit number is represented in a 2-word array.
 * All macros are defined such that the result is the last parameter.
 *

*
 * Define shift, rotate left, and rotate right functions
 *

#define SHA512_SHR(bits, word, ret) (                          \
    // (((uint64_t)((word))) &gt;&gt; (bits))
                     \
    (ret)[0] = (((bits) &lt; 32) &amp;&amp; ((bits) &gt;= 0)) ?              \
      ((word)[0] &gt;&gt; (bits)) : 0,                               \
    (ret)[1] = ((bits) &gt; 32) ? ((word)[0] &gt;&gt; ((bits) - 32)) :  \
      ((bits) == 32) ? (word)[0] :                             \
      ((bits) &gt;= 0) ?                                          \
        (((word)[0] &lt;&lt; (32 - (bits))) |                        \
        ((word)[1] &gt;&gt; (bits))) : 0 )

#define SHA512_SHL(bits, word, ret) (                          \
    // (((uint64_t)(word)) &lt;&lt; (bits))                        \
    (ret)[0] = ((bits) &gt; 32) ? ((word)[1] &lt;&lt; ((bits) - 32)) :  \



         ((bits) == 32) ? (word)[1] :                          \
         ((bits) &gt;= 0) ?                                       \
           (((word)[0] &lt;&lt; (bits)) |                            \
           ((word)[1] &gt;&gt; (32 - (bits)))) :                     \
         0,                                                    \
    (ret)[1] = (((bits) &lt; 32) &amp;&amp; ((bits) &gt;= 0)) ?              \
        ((word)[1] &lt;&lt; (bits)) : 0 )

*
 * Define 64-bit OR
 *
#define SHA512_OR(word1, word2, ret) (                         \
    (ret)[0] = (word1)[0] | (word2)[0],                        \
    (ret)[1] = (word1)[1] | (word2)[1] )

*
 * Define 64-bit XOR
 *
#define SHA512_XOR(word1, word2, ret) (                        \
    (ret)[0] = (word1)[0] ^ (word2)[0],                        \
    (ret)[1] = (word1)[1] ^ (word2)[1] )

*
 * Define 64-bit AND
 *
#define SHA512_AND(word1, word2, ret) (                        \
    (ret)[0] = (word1)[0] &amp; (word2)[0],                        \
    (ret)[1] = (word1)[1] &amp; (word2)[1] )

*
 * Define 64-bit TILDA
 *
#define SHA512_TILDA(word, ret)                                \
  ( (ret)[0] = ~(word)[0], (ret)[1] = ~(word)[1] )

*
 * Define 64-bit ADD
 *
#define SHA512_ADD(word1, word2, ret) (                        \
    (ret)[1] = (word1)[1], (ret)[1] += (word2)[1],             \
    (ret)[0] = (word1)[0] + (word2)[0] + ((ret)[1] &lt; (word1)[1]) )

*
 * Add the 4word value in word2 to word1.
 *
static uint32_t ADDTO4_temp, ADDTO4_temp2;
#define SHA512_ADDTO4(word1, word2) (                          \
    ADDTO4_temp = (word1)[3],                                  \


    (word1)[3] += (word2)[3],                                  \
    ADDTO4_temp2 = (word1)[2],                                 \
    (word1)[2] += (word2)[2] + ((word1)[3] &lt; ADDTO4_temp),     \
    ADDTO4_temp = (word1)[1],                                  \
    (word1)[1] += (word2)[1] + ((word1)[2] &lt; ADDTO4_temp2),    \
    (word1)[0] += (word2)[0] + ((word1)[1] &lt; ADDTO4_temp) )

*
 * Add the 2word value in word2 to word1.
 *
static uint32_t ADDTO2_temp;
#define SHA512_ADDTO2(word1, word2) (                          \
    ADDTO2_temp = (word1)[1],                                  \
    (word1)[1] += (word2)[1],                                  \
    (word1)[0] += (word2)[0] + ((word1)[1] &lt; ADDTO2_temp) )

*
 * SHA rotate   ((word &gt;&gt; bits) | (word &lt;&lt; (64-bits)))
 *
static uint32_t ROTR_temp1[2], ROTR_temp2[2];
#define SHA512_ROTR(bits, word, ret) (                         \
    SHA512_SHR((bits), (word), ROTR_temp1),                    \
    SHA512_SHL(64-(bits), (word), ROTR_temp2),                 \
    SHA512_OR(ROTR_temp1, ROTR_temp2, (ret)) )

*
 * Define the SHA SIGMA and sigma macros
 *
 *  SHA512_ROTR(28,word) ^ SHA512_ROTR(34,word) ^ SHA512_ROTR(39,word)
 *

static uint32_t SIGMA0_temp1[2], SIGMA0_temp2[2],
  SIGMA0_temp3[2], SIGMA0_temp4[2];
#define SHA512_SIGMA0(word, ret) (                             \
    SHA512_ROTR(28, (word), SIGMA0_temp1),                     \
    SHA512_ROTR(34, (word), SIGMA0_temp2),                     \
    SHA512_ROTR(39, (word), SIGMA0_temp3),                     \
    SHA512_XOR(SIGMA0_temp2, SIGMA0_temp3, SIGMA0_temp4),      \
    SHA512_XOR(SIGMA0_temp1, SIGMA0_temp4, (ret)) )

*
 * SHA512_ROTR(14,word) ^ SHA512_ROTR(18,word) ^ SHA512_ROTR(41,word)
 *
static uint32_t SIGMA1_temp1[2], SIGMA1_temp2[2],
  SIGMA1_temp3[2], SIGMA1_temp4[2];
#define SHA512_SIGMA1(word, ret) (                             \
    SHA512_ROTR(14, (word), SIGMA1_temp1),                     \
    SHA512_ROTR(18, (word), SIGMA1_temp2),                     \
    SHA512_ROTR(41, (word), SIGMA1_temp3),                     \



    SHA512_XOR(SIGMA1_temp2, SIGMA1_temp3, SIGMA1_temp4),      \
    SHA512_XOR(SIGMA1_temp1, SIGMA1_temp4, (ret)) )


 * (SHA512_ROTR( 1,word) ^ SHA512_ROTR( 8,word) ^ SHA512_SHR( 7,word))
 *

static uint32_t sigma0_temp1[2], sigma0_temp2[2],
  sigma0_temp3[2], sigma0_temp4[2];
#define SHA512_sigma0(word, ret) (                             \
    SHA512_ROTR( 1, (word), sigma0_temp1),                     \
    SHA512_ROTR( 8, (word), sigma0_temp2),                     \
    SHA512_SHR( 7, (word), sigma0_temp3),                      \
    SHA512_XOR(sigma0_temp2, sigma0_temp3, sigma0_temp4),      \
    SHA512_XOR(sigma0_temp1, sigma0_temp4, (ret)) )

*
 * (SHA512_ROTR(19,word) ^ SHA512_ROTR(61,word) ^ SHA512_SHR( 6,word))
 *

static uint32_t sigma1_temp1[2], sigma1_temp2[2],
  sigma1_temp3[2], sigma1_temp4[2];
#define SHA512_sigma1(word, ret) (                             \
    SHA512_ROTR(19, (word), sigma1_temp1),                     \
    SHA512_ROTR(61, (word), sigma1_temp2),                     \
    SHA512_SHR( 6, (word), sigma1_temp3),                      \
    SHA512_XOR(sigma1_temp2, sigma1_temp3, sigma1_temp4),      \
    SHA512_XOR(sigma1_temp1, sigma1_temp4, (ret)) )

#ifndef USE_MODIFIED_MACROS
*
 * These definitions are the ones used in FIPS 180-3, <a href="https://datatracker.ietf.org/doc/html/rfc6234#section-4.1.3">section 4.1.3</a>
 *  Ch(x,y,z)   ((x &amp; y) ^ (~x &amp; z))
 *

static uint32_t Ch_temp1[2], Ch_temp2[2], Ch_temp3[2];
#define SHA_Ch(x, y, z, ret) (                                 \
    SHA512_AND(x, y, Ch_temp1),                                \
    SHA512_TILDA(x, Ch_temp2),                                 \
    SHA512_AND(Ch_temp2, z, Ch_temp3),                         \
    SHA512_XOR(Ch_temp1, Ch_temp3, (ret)) )

*
 *  Maj(x,y,z)  (((x)&amp;(y)) ^ ((x)&amp;(z)) ^ ((y)&amp;(z)))
 *

static uint32_t Maj_temp1[2], Maj_temp2[2],
  Maj_temp3[2], Maj_temp4[2];
#define SHA_Maj(x, y, z, ret) (                                \
    SHA512_AND(x, y, Maj_temp1),                               \
    SHA512_AND(x, z, Maj_temp2),                               \
    SHA512_AND(y, z, Maj_temp3),                               \




    SHA512_XOR(Maj_temp2, Maj_temp3, Maj_temp4),               \
    SHA512_XOR(Maj_temp1, Maj_temp4, (ret)) )
#else // !USE_MODIFIED_MACROS
*
 * These definitions are potentially faster equivalents for the ones
 * used in FIPS 180-3, <a href="https://datatracker.ietf.org/doc/html/rfc6234#section-4.1.3">section 4.1.3</a>.
 *   ((x &amp; y) ^ (~x &amp; z)) becomes
 *   ((x &amp; (y ^ z)) ^ z)
 *

#define SHA_Ch(x, y, z, ret) (                                 \
   (ret)[0] = (((x)[0] &amp; ((y)[0] ^ (z)[0])) ^ (z)[0]),         \
   (ret)[1] = (((x)[1] &amp; ((y)[1] ^ (z)[1])) ^ (z)[1]) )

*
 *   ((x &amp; y) ^ (x &amp; z) ^ (y &amp; z)) becomes
 *   ((x &amp; (y | z)) | (y &amp; z))
 *

#define SHA_Maj(x, y, z, ret) (                                 \
   ret[0] = (((x)[0] &amp; ((y)[0] | (z)[0])) | ((y)[0] &amp; (z)[0])), \
   ret[1] = (((x)[1] &amp; ((y)[1] | (z)[1])) | ((y)[1] &amp; (z)[1])) )
#endif // USE_MODIFIED_MACROS


*
 * Add "length" to the length.
 * Set Corrupted when overflow has occurred.
 *

static uint32_t addTemp[4] = { 0, 0, 0, 0 };
#define SHA384_512AddLength(context, length) (                        \
    addTemp[3] = (length), SHA512_ADDTO4((context)-&gt;Length, addTemp), \
    (context)-&gt;Corrupted = (((context)-&gt;Length[3] &lt; (length)) &amp;&amp;      \
       ((context)-&gt;Length[2] == 0) &amp;&amp; ((context)-&gt;Length[1] == 0) &amp;&amp;  \
       ((context)-&gt;Length[0] == 0)) ? shaInputTooLong :               \
                                      (context)-&gt;Corrupted )



static int SHA384_512Reset(SHA512Context *context,
                           uint32_t H0[SHA512HashSize/4]);
static void SHA384_512ProcessMessageBlock(SHA512Context *context);
static void SHA384_512Finalize(SHA512Context *context,
  uint8_t Pad_Byte);
static void SHA384_512PadMessage(SHA512Context *context,
  uint8_t Pad_Byte);
static int SHA384_512ResultN( SHA512Context *context,
  uint8_t Message_Digest[ ], int HashSize);

* Initial Hash Values: FIPS 180-3

static uint32_t SHA384_H0[SHA512HashSize/4] = {
    0xCBBB9D5D, 0xC1059ED8, 0x629A292A, 0x367CD507, 0x9159015A,



    0x3070DD17, 0x152FECD8, 0xF70E5939, 0x67332667, 0xFFC00B31,
    0x8EB44A87, 0x68581511, 0xDB0C2E0D, 0x64F98FA7, 0x47B5481D,
    0xBEFA4FA4
};
static uint32_t SHA512_H0[SHA512HashSize/4] = {
    0x6A09E667, 0xF3BCC908, 0xBB67AE85, 0x84CAA73B, 0x3C6EF372,
    0xFE94F82B, 0xA54FF53A, 0x5F1D36F1, 0x510E527F, 0xADE682D1,
    0x9B05688C, 0x2B3E6C1F, 0x1F83D9AB, 0xFB41BD6B, 0x5BE0CD19,
    0x137E2179
};

#else // !USE_32BIT_ONLY

#include "sha-private.h"

* Define the SHA shift, rotate left and
// rotate right macros

#define SHA512_SHR(bits,word)  (((uint64_t)(word)) &gt;&gt; (bits))
#define SHA512_ROTR(bits,word) ((((uint64_t)(word)) &gt;&gt; (bits)) | \
                                (((uint64_t)(word)) &lt;&lt; (64-(bits))))

*
 * Define the SHA SIGMA and sigma macros
 *
 *  SHA512_ROTR(28,word) ^ SHA512_ROTR(34,word) ^ SHA512_ROTR(39,word)
 *
#define SHA512_SIGMA0(word)   \
 (SHA512_ROTR(28,word) ^ SHA512_ROTR(34,word) ^ SHA512_ROTR(39,word))
#define SHA512_SIGMA1(word)   \
 (SHA512_ROTR(14,word) ^ SHA512_ROTR(18,word) ^ SHA512_ROTR(41,word))
#define SHA512_sigma0(word)   \
 (SHA512_ROTR( 1,word) ^ SHA512_ROTR( 8,word) ^ SHA512_SHR( 7,word))
#define SHA512_sigma1(word)   \
 (SHA512_ROTR(19,word) ^ SHA512_ROTR(61,word) ^ SHA512_SHR( 6,word))

*
 * Add "length" to the length.
 * Set Corrupted when overflow has occurred.
 *
static uint64_t addTemp;
#define SHA384_512AddLength(context, length)                   \
   (addTemp = context-&gt;Length_Low, context-&gt;Corrupted =        \
    ((context-&gt;Length_Low += length) &lt; addTemp) &amp;&amp;             \
    (++context-&gt;Length_High == 0) ? shaInputTooLong :          \
                                    (context)-&gt;Corrupted)



static int SHA384_512Reset(SHA512Context *context,
                           uint64_t H0[SHA512HashSize/8]);



static void SHA384_512ProcessMessageBlock(SHA512Context *context);
static void SHA384_512Finalize(SHA512Context *context,
  uint8_t Pad_Byte);
static void SHA384_512PadMessage(SHA512Context *context,
  uint8_t Pad_Byte);
static int SHA384_512ResultN(SHA512Context *context,
  uint8_t Message_Digest[ ], int HashSize);

// Initial Hash Values: FIPS 180-3

static uint64_t SHA384_H0[ ] = {
    0xCBBB9D5DC1059ED8ll, 0x629A292A367CD507ll, 0x9159015A3070DD17ll,
    0x152FECD8F70E5939ll, 0x67332667FFC00B31ll, 0x8EB44A8768581511ll,
    0xDB0C2E0D64F98FA7ll, 0x47B5481DBEFA4FA4ll
};
static uint64_t SHA512_H0[ ] = {
    0x6A09E667F3BCC908ll, 0xBB67AE8584CAA73Bll, 0x3C6EF372FE94F82Bll,
    0xA54FF53A5F1D36F1ll, 0x510E527FADE682D1ll, 0x9B05688C2B3E6C1Fll,
    0x1F83D9ABFB41BD6Bll, 0x5BE0CD19137E2179ll
};

#endif // USE_32BIT_ONLY



*
 * SHA384Reset
 *
 * Description:
 *   This function will initialize the SHA384Context in preparation
 *   for computing a new SHA384 message digest.
 *
 * Parameters:
 *   context: [in/out]
 *     The context to reset.
 *
 * Returns:
 *   sha Error Code.
 *
 *
int SHA384Reset(SHA384Context *context)
{
  return SHA384_512Reset(context, SHA384_H0);
}

*
 * SHA384Input
 *
 * Description:
 *   This function accepts an array of octets as the next portion
 *   of the message.



 *
 * Parameters:
 *   context: [in/out]
 *     The SHA context to update.
 *   message_array[ ]: [in]
 *     An array of octets representing the next portion of
 *     the message.
 *   length: [in]
 *     The length of the message in message_array.
 *
 * Returns:
 *   sha Error Code.
 *
 *

int SHA384Input(SHA384Context *context,
    const uint8_t *message_array, unsigned int length)
{
  return SHA512Input(context, message_array, length);
}

*
 * SHA384FinalBits
 *
 * Description:
 *   This function will add in any final bits of the message.
 *
 * Parameters:
 *   context: [in/out]
 *     The SHA context to update.
 *   message_bits: [in]
 *     The final bits of the message, in the upper portion of the
 *     byte.  (Use 0b###00000 instead of 0b00000### to input the
 *     three bits ###.)
 *   length: [in]
 *     The number of bits in message_bits, between 1 and 7.
 *
 * Returns:
 *   sha Error Code.
 *
 *
int SHA384FinalBits(SHA384Context *context,
                    uint8_t message_bits, unsigned int length)
{
  return SHA512FinalBits(context, message_bits, length);
}

*
 * SHA384Result



 *
 * Description:
 *   This function will return the 384-bit message digest
 *   into the Message_Digest array provided by the caller.
 *   NOTE:
 *    The first octet of hash is stored in the element with index 0,
 *    the last octet of hash in the element with index 47.
 *
 * Parameters:
 *   context: [in/out]
 *     The context to use to calculate the SHA hash.
 *   Message_Digest[ ]: [out]
 *     Where the digest is returned.
 *
 * Returns:
 *   sha Error Code.
 *
 *

int SHA384Result(SHA384Context *context,
    uint8_t Message_Digest[SHA384HashSize])
{
  return SHA384_512ResultN(context, Message_Digest, SHA384HashSize);
}


*
 * SHA512Reset
 *
 * Description:
 *   This function will initialize the SHA512Context in preparation
 *   for computing a new SHA512 message digest.
 *
 * Parameters:
 *   context: [in/out]
 *     The context to reset.
 *
 * Returns:
 *   sha Error Code.
 *
 *
int SHA512Reset(SHA512Context *context)
{
  return SHA384_512Reset(context, SHA512_H0);
}

*
 * SHA512Input
 *
 * Description:




 *   This function accepts an array of octets as the next portion
 *   of the message.
 *
 * Parameters:
 *   context: [in/out]
 *     The SHA context to update.
 *   message_array[ ]: [in]
 *     An array of octets representing the next portion of
 *     the message.
 *   length: [in]
 *     The length of the message in message_array.
 *
 * Returns:
 *   sha Error Code.
 *
 *

int SHA512Input(SHA512Context *context,
        const uint8_t *message_array,
        unsigned int length)
{
  if (!context) return shaNull;
  if (!length) return shaSuccess;
  if (!message_array) return shaNull;
  if (context-&gt;Computed) return context-&gt;Corrupted = shaStateError;
  if (context-&gt;Corrupted) return context-&gt;Corrupted;

  while (length--) {
    context-&gt;Message_Block[context-&gt;Message_Block_Index++] =
            *message_array;

    if ((SHA384_512AddLength(context, 8) == shaSuccess) &amp;&amp;
      (context-&gt;Message_Block_Index == SHA512_Message_Block_Size))
      SHA384_512ProcessMessageBlock(context);

    message_array++;
  }

  return context-&gt;Corrupted;
}

*
 * SHA512FinalBits
 *
 * Description:
 *   This function will add in any final bits of the message.
 *
 * Parameters:
 *   context: [in/out]



 *     The SHA context to update.
 *   message_bits: [in]
 *     The final bits of the message, in the upper portion of the
 *     byte.  (Use 0b###00000 instead of 0b00000### to input the
 *     three bits ###.)
 *   length: [in]
 *     The number of bits in message_bits, between 1 and 7.
 *
 * Returns:
 *   sha Error Code.
 *
 *

int SHA512FinalBits(SHA512Context *context,
                    uint8_t message_bits, unsigned int length)
{
  static uint8_t masks[8] = {
      0x00,  0x80,
       0xC0, 0xE0,
       0xF0, 0xF8,
       0xFC,  0xFE
  };
  static uint8_t markbit[8] = {
      0x80, 0x40,
      0x20, 0x10,
       0x08,  0x04,
       0x02,  0x01
  };

  if (!context) return shaNull;
  if (!length) return shaSuccess;
  if (context-&gt;Corrupted) return context-&gt;Corrupted;
  if (context-&gt;Computed) return context-&gt;Corrupted = shaStateError;
  if (length &gt;= 8) return context-&gt;Corrupted = shaBadParam;

  SHA384_512AddLength(context, length);
  SHA384_512Finalize(context, (uint8_t)
    ((message_bits &amp; masks[length]) | markbit[length]));

  return context-&gt;Corrupted;
}


*
 * SHA512Result
 *
 * Description:
 *   This function will return the 512-bit message digest
 *   into the Message_Digest array provided by the caller.
 *   NOTE:




 *    The first octet of hash is stored in the element with index 0,
 *    the last octet of hash in the element with index 63.
 *
 * Parameters:
 *   context: [in/out]
 *     The context to use to calculate the SHA hash.
 *   Message_Digest[ ]: [out]
 *     Where the digest is returned.
 *
 * Returns:
 *   sha Error Code.
 *
 *

int SHA512Result(SHA512Context *context,
    uint8_t Message_Digest[SHA512HashSize])
{
  return SHA384_512ResultN(context, Message_Digest, SHA512HashSize);
}

*
 * SHA384_512Reset
 *
 * Description:
 *   This helper function will initialize the SHA512Context in
 *   preparation for computing a new SHA384 or SHA512 message
 *   digest.
 *
 * Parameters:
 *   context: [in/out]
 *     The context to reset.
 *   H0[ ]: [in]
 *     The initial hash value array to use.
 *
 * Returns:
 *   sha Error Code.
 *
 *

#ifdef USE_32BIT_ONLY
static int SHA384_512Reset(SHA512Context *context,
                           uint32_t H0[SHA512HashSize/4])
#else // !USE_32BIT_ONLY
static int SHA384_512Reset(SHA512Context *context,
                           uint64_t H0[SHA512HashSize/8])
#endif // USE_32BIT_ONLY
{
  int i;
  if (!context) return shaNull;




  context-&gt;Message_Block_Index = 0;

#ifdef USE_32BIT_ONLY
  context-&gt;Length[0] = context-&gt;Length[1] =
  context-&gt;Length[2] = context-&gt;Length[3] = 0;

  for (i = 0; i &lt; SHA512HashSize/4; i++)
    context-&gt;Intermediate_Hash[i] = H0[i];
#else // !USE_32BIT_ONLY
  context-&gt;Length_High = context-&gt;Length_Low = 0;

  for (i = 0; i &lt; SHA512HashSize/8; i++)
    context-&gt;Intermediate_Hash[i] = H0[i];
#endif // USE_32BIT_ONLY

  context-&gt;Computed = 0;
  context-&gt;Corrupted = shaSuccess;

  return shaSuccess;
}

*
 * SHA384_512ProcessMessageBlock
 *
 * Description:
 *   This helper function will process the next 1024 bits of the
 *   message stored in the Message_Block array.
 *
 * Parameters:
 *   context: [in/out]
 *     The SHA context to update.
 *
 * Returns:
 *   Nothing.
 *
 * Comments:
 *   Many of the variable names in this code, especially the
 *   single character names, were used because those were the
 *   names used in the Secure Hash Standard.
 *
 *
 *
static void SHA384_512ProcessMessageBlock(SHA512Context *context)
{
#ifdef USE_32BIT_ONLY
  // Constants defined in FIPS 180-3,
  static const uint32_t K[80*2] = {
      0x428A2F98, 0xD728AE22, 0x71374491, 0x23EF65CD, 0xB5C0FBCF,



      0xEC4D3B2F, 0xE9B5DBA5, 0x8189DBBC, 0x3956C25B, 0xF348B538,
      0x59F111F1, 0xB605D019, 0x923F82A4, 0xAF194F9B, 0xAB1C5ED5,
      0xDA6D8118, 0xD807AA98, 0xA3030242, 0x12835B01, 0x45706FBE,
      0x243185BE, 0x4EE4B28C, 0x550C7DC3, 0xD5FFB4E2, 0x72BE5D74,
      0xF27B896F, 0x80DEB1FE, 0x3B1696B1, 0x9BDC06A7, 0x25C71235,
      0xC19BF174, 0xCF692694, 0xE49B69C1, 0x9EF14AD2, 0xEFBE4786,
      0x384F25E3, 0x0FC19DC6, 0x8B8CD5B5, 0x240CA1CC, 0x77AC9C65,
      0x2DE92C6F, 0x592B0275, 0x4A7484AA, 0x6EA6E483, 0x5CB0A9DC,
      0xBD41FBD4, 0x76F988DA, 0x831153B5, 0x983E5152, 0xEE66DFAB,
      0xA831C66D, 0x2DB43210, 0xB00327C8, 0x98FB213F, 0xBF597FC7,
      0xBEEF0EE4, 0xC6E00BF3, 0x3DA88FC2, 0xD5A79147, 0x930AA725,
      0x06CA6351, 0xE003826F, 0x14292967, 0x0A0E6E70, 0x27B70A85,
      0x46D22FFC, 0x2E1B2138, 0x5C26C926, 0x4D2C6DFC, 0x5AC42AED,
      0x53380D13, 0x9D95B3DF, 0x650A7354, 0x8BAF63DE, 0x766A0ABB,
      0x3C77B2A8, 0x81C2C92E, 0x47EDAEE6, 0x92722C85, 0x1482353B,
      0xA2BFE8A1, 0x4CF10364, 0xA81A664B, 0xBC423001, 0xC24B8B70,
      0xD0F89791, 0xC76C51A3, 0x0654BE30, 0xD192E819, 0xD6EF5218,
      0xD6990624, 0x5565A910, 0xF40E3585, 0x5771202A, 0x106AA070,
      0x32BBD1B8, 0x19A4C116, 0xB8D2D0C8, 0x1E376C08, 0x5141AB53,
      0x2748774C, 0xDF8EEB99, 0x34B0BCB5, 0xE19B48A8, 0x391C0CB3,
      0xC5C95A63, 0x4ED8AA4A, 0xE3418ACB, 0x5B9CCA4F, 0x7763E373,
      0x682E6FF3, 0xD6B2B8A3, 0x748F82EE, 0x5DEFB2FC, 0x78A5636F,
      0x43172F60, 0x84C87814, 0xA1F0AB72, 0x8CC70208, 0x1A6439EC,
      0x90BEFFFA, 0x23631E28, 0xA4506CEB, 0xDE82BDE9, 0xBEF9A3F7,
      0xB2C67915, 0xC67178F2, 0xE372532B, 0xCA273ECE, 0xEA26619C,
      0xD186B8C7, 0x21C0C207, 0xEADA7DD6, 0xCDE0EB1E, 0xF57D4F7F,
      0xEE6ED178, 0x06F067AA, 0x72176FBA, 0x0A637DC5, 0xA2C898A6,
      0x113F9804, 0xBEF90DAE, 0x1B710B35, 0x131C471B, 0x28DB77F5,
      0x23047D84, 0x32CAAB7B, 0x40C72493, 0x3C9EBE0A, 0x15C9BEBC,
      0x431D67C4, 0x9C100D4C, 0x4CC5D4BE, 0xCB3E42B6, 0x597F299C,
      0xFC657E2A, 0x5FCB6FAB, 0x3AD6FAEC, 0x6C44198C, 0x4A475817
  };

  int     t, t2, t8;     // Loop counter
  uint32_t  temp1[2], temp2[2],
           // Temporary word values
        temp3[2], temp4[2], temp5[2];
  uint32_t  W[2*80];               // Word sequence
  uint32_t  A[2], B[2], C[2], D[2], // Word buffers
        E[2], F[2], G[2], H[2];

  // Initialize the first 16 words in the array W
  for (t = t2 = t8 = 0; t &lt; 16; t++, t8 += 8) {
    W[t2++] = ((((uint32_t)context-&gt;Message_Block[t8    ])) &lt;&lt; 24) |
              ((((uint32_t)context-&gt;Message_Block[t8 + 1])) &lt;&lt; 16) |
              ((((uint32_t)context-&gt;Message_Block[t8 + 2])) &lt;&lt; 8) |
              ((((uint32_t)context-&gt;Message_Block[t8 + 3])));
    W[t2++] = ((((uint32_t)context-&gt;Message_Block[t8 + 4])) &lt;&lt; 24) |
              ((((uint32_t)context-&gt;Message_Block[t8 + 5])) &lt;&lt; 16) |
              ((((uint32_t)context-&gt;Message_Block[t8 + 6])) &lt;&lt; 8) |



              ((((uint32_t)context-&gt;Message_Block[t8 + 7])));
  }

  for (t = 16; t &lt; 80; t++, t2 += 2) {
    // W[t] = SHA512_sigma1(W[t-2]) + W[t-7] +
    //  SHA512_sigma0(W[t-15]) + W[t-16];
    uint32_t *Wt2 = &amp;W[t2-2*2];
    uint32_t *Wt7 = &amp;W[t2-7*2];
    uint32_t *Wt15 = &amp;W[t2-15*2];
    uint32_t *Wt16 = &amp;W[t2-16*2];
    SHA512_sigma1(Wt2, temp1);
    SHA512_ADD(temp1, Wt7, temp2);
    SHA512_sigma0(Wt15, temp1);
    SHA512_ADD(temp1, Wt16, temp3);
    SHA512_ADD(temp2, temp3, &amp;W[t2]);
  }

  A[0] = context-&gt;Intermediate_Hash[0];
  A[1] = context-&gt;Intermediate_Hash[1];
  B[0] = context-&gt;Intermediate_Hash[2];
  B[1] = context-&gt;Intermediate_Hash[3];
  C[0] = context-&gt;Intermediate_Hash[4];
  C[1] = context-&gt;Intermediate_Hash[5];
  D[0] = context-&gt;Intermediate_Hash[6];
  D[1] = context-&gt;Intermediate_Hash[7];
  E[0] = context-&gt;Intermediate_Hash[8];
  E[1] = context-&gt;Intermediate_Hash[9];
  F[0] = context-&gt;Intermediate_Hash[10];
  F[1] = context-&gt;Intermediate_Hash[11];
  G[0] = context-&gt;Intermediate_Hash[12];
  G[1] = context-&gt;Intermediate_Hash[13];
  H[0] = context-&gt;Intermediate_Hash[14];
  H[1] = context-&gt;Intermediate_Hash[15];

  for (t = t2 = 0; t &lt; 80; t++, t2 += 2) {
    //
     * temp1 = H + SHA512_SIGMA1(E) + SHA_Ch(E,F,G) + K[t] + W[t];
     *
    SHA512_SIGMA1(E,temp1);
    SHA512_ADD(H, temp1, temp2);
    SHA_Ch(E,F,G,temp3);
    SHA512_ADD(temp2, temp3, temp4);
    SHA512_ADD(&amp;K[t2], &amp;W[t2], temp5);
    SHA512_ADD(temp4, temp5, temp1);
    *
     * temp2 = SHA512_SIGMA0(A) + SHA_Maj(A,B,C);
     *
    SHA512_SIGMA0(A,temp3);



    SHA_Maj(A,B,C,temp4);
    SHA512_ADD(temp3, temp4, temp2);
    H[0] = G[0]; H[1] = G[1];
    G[0] = F[0]; G[1] = F[1];
    F[0] = E[0]; F[1] = E[1];
    SHA512_ADD(D, temp1, E);
    D[0] = C[0]; D[1] = C[1];
    C[0] = B[0]; C[1] = B[1];
    B[0] = A[0]; B[1] = A[1];
    SHA512_ADD(temp1, temp2, A);
  }

  SHA512_ADDTO2(&amp;context-&gt;Intermediate_Hash[0], A);
  SHA512_ADDTO2(&amp;context-&gt;Intermediate_Hash[2], B);
  SHA512_ADDTO2(&amp;context-&gt;Intermediate_Hash[4], C);
  SHA512_ADDTO2(&amp;context-&gt;Intermediate_Hash[6], D);
  SHA512_ADDTO2(&amp;context-&gt;Intermediate_Hash[8], E);
  SHA512_ADDTO2(&amp;context-&gt;Intermediate_Hash[10], F);
  SHA512_ADDTO2(&amp;context-&gt;Intermediate_Hash[12], G);
  SHA512_ADDTO2(&amp;context-&gt;Intermediate_Hash[14], H);

#else // !USE_32BIT_ONLY
  // Constants defined in FIPS 180-3,

  static const uint64_t K[80] = {
      0x428A2F98D728AE22ll, 0x7137449123EF65CDll, 0xB5C0FBCFEC4D3B2Fll,
      0xE9B5DBA58189DBBCll, 0x3956C25BF348B538ll, 0x59F111F1B605D019ll,
      0x923F82A4AF194F9Bll, 0xAB1C5ED5DA6D8118ll, 0xD807AA98A3030242ll,
      0x12835B0145706FBEll, 0x243185BE4EE4B28Cll, 0x550C7DC3D5FFB4E2ll,
      0x72BE5D74F27B896Fll, 0x80DEB1FE3B1696B1ll, 0x9BDC06A725C71235ll,
      0xC19BF174CF692694ll, 0xE49B69C19EF14AD2ll, 0xEFBE4786384F25E3ll,
      0x0FC19DC68B8CD5B5ll, 0x240CA1CC77AC9C65ll, 0x2DE92C6F592B0275ll,
      0x4A7484AA6EA6E483ll, 0x5CB0A9DCBD41FBD4ll, 0x76F988DA831153B5ll,
      0x983E5152EE66DFABll, 0xA831C66D2DB43210ll, 0xB00327C898FB213Fll,
      0xBF597FC7BEEF0EE4ll, 0xC6E00BF33DA88FC2ll, 0xD5A79147930AA725ll,
      0x06CA6351E003826Fll, 0x142929670A0E6E70ll, 0x27B70A8546D22FFCll,
      0x2E1B21385C26C926ll, 0x4D2C6DFC5AC42AEDll, 0x53380D139D95B3DFll,
      0x650A73548BAF63DEll, 0x766A0ABB3C77B2A8ll, 0x81C2C92E47EDAEE6ll,
      0x92722C851482353Bll, 0xA2BFE8A14CF10364ll, 0xA81A664BBC423001ll,
      0xC24B8B70D0F89791ll, 0xC76C51A30654BE30ll, 0xD192E819D6EF5218ll,
      0xD69906245565A910ll, 0xF40E35855771202All, 0x106AA07032BBD1B8ll,
      0x19A4C116B8D2D0C8ll, 0x1E376C085141AB53ll, 0x2748774CDF8EEB99ll,
      0x34B0BCB5E19B48A8ll, 0x391C0CB3C5C95A63ll, 0x4ED8AA4AE3418ACBll,
      0x5B9CCA4F7763E373ll, 0x682E6FF3D6B2B8A3ll, 0x748F82EE5DEFB2FCll,
      0x78A5636F43172F60ll, 0x84C87814A1F0AB72ll, 0x8CC702081A6439ECll,
      0x90BEFFFA23631E28ll, 0xA4506CEBDE82BDE9ll, 0xBEF9A3F7B2C67915ll,
      0xC67178F2E372532Bll, 0xCA273ECEEA26619Cll, 0xD186B8C721C0C207ll,
      0xEADA7DD6CDE0EB1Ell, 0xF57D4F7FEE6ED178ll, 0x06F067AA72176FBAll,
      0x0A637DC5A2C898A6ll, 0x113F9804BEF90DAEll, 0x1B710B35131C471Bll,



      0x28DB77F523047D84ll, 0x32CAAB7B40C72493ll, 0x3C9EBE0A15C9BEBCll,
      0x431D67C49C100D4Cll, 0x4CC5D4BECB3E42B6ll, 0x597F299CFC657E2All,
      0x5FCB6FAB3AD6FAECll, 0x6C44198C4A475817ll
  };
  int        t, t8;  // Loop counter
  uint64_t   temp1, temp2;
        // Temporary word value
  uint64_t   W[80];     // Word sequence
  uint64_t   A, B, C, D, E, F, G, H;
           // Word buffers

  *
   * Initialize the first 16 words in the array W
   *
  for (t = t8 = 0; t &lt; 16; t++, t8 += 8)
    W[t] = ((uint64_t)(context-&gt;Message_Block[t8  ]) &lt;&lt; 56) |
           ((uint64_t)(context-&gt;Message_Block[t8 + 1]) &lt;&lt; 48) |
           ((uint64_t)(context-&gt;Message_Block[t8 + 2]) &lt;&lt; 40) |
           ((uint64_t)(context-&gt;Message_Block[t8 + 3]) &lt;&lt; 32) |
           ((uint64_t)(context-&gt;Message_Block[t8 + 4]) &lt;&lt; 24) |
           ((uint64_t)(context-&gt;Message_Block[t8 + 5]) &lt;&lt; 16) |
           ((uint64_t)(context-&gt;Message_Block[t8 + 6]) &lt;&lt; 8) |
           ((uint64_t)(context-&gt;Message_Block[t8 + 7]));

  for (t = 16; t &lt; 80; t++)
    W[t] = SHA512_sigma1(W[t-2]) + W[t-7] +
        SHA512_sigma0(W[t-15]) + W[t-16];
  A = context-&gt;Intermediate_Hash[0];
  B = context-&gt;Intermediate_Hash[1];
  C = context-&gt;Intermediate_Hash[2];
  D = context-&gt;Intermediate_Hash[3];
  E = context-&gt;Intermediate_Hash[4];
  F = context-&gt;Intermediate_Hash[5];
  G = context-&gt;Intermediate_Hash[6];
  H = context-&gt;Intermediate_Hash[7];

  for (t = 0; t &lt; 80; t++) {
    temp1 = H + SHA512_SIGMA1(E) + SHA_Ch(E,F,G) + K[t] + W[t];
    temp2 = SHA512_SIGMA0(A) + SHA_Maj(A,B,C);
    H = G;
    G = F;
    F = E;
    E = D + temp1;
    D = C;
    C = B;
    B = A;
    A = temp1 + temp2;
  }

  context-&gt;Intermediate_Hash[0] += A;



  context-&gt;Intermediate_Hash[1] += B;
  context-&gt;Intermediate_Hash[2] += C;
  context-&gt;Intermediate_Hash[3] += D;
  context-&gt;Intermediate_Hash[4] += E;
  context-&gt;Intermediate_Hash[5] += F;
  context-&gt;Intermediate_Hash[6] += G;
  context-&gt;Intermediate_Hash[7] += H;
#endif // USE_32BIT_ONLY

  context-&gt;Message_Block_Index = 0;
}


*
 * SHA384_512Finalize
 *
 * Description:
 *   This helper function finishes off the digest calculations.
 *
 * Parameters:
 *   context: [in/out]
 *     The SHA context to update.
 *   Pad_Byte: [in]
 *     The last byte to add to the message block before the 0-padding
 *     and length.  This will contain the last bits of the message
 *     followed by another single bit.  If the message was an
 *     exact multiple of 8-bits long, Pad_Byte will be 0x80.
 *
 * Returns:
 *   sha Error Code.
 *
 *

static void SHA384_512Finalize(SHA512Context *context,
    uint8_t Pad_Byte)
{
  int_least16_t i;
  SHA384_512PadMessage(context, Pad_Byte);
  // message may be sensitive, clear it out
  for (i = 0; i &lt; SHA512_Message_Block_Size; ++i)
    context-&gt;Message_Block[i] = 0;
#ifdef USE_32BIT_ONLY    // and clear length
  context-&gt;Length[0] = context-&gt;Length[1] = 0;
  context-&gt;Length[2] = context-&gt;Length[3] = 0;
#else // !USE_32BIT_ONLY
  context-&gt;Length_High = context-&gt;Length_Low = 0;
#endif // USE_32BIT_ONLY
  context-&gt;Computed = 1;
}




*
 * SHA384_512PadMessage
 *
 * Description:
 *   According to the standard, the message must be padded to the next
 *   even multiple of 1024 bits.  The first padding bit must be a '1'.
 *   The last 128 bits represent the length of the original message.
 *   All bits in between should be 0.  This helper function will
 *   pad the message according to those rules by filling the
 *   Message_Block array accordingly.  When it returns, it can be
 *   assumed that the message digest has been computed.
 *
 * Parameters:
 *   context: [in/out]
 *     The context to pad.
 *   Pad_Byte: [in]
 *     The last byte to add to the message block before the 0-padding
 *     and length.  This will contain the last bits of the message
 *     followed by another single bit.  If the message was an
 *     exact multiple of 8-bits long, Pad_Byte will be 0x80.
 *
 * Returns:
 *   Nothing.
 *
 *
static void SHA384_512PadMessage(SHA512Context *context,
    uint8_t Pad_Byte)
{
  *
   * Check to see if the current message block is too small to hold
   * the initial padding bits and length.  If so, we will pad the
   * block, process it, and then continue padding into a second
   * block.
   *
  if (context-&gt;Message_Block_Index &gt;= (SHA512_Message_Block_Size-16)) {
    context-&gt;Message_Block[context-&gt;Message_Block_Index++] = Pad_Byte;
    while (context-&gt;Message_Block_Index &lt; SHA512_Message_Block_Size)
      context-&gt;Message_Block[context-&gt;Message_Block_Index++] = 0;

    SHA384_512ProcessMessageBlock(context);
  } else
    context-&gt;Message_Block[context-&gt;Message_Block_Index++] = Pad_Byte;

  while (context-&gt;Message_Block_Index &lt; (SHA512_Message_Block_Size-16))
    context-&gt;Message_Block[context-&gt;Message_Block_Index++] = 0;

  *
   * Store the message length as the last 16 octets



#ifdef USE_32BIT_ONLY
  context-&gt;Message_Block[112] = (uint8_t)(context-&gt;Length[0] &gt;&gt; 24);
  context-&gt;Message_Block[113] = (uint8_t)(context-&gt;Length[0] &gt;&gt; 16);
  context-&gt;Message_Block[114] = (uint8_t)(context-&gt;Length[0] &gt;&gt; 8);
  context-&gt;Message_Block[115] = (uint8_t)(context-&gt;Length[0]);
  context-&gt;Message_Block[116] = (uint8_t)(context-&gt;Length[1] &gt;&gt; 24);
  context-&gt;Message_Block[117] = (uint8_t)(context-&gt;Length[1] &gt;&gt; 16);
  context-&gt;Message_Block[118] = (uint8_t)(context-&gt;Length[1] &gt;&gt; 8);
  context-&gt;Message_Block[119] = (uint8_t)(context-&gt;Length[1]);

  context-&gt;Message_Block[120] = (uint8_t)(context-&gt;Length[2] &gt;&gt; 24);
  context-&gt;Message_Block[121] = (uint8_t)(context-&gt;Length[2] &gt;&gt; 16);
  context-&gt;Message_Block[122] = (uint8_t)(context-&gt;Length[2] &gt;&gt; 8);
  context-&gt;Message_Block[123] = (uint8_t)(context-&gt;Length[2]);
  context-&gt;Message_Block[124] = (uint8_t)(context-&gt;Length[3] &gt;&gt; 24);
  context-&gt;Message_Block[125] = (uint8_t)(context-&gt;Length[3] &gt;&gt; 16);
  context-&gt;Message_Block[126] = (uint8_t)(context-&gt;Length[3] &gt;&gt; 8);
  context-&gt;Message_Block[127] = (uint8_t)(context-&gt;Length[3]);
#else // !USE_32BIT_ONLY
  context-&gt;Message_Block[112] = (uint8_t)(context-&gt;Length_High &gt;&gt; 56);
  context-&gt;Message_Block[113] = (uint8_t)(context-&gt;Length_High &gt;&gt; 48);
  context-&gt;Message_Block[114] = (uint8_t)(context-&gt;Length_High &gt;&gt; 40);
  context-&gt;Message_Block[115] = (uint8_t)(context-&gt;Length_High &gt;&gt; 32);
  context-&gt;Message_Block[116] = (uint8_t)(context-&gt;Length_High &gt;&gt; 24);
  context-&gt;Message_Block[117] = (uint8_t)(context-&gt;Length_High &gt;&gt; 16);
  context-&gt;Message_Block[118] = (uint8_t)(context-&gt;Length_High &gt;&gt; 8);
  context-&gt;Message_Block[119] = (uint8_t)(context-&gt;Length_High);

  context-&gt;Message_Block[120] = (uint8_t)(context-&gt;Length_Low &gt;&gt; 56);
  context-&gt;Message_Block[121] = (uint8_t)(context-&gt;Length_Low &gt;&gt; 48);
  context-&gt;Message_Block[122] = (uint8_t)(context-&gt;Length_Low &gt;&gt; 40);
  context-&gt;Message_Block[123] = (uint8_t)(context-&gt;Length_Low &gt;&gt; 32);
  context-&gt;Message_Block[124] = (uint8_t)(context-&gt;Length_Low &gt;&gt; 24);
  context-&gt;Message_Block[125] = (uint8_t)(context-&gt;Length_Low &gt;&gt; 16);
  context-&gt;Message_Block[126] = (uint8_t)(context-&gt;Length_Low &gt;&gt; 8);
  context-&gt;Message_Block[127] = (uint8_t)(context-&gt;Length_Low);
#endif // USE_32BIT_ONLY

  SHA384_512ProcessMessageBlock(context);
}

*
 * SHA384_512ResultN
 *
 * Description:
 *   This helper function will return the 384-bit or 512-bit message
 *   digest into the Message_Digest array provided by the caller.



 *   NOTE:
 *    The first octet of hash is stored in the element with index 0,
 *    the last octet of hash in the element with index 47/63.
 *
 * Parameters:
 *   context: [in/out]
 *     The context to use to calculate the SHA hash.
 *   Message_Digest[ ]: [out]
 *     Where the digest is returned.
 *   HashSize: [in]
 *     The size of the hash, either 48 or 64.
 *
 * Returns:
 *   sha Error Code.
 *
 *
static int SHA384_512ResultN(SHA512Context *context,
    uint8_t Message_Digest[ ], int HashSize)
{
  int i;
#ifdef USE_32BIT_ONLY
  int i2;
#endif // USE_32BIT_ONLY

  if (!context) return shaNull;
  if (!Message_Digest) return shaNull;
  if (context-&gt;Corrupted) return context-&gt;Corrupted;

  if (!context-&gt;Computed)
    SHA384_512Finalize(context, 0x80);

#ifdef USE_32BIT_ONLY
  for (i = i2 = 0; i &lt; HashSize; ) {
    Message_Digest[i++]=(uint8_t)(context-&gt;Intermediate_Hash[i2]&gt;&gt;24);
    Message_Digest[i++]=(uint8_t)(context-&gt;Intermediate_Hash[i2]&gt;&gt;16);
    Message_Digest[i++]=(uint8_t)(context-&gt;Intermediate_Hash[i2]&gt;&gt;8);
    Message_Digest[i++]=(uint8_t)(context-&gt;Intermediate_Hash[i2++]);
    Message_Digest[i++]=(uint8_t)(context-&gt;Intermediate_Hash[i2]&gt;&gt;24);
    Message_Digest[i++]=(uint8_t)(context-&gt;Intermediate_Hash[i2]&gt;&gt;16);
    Message_Digest[i++]=(uint8_t)(context-&gt;Intermediate_Hash[i2]&gt;&gt;8);
    Message_Digest[i++]=(uint8_t)(context-&gt;Intermediate_Hash[i2++]);
  }
#else // !USE_32BIT_ONLY
  for (i = 0; i &lt; HashSize; ++i)
    Message_Digest[i] = (uint8_t)
      (context-&gt;Intermediate_Hash[i&gt;&gt;3] &gt;&gt; 8 * ( 7 - ( i % 8 ) ));
#endif // USE_32BIT_ONLY



  return shaSuccess;
}





// usha.c

 *  Description:
 *     This file implements a unified interface to the SHA algorithms.
 *

#include "sha.h"

*
 *  USHAReset
 *
 *  Description:
 *      This function will initialize the SHA Context in preparation
 *      for computing a new SHA message digest.
 *
 *  Parameters:
 *      context: [in/out]
 *          The context to reset.
 *      whichSha: [in]
 *          Selects which SHA reset to call
 *
 *  Returns:
 *      sha Error Code.
 *
 *
int USHAReset(USHAContext *context, enum SHAversion whichSha)
{
  if (!context) return shaNull;
  context-&gt;whichSha = whichSha;
  switch (whichSha) {
    case SHA1:   return SHA1Reset((SHA1Context*)&amp;context-&gt;ctx);
    case SHA224: return SHA224Reset((SHA224Context*)&amp;context-&gt;ctx);
    case SHA256: return SHA256Reset((SHA256Context*)&amp;context-&gt;ctx);
    case SHA384: return SHA384Reset((SHA384Context*)&amp;context-&gt;ctx);
    case SHA512: return SHA512Reset((SHA512Context*)&amp;context-&gt;ctx);
    default: return shaBadParam;
  }
}


*
 *  USHAInput
 *
 *  Description:
 *      This function accepts an array of octets as the next portion
 *      of the message.
 *
 *  Parameters:
 *      context: [in/out]
 *          The SHA context to update.
 *      message_array: [in]
 *          An array of octets representing the next portion of
 *          the message.
 *      length: [in]
 *          The length of the message in message_array.
 *
 *  Returns:
 *      sha Error Code.
 *
 *
int USHAInput(USHAContext *context,
              const uint8_t *bytes, unsigned int bytecount)
{
  if (!context) return shaNull;
  switch (context-&gt;whichSha) {
    case SHA1:
      return SHA1Input((SHA1Context*)&amp;context-&gt;ctx, bytes,
                       bytecount);
    case SHA224:
      return SHA224Input((SHA224Context*)&amp;context-&gt;ctx, bytes,
          bytecount);
    case SHA256:
      return SHA256Input((SHA256Context*)&amp;context-&gt;ctx, bytes,
          bytecount);
    case SHA384:
      return SHA384Input((SHA384Context*)&amp;context-&gt;ctx, bytes,
          bytecount);
    case SHA512:
      return SHA512Input((SHA512Context*)&amp;context-&gt;ctx, bytes,
          bytecount);
    default: return shaBadParam;
  }
}







*
 * USHAFinalBits
 *
 * Description:
 *   This function will add in any final bits of the message.
 *
 * Parameters:
 *   context: [in/out]
 *     The SHA context to update.
 *   message_bits: [in]
 *     The final bits of the message, in the upper portion of the
 *     byte.  (Use 0b###00000 instead of 0b00000### to input the
 *     three bits ###.)
 *   length: [in]
 *     The number of bits in message_bits, between 1 and 7.
 *
 * Returns:
 *   sha Error Code.
 *
int USHAFinalBits(USHAContext *context,
                  uint8_t bits, unsigned int bit_count)
{
  if (!context) return shaNull;
  switch (context-&gt;whichSha) {
    case SHA1:
      return SHA1FinalBits((SHA1Context*)&amp;context-&gt;ctx, bits,
                           bit_count);
    case SHA224:
      return SHA224FinalBits((SHA224Context*)&amp;context-&gt;ctx, bits,
          bit_count);
    case SHA256:
      return SHA256FinalBits((SHA256Context*)&amp;context-&gt;ctx, bits,
          bit_count);
    case SHA384:
      return SHA384FinalBits((SHA384Context*)&amp;context-&gt;ctx, bits,
          bit_count);
    case SHA512:
      return SHA512FinalBits((SHA512Context*)&amp;context-&gt;ctx, bits,
          bit_count);
    default: return shaBadParam;
  }
}







*
 * USHAResult
 *
 * Description:
 *   This function will return the message digest of the appropriate
 *   bit size, as returned by USHAHashSizeBits(whichSHA) for the
 *   'whichSHA' value used in the preceeding call to USHAReset,
 *   into the Message_Digest array provided by the caller.
 *
 * Parameters:
 *   context: [in/out]
 *     The context to use to calculate the SHA-1 hash.
 *   Message_Digest: [out]
 *     Where the digest is returned.
 *
 * Returns:
 *   sha Error Code.
 *
 *
int USHAResult(USHAContext *context,
               uint8_t Message_Digest[USHAMaxHashSize])
{
  if (!context) return shaNull;
  switch (context-&gt;whichSha) {
    case SHA1:
      return SHA1Result((SHA1Context*)&amp;context-&gt;ctx, Message_Digest);
    case SHA224:
      return SHA224Result((SHA224Context*)&amp;context-&gt;ctx,
                          Message_Digest);
    case SHA256:
      return SHA256Result((SHA256Context*)&amp;context-&gt;ctx,
                          Message_Digest);
    case SHA384:
      return SHA384Result((SHA384Context*)&amp;context-&gt;ctx,
                          Message_Digest);
    case SHA512:
      return SHA512Result((SHA512Context*)&amp;context-&gt;ctx,
                          Message_Digest);
    default: return shaBadParam;
  }
}

*
 * USHABlockSize
 *
 * Description:
 *   This function will return the blocksize for the given SHA
 *   algorithm.



 *
 * Parameters:
 *   whichSha:
 *     which SHA algorithm to query
 *
 * Returns:
 *   block size
 *
 *
int USHABlockSize(enum SHAversion whichSha)
{
  switch (whichSha) {
    case SHA1:   return SHA1_Message_Block_Size;
    case SHA224: return SHA224_Message_Block_Size;
    case SHA256: return SHA256_Message_Block_Size;
    case SHA384: return SHA384_Message_Block_Size;
    default:
    case SHA512: return SHA512_Message_Block_Size;
  }
}


*
 * USHAHashSize
 *
 * Description:
 *   This function will return the hashsize for the given SHA
 *   algorithm.
 *
 * Parameters:
 *   whichSha:
 *     which SHA algorithm to query
 *
 * Returns:
 *   hash size
 *
 *
int USHAHashSize(enum SHAversion whichSha)
{
  switch (whichSha) {
    case SHA1:   return SHA1HashSize;
    case SHA224: return SHA224HashSize;
    case SHA256: return SHA256HashSize;
    case SHA384: return SHA384HashSize;
    default:
    case SHA512: return SHA512HashSize;
  }
}



*
 * USHAHashSizeBits
 *
 * Description:
 *   This function will return the hashsize for the given SHA
 *   algorithm, expressed in bits.
 *
 * Parameters:
 *   whichSha:
 *     which SHA algorithm to query
 *
 * Returns:
 *   hash size in bits
 *
 *
int USHAHashSizeBits(enum SHAversion whichSha)
{
  switch (whichSha) {
    case SHA1:   return SHA1HashSizeBits;
    case SHA224: return SHA224HashSizeBits;
    case SHA256: return SHA256HashSizeBits;
    case SHA384: return SHA384HashSizeBits;
    default:
    case SHA512: return SHA512HashSizeBits;
  }
}

*
 * USHAHashName
 *
 * Description:
 *   This function will return the name of the given SHA algorithm
 *   as a string.
 *
 * Parameters:
 *   whichSha:
 *     which SHA algorithm to query
 *
 * Returns:
 *   character string with the name in it
 *
 *
const char *USHAHashName(enum SHAversion whichSha)
{
  switch (whichSha) {
    case SHA1:   return "SHA1";
    case SHA224: return "SHA224";
    case SHA256: return "SHA256";




    case SHA384: return "SHA384";
    default:
    case SHA512: return "SHA512";
  }
}




//  The HMAC Code

*
 *  Description:
 *      This file implements the HMAC algorithm (Keyed-Hashing for
 *      Message Authentication, [<a href="https://datatracker.ietf.org/doc/html/rfc2104">RFC 2104</a>]), expressed in terms of
 *      the various SHA algorithms.
 *

#include "sha.h"

*
 *  hmac
 *
 *  Description:
 *      This function will compute an HMAC message digest.
 *
 *  Parameters:
 *      whichSha: [in]
 *          One of SHA1, SHA224, SHA256, SHA384, SHA512
 *      message_array[ ]: [in]
 *          An array of octets representing the message.
 *          Note: in <a href="https://datatracker.ietf.org/doc/html/rfc2104">RFC 2104</a>, this parameter is known
 *          as 'text'.
 *      length: [in]
 *          The length of the message in message_array.
 *      key[ ]: [in]
 *          The secret shared key.
 *      key_len: [in]
 *          The length of the secret shared key.
 *      digest[ ]: [out]
 *          Where the digest is to be returned.
 *          NOTE: The length of the digest is determined by
 *              the value of whichSha.
 *




 *  Returns:
 *      sha Error Code.
 *
 *

int hmac(SHAversion whichSha,
    const unsigned char *message_array, int length,
    const unsigned char *key, int key_len,
    uint8_t digest[USHAMaxHashSize])
{
  HMACContext context;
  return hmacReset(&amp;context, whichSha, key, key_len) ||
         hmacInput(&amp;context, message_array, length) ||
         hmacResult(&amp;context, digest);
}

*
 *  hmacReset
 *
 *  Description:
 *      This function will initialize the hmacContext in preparation
 *      for computing a new HMAC message digest.
 *
 *  Parameters:
 *      context: [in/out]
 *          The context to reset.
 *      whichSha: [in]
 *          One of SHA1, SHA224, SHA256, SHA384, SHA512
 *      key[ ]: [in]
 *          The secret shared key.
 *      key_len: [in]
 *          The length of the secret shared key.
 *
 *  Returns:
 *      sha Error Code.
 *
 *

int hmacReset(HMACContext *context, enum SHAversion whichSha,
    const unsigned char *key, int key_len)
{
  int i, blocksize, hashsize, ret;

  // inner padding - key XORd with ipad
  unsigned char k_ipad[USHA_Max_Message_Block_Size];

  // temporary buffer when keylen &gt; blocksize

  unsigned char tempkey[USHAMaxHashSize];




  if (!context) return shaNull;
  context-&gt;Computed = 0;
  context-&gt;Corrupted = shaSuccess;

  blocksize = context-&gt;blockSize = USHABlockSize(whichSha);
  hashsize = context-&gt;hashSize = USHAHashSize(whichSha);
  context-&gt;whichSha = whichSha;

  *
   * If key is longer than the hash blocksize,
   * reset it to key = HASH(key).
   *
  if (key_len &gt; blocksize) {
    USHAContext tcontext;
    int err = USHAReset(&amp;tcontext, whichSha) ||
              USHAInput(&amp;tcontext, key, key_len) ||
              USHAResult(&amp;tcontext, tempkey);
    if (err != shaSuccess) return err;

    key = tempkey;
    key_len = hashsize;
  }

  *
   * The HMAC transform looks like:
   *
   * SHA(K XOR opad, SHA(K XOR ipad, text))
   *
   * where K is an n byte key, 0-padded to a total of blocksize bytes,
   * ipad is the byte 0x36 repeated blocksize times,
   * opad is the byte 0x5c repeated blocksize times,
   * and text is the data being protected.
   *

  * store key into the pads, XOR'd with ipad
// and opad values

  for (i = 0; i &lt; key_len; i++) {
    k_ipad[i] = key[i] ^ 0x36;
    context-&gt;k_opad[i] = key[i] ^ 0x5c;
  }
  // remaining pad bytes are '\0' XOR'd
  // with ipad and opad values
  for ( ; i &lt; blocksize; i++) {
    k_ipad[i] = 0x36;
    context-&gt;k_opad[i] = 0x5c;
  }

  // perform inner hash
  // init context for 1st pass
  ret = USHAReset(&amp;context-&gt;shaContext, whichSha) ||



// and start with inner pad
        USHAInput(&amp;context-&gt;shaContext, k_ipad, blocksize);
  return context-&gt;Corrupted = ret;
}



*
 *  hmacInput
 *
 *  Description:
 *      This function accepts an array of octets as the next portion
 *      of the message.  It may be called multiple times.
 *
 *  Parameters:
 *      context: [in/out]
 *          The HMAC context to update.
 *      text[ ]: [in]
 *          An array of octets representing the next portion of
 *          the message.
 *      text_len: [in]
 *          The length of the message in text.
 *
 *  Returns:
 *      sha Error Code.
 *
 *
int hmacInput(HMACContext *context, const unsigned char *text,
    int text_len)
{
  if (!context) return shaNull;
  if (context-&gt;Corrupted) return context-&gt;Corrupted;
  if (context-&gt;Computed) return context-&gt;Corrupted = shaStateError;
  // then text of datagram
  return context-&gt;Corrupted =
    USHAInput(&amp;context-&gt;shaContext, text, text_len);
}

*
 * hmacFinalBits
 *
 * Description:
 *   This function will add in any final bits of the message.
 *
 * Parameters:
 *   context: [in/out]
 *     The HMAC context to update.
 *   message_bits: [in]
 *     The final bits of the message, in the upper portion of the
 *     byte.  (Use 0b###00000 instead of 0b00000### to input the

 *     three bits ###.)
 *   length: [in]
 *     The number of bits in message_bits, between 1 and 7.
 *
 * Returns:
 *   sha Error Code.
 *
int hmacFinalBits(HMACContext *context,
    uint8_t bits, unsigned int bit_count)
{
  if (!context) return shaNull;
  if (context-&gt;Corrupted) return context-&gt;Corrupted;
  if (context-&gt;Computed) return context-&gt;Corrupted = shaStateError;
  // then final bits of datagram
  return context-&gt;Corrupted =
    USHAFinalBits(&amp;context-&gt;shaContext, bits, bit_count);
}

*
 * hmacResult
 *
 * Description:
 *   This function will return the N-byte message digest into the
 *   Message_Digest array provided by the caller.
 *
 * Parameters:
 *   context: [in/out]
 *     The context to use to calculate the HMAC hash.
 *   digest[ ]: [out]
 *     Where the digest is returned.
 *     NOTE 2: The length of the hash is determined by the value of
 *      whichSha that was passed to hmacReset().
 *
 * Returns:
 *   sha Error Code.
 *
 *
int hmacResult(HMACContext *context, uint8_t *digest)
{
  int ret;
  if (!context) return shaNull;
  if (context-&gt;Corrupted) return context-&gt;Corrupted;
  if (context-&gt;Computed) return context-&gt;Corrupted = shaStateError;

  // finish up 1st pass
  // (Use digest here as a temporary buffer.)
  ret =
    USHAResult(&amp;context-&gt;shaContext, digest) ||


         // perform outer SHA
         // init context for 2nd pass
         USHAReset(&amp;context-&gt;shaContext, context-&gt;whichSha) ||

         // start with outer pad
         USHAInput(&amp;context-&gt;shaContext, context-&gt;k_opad,
                   context-&gt;blockSize) ||

         // then results of 1st hash
         USHAInput(&amp;context-&gt;shaContext, digest, context-&gt;hashSize) ||
         // finish up 2nd pass
         USHAResult(&amp;context-&gt;shaContext, digest);

  context-&gt;Computed = 1;
  return context-&gt;Corrupted = ret;
}




// The HKDF Code

********** hkdf.c ****
*
 *  Description:
 *      This file implements the HKDF algorithm (HMAC-based
 *      Extract-and-Expand Key Derivation Function, <a href="https://datatracker.ietf.org/doc/html/rfc5869">RFC 5869</a>),
 *      expressed in terms of the various SHA algorithms.
 *

#include "sha.h"
#include &lt;string.h&gt;
#include &lt;stdlib.h&gt;

*
 *  hkdf
 *
 *  Description:
 *      This function will generate keying material using HKDF.
 *
 *  Parameters:
 *      whichSha: [in]
 *          One of SHA1, SHA224, SHA256, SHA384, SHA512
 *      salt[ ]: [in]
 *          The optional salt value (a non-secret random value);
 *          if not provided (salt == NULL), it is set internally



 *          to a string of HashLen(whichSha) zeros.
 *      salt_len: [in]
 *          The length of the salt value.  (Ignored if salt == NULL.)
 *      ikm[ ]: [in]
 *          Input keying material.
 *      ikm_len: [in]
 *          The length of the input keying material.
 *      info[ ]: [in]
 *          The optional context and application specific information.
 *          If info == NULL or a zero-length string, it is ignored.
 *      info_len: [in]
 *          The length of the optional context and application specific
 *          information.  (Ignored if info == NULL.)
 *      okm[ ]: [out]
 *          Where the HKDF is to be stored.
 *      okm_len: [in]
 *          The length of the buffer to hold okm.
 *          okm_len must be &lt;= 255 * USHABlockSize(whichSha)
 *
 *  Notes:
 *      Calls hkdfExtract() and hkdfExpand().
 *
 *  Returns:
 *      sha Error Code.
 *
 *
int hkdf(SHAversion whichSha,
    const unsigned char *salt, int salt_len,
    const unsigned char *ikm, int ikm_len,
    const unsigned char *info, int info_len,
    uint8_t okm[ ], int okm_len)
{
  uint8_t prk[USHAMaxHashSize];
  return hkdfExtract(whichSha, salt, salt_len, ikm, ikm_len, prk) ||
         hkdfExpand(whichSha, prk, USHAHashSize(whichSha), info,
                    info_len, okm, okm_len);
}

*
 *  hkdfExtract
 *
 *  Description:
 *      This function will perform HKDF extraction.
 *
 *  Parameters:
 *      whichSha: [in]
 *          One of SHA1, SHA224, SHA256, SHA384, SHA512
 *      salt[ ]: [in]



 *          The optional salt value (a non-secret random value);
 *          if not provided (salt == NULL), it is set internally
 *          to a string of HashLen(whichSha) zeros.
 *      salt_len: [in]
 *          The length of the salt value.  (Ignored if salt == NULL.)
 *      ikm[ ]: [in]
 *          Input keying material.
 *      ikm_len: [in]
 *          The length of the input keying material.
 *      prk[ ]: [out]
 *          Array where the HKDF extraction is to be stored.
 *          Must be larger than USHAHashSize(whichSha);
 *
 *  Returns:
 *      sha Error Code.
 *
 *
int hkdfExtract(SHAversion whichSha,
    const unsigned char *salt, int salt_len,
    const unsigned char *ikm, int ikm_len,
    uint8_t prk[USHAMaxHashSize])
{
  unsigned char nullSalt[USHAMaxHashSize];
  if (salt == 0) {
    salt = nullSalt;
    salt_len = USHAHashSize(whichSha);
    memset(nullSalt, '\0', salt_len);
  } else if (salt_len &lt; 0) {
    return shaBadParam;
  }
  return hmac(whichSha, ikm, ikm_len, salt, salt_len, prk);
}


*
 *  hkdfExpand
 *
 *  Description:
 *      This function will perform HKDF expansion.
 *
 *  Parameters:
 *      whichSha: [in]
 *          One of SHA1, SHA224, SHA256, SHA384, SHA512
 *      prk[ ]: [in]
 *          The pseudo-random key to be expanded; either obtained
 *          directly from a cryptographically strong, uniformly
 *          distributed pseudo-random number generator, or as the
 *          output from hkdfExtract().
 *      prk_len: [in]


 *          The length of the pseudo-random key in prk;
 *          should at least be equal to USHAHashSize(whichSHA).
 *      info[ ]: [in]
 *          The optional context and application specific information.
 *          If info == NULL or a zero-length string, it is ignored.
 *      info_len: [in]
 *          The length of the optional context and application specific
 *          information.  (Ignored if info == NULL.)
 *      okm[ ]: [out]
 *          Where the HKDF is to be stored.
 *      okm_len: [in]
 *          The length of the buffer to hold okm.
 *          okm_len must be &lt;= 255 * USHABlockSize(whichSha)
 *
 *  Returns:
 *      sha Error Code.
 *
 *

int hkdfExpand(SHAversion whichSha, const uint8_t prk[ ], int prk_len,
    const unsigned char *info, int info_len,
    uint8_t okm[ ], int okm_len)
{
  int hash_len, N;
  unsigned char T[USHAMaxHashSize];
  int Tlen, where, i;

  if (info == 0) {
    info = (const unsigned char *)"";
    info_len = 0;
  } else if (info_len &lt; 0) {
    return shaBadParam;
  }
  if (okm_len &lt;= 0) return shaBadParam;
  if (!okm) return shaBadParam;

  hash_len = USHAHashSize(whichSha);
  if (prk_len &lt; hash_len) return shaBadParam;
  N = okm_len / hash_len;
  if ((okm_len % hash_len) != 0) N++;
  if (N &gt; 255) return shaBadParam;

  Tlen = 0;
  where = 0;
  for (i = 1; i &lt;= N; i++) {
    HMACContext context;
    unsigned char c = i;
    int ret = hmacReset(&amp;context, whichSha, prk, prk_len) ||
              hmacInput(&amp;context, T, Tlen) ||




              hmacInput(&amp;context, info, info_len) ||
              hmacInput(&amp;context, &amp;c, 1) ||
              hmacResult(&amp;context, T);
    if (ret != shaSuccess) return ret;
    memcpy(okm + where, T,
           (i != N) ? hash_len : (okm_len - where));
    where += hash_len;
    Tlen = hash_len;
  }
  return shaSuccess;
}

*
 *  hkdfReset
 *
 *  Description:
 *      This function will initialize the hkdfContext in preparation
 *      for key derivation using the modular HKDF interface for
 *      arbitrary length inputs.
 *
 *  Parameters:
 *      context: [in/out]
 *          The context to reset.
 *      whichSha: [in]
 *          One of SHA1, SHA224, SHA256, SHA384, SHA512
 *      salt[ ]: [in]
 *          The optional salt value (a non-secret random value);
 *          if not provided (salt == NULL), it is set internally
 *          to a string of HashLen(whichSha) zeros.
 *      salt_len: [in]
 *          The length of the salt value.  (Ignored if salt == NULL.)
 *
 *  Returns:
 *      sha Error Code.
 *
 *
int hkdfReset(HKDFContext *context, enum SHAversion whichSha,
              const unsigned char *salt, int salt_len)
{
  unsigned char nullSalt[USHAMaxHashSize];
  if (!context) return shaNull;

  context-&gt;whichSha = whichSha;
  context-&gt;hashSize = USHAHashSize(whichSha);
  if (salt == 0) {
    salt = nullSalt;
    salt_len = context-&gt;hashSize;
    memset(nullSalt, '\0', salt_len);



  }

  return hmacReset(&amp;context-&gt;hmacContext, whichSha, salt, salt_len);
}

*
 *  hkdfInput
 *
 *  Description:
 *      This function accepts an array of octets as the next portion
 *      of the input keying material.  It may be called multiple times.
 *
 *  Parameters:
 *      context: [in/out]
 *          The HKDF context to update.
 *      ikm[ ]: [in]
 *          An array of octets representing the next portion of
 *          the input keying material.
 *      ikm_len: [in]
 *          The length of ikm.
 *
 *  Returns:
 *      sha Error Code.
 *
 *
int hkdfInput(HKDFContext *context, const unsigned char *ikm,
              int ikm_len)
{
  if (!context) return shaNull;
  if (context-&gt;Corrupted) return context-&gt;Corrupted;
  if (context-&gt;Computed) return context-&gt;Corrupted = shaStateError;
  return hmacInput(&amp;context-&gt;hmacContext, ikm, ikm_len);
}

*
 * hkdfFinalBits
 *
 * Description:
 *   This function will add in any final bits of the
 *   input keying material.
 *
 * Parameters:
 *   context: [in/out]
 *     The HKDF context to update
 *   ikm_bits: [in]
 *     The final bits of the input keying material, in the upper
 *     portion of the byte.  (Use 0b###00000 instead of 0b00000###
 *     to input the three bits ###.)



 *   ikm_bit_count: [in]
 *     The number of bits in message_bits, between 1 and 7.
 *
 * Returns:
 *   sha Error Code.
 *
int hkdfFinalBits(HKDFContext *context, uint8_t ikm_bits,
                  unsigned int ikm_bit_count)
{
  if (!context) return shaNull;
  if (context-&gt;Corrupted) return context-&gt;Corrupted;
  if (context-&gt;Computed) return context-&gt;Corrupted = shaStateError;
  return hmacFinalBits(&amp;context-&gt;hmacContext, ikm_bits, ikm_bit_count);
}

*
 * hkdfResult
 *
 * Description:
 *   This function will finish the HKDF extraction and perform the
 *   final HKDF expansion.
 *
 * Parameters:
 *   context: [in/out]
 *     The HKDF context to use to calculate the HKDF hash.
 *   prk[ ]: [out]
 *     An optional location to store the HKDF extraction.
 *     Either NULL, or pointer to a buffer that must be
 *     larger than USHAHashSize(whichSha);
 *   info[ ]: [in]
 *     The optional context and application specific information.
 *     If info == NULL or a zero-length string, it is ignored.
 *   info_len: [in]
 *     The length of the optional context and application specific
 *     information.  (Ignored if info == NULL.)
 *   okm[ ]: [out]
 *     Where the HKDF is to be stored.
 *   okm_len: [in]
 *     The length of the buffer to hold okm.
 *     okm_len must be &lt;= 255 * USHABlockSize(whichSha)
 *
 * Returns:
 *   sha Error Code.
 *
 *
int hkdfResult(HKDFContext *context,
               uint8_t prk[USHAMaxHashSize],
               const unsigned char *info, int info_len,



               uint8_t okm[ ], int okm_len)
{
  uint8_t prkbuf[USHAMaxHashSize];
  int ret;

  if (!context) return shaNull;
  if (context-&gt;Corrupted) return context-&gt;Corrupted;
  if (context-&gt;Computed) return context-&gt;Corrupted = shaStateError;
  if (!okm) return context-&gt;Corrupted = shaBadParam;
  if (!prk) prk = prkbuf;

  ret = hmacResult(&amp;context-&gt;hmacContext, prk) ||
        hkdfExpand(context-&gt;whichSha, prk, context-&gt;hashSize, info,
                   info_len, okm, okm_len);
  context-&gt;Computed = 1;
  return context-&gt;Corrupted = ret;
}




The Test Driver

   The following code is a main program test driver to exercise the code
   in sha1.c, sha224-256.c, sha384-512.c, hmac.c, and hkdf.c.  The test
   driver can also be used as a standalone program for generating the
   hashes.  Note that the tests assume that character values are as in
   [<a href="https://datatracker.ietf.org/doc/html/rfc6234#ref-US-ASCII" title="&quot;USA Standard Code for Information Interchange&quot;">US-ASCII</a>] and a run time check warns if the code appears to have
   been compiled with some other character system.

   See also [<a href="https://datatracker.ietf.org/doc/html/rfc6234#ref-SHAVS" title="&quot;The Secure Hash Algorithm Validation System (SHAVS)&quot;">SHAVS</a>].

***************** shatest.c ****
*
 *  Description:
 *    This file will exercise the SHA code performing
 *      the three tests documented in FIPS PUB 180-3
 *        (<a href="http://csrc.nist.gov/publications/fips/">http://csrc.nist.gov/publications/fips/</a>
 *         fips180-2/fips180-2withchangenotice.pdf)
 *      one that calls SHAInput with an exact multiple of 512 bits
 *      the seven tests documented for each algorithm in
 *        "The Secure Hash Algorithm Validation System (SHAVS)"
 *        (<a href="http://csrc.nist.gov/cryptval/shs/SHAVS.pdf">http://csrc.nist.gov/cryptval/shs/SHAVS.pdf</a>),
 *        three of which are bit-level tests
 *

 *    These tests have subsequently been moved to pages linked from
 *      <a href="http://csrc.nist.gov/groups/ST/toolkit/examples.html">http://csrc.nist.gov/groups/ST/toolkit/examples.html</a>
 *
 *    This file will exercise the HMAC SHA1 code performing
 *      the seven tests documented in RFCs [<a href="https://datatracker.ietf.org/doc/html/rfc2202">RFC 2202</a>] and [<a href="https://datatracker.ietf.org/doc/html/rfc4231">RFC 4231</a>].
 *
 *    This file will exercise the HKDF code performing
 *      the seven tests documented in <a href="https://datatracker.ietf.org/doc/html/rfc4869">RFC 4869</a>.
 *
 *    To run the tests and just see PASSED/FAILED, use the -p option.
 *
 *    Other options exercise:
 *      hashing an arbitrary string
 *      hashing a file's contents
 *      a few error test checks
 *      printing the results in raw format
 *
 *  Portability Issues:
 *    None.
 *
 *

#include &lt;stdint.h&gt;
#include &lt;stdio.h&gt;
#include &lt;stdlib.h&gt;
#include &lt;string.h&gt;
#include &lt;ctype.h&gt;
#include &lt;unistd.h&gt;
// defines getopt() and optarg
#include "sha.h"

static int scasecmp(const char *s1, const char *s2);

*
 *  Define patterns for testing
 *
#define TEST1    "abc"
#define TEST2_1  \
        "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"
#define TEST2_2a \
        "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmn"
#define TEST2_2b \
        "hijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu"
#define TEST2_2  TEST2_2a TEST2_2b
#define TEST3    "a"    // times 1000000
#define TEST4a   "01234567012345670123456701234567"
#define TEST4b   "01234567012345670123456701234567"
    // an exact multiple of 512 bits
#define TEST4   TEST4a TEST4b      // times 10




#define TEST7_1 \
  "\x49\xb2\xae\xc2\x59\x4b\xbe\x3a\x3b\x11\x75\x42\xd9\x4a\xc8"
#define TEST8_1 \
  "\x9a\x7d\xfd\xf1\xec\xea\xd0\x6e\xd6\x46\xaa\x55\xfe\x75\x71\x46"
#define TEST9_1 \
  "\x65\xf9\x32\x99\x5b\xa4\xce\x2c\xb1\xb4\xa2\xe7\x1a\xe7\x02\x20" \
  "\xaa\xce\xc8\x96\x2d\xd4\x49\x9c\xbd\x7c\x88\x7a\x94\xea\xaa\x10" \
  "\x1e\xa5\xaa\xbc\x52\x9b\x4e\x7e\x43\x66\x5a\x5a\xf2\xcd\x03\xfe" \
  "\x67\x8e\xa6\xa5\x00\x5b\xba\x3b\x08\x22\x04\xc2\x8b\x91\x09\xf4" \
  "\x69\xda\xc9\x2a\xaa\xb3\xaa\x7c\x11\xa1\xb3\x2a"
#define TEST10_1 \
  "\xf7\x8f\x92\x14\x1b\xcd\x17\x0a\xe8\x9b\x4f\xba\x15\xa1\xd5\x9f" \
  "\x3f\xd8\x4d\x22\x3c\x92\x51\xbd\xac\xbb\xae\x61\xd0\x5e\xd1\x15" \
  "\xa0\x6a\x7c\xe1\x17\xb7\xbe\xea\xd2\x44\x21\xde\xd9\xc3\x25\x92" \
  "\xbd\x57\xed\xea\xe3\x9c\x39\xfa\x1f\xe8\x94\x6a\x84\xd0\xcf\x1f" \
  "\x7b\xee\xad\x17\x13\xe2\xe0\x95\x98\x97\x34\x7f\x67\xc8\x0b\x04" \
  "\x00\xc2\x09\x81\x5d\x6b\x10\xa6\x83\x83\x6f\xd5\x56\x2a\x56\xca" \
  "\xb1\xa2\x8e\x81\xb6\x57\x66\x54\x63\x1c\xf1\x65\x66\xb8\x6e\x3b" \
  "\x33\xa1\x08\xb0\x53\x07\xc0\x0a\xff\x14\xa7\x68\xed\x73\x50\x60" \
  "\x6a\x0f\x85\xe6\xa9\x1d\x39\x6f\x5b\x5c\xbe\x57\x7f\x9b\x38\x80" \
  "\x7c\x7d\x52\x3d\x6d\x79\x2f\x6e\xbc\x24\xa4\xec\xf2\xb3\xa4\x27" \
  "\xcd\xbb\xfb"
#define TEST7_224 \
  "\xf0\x70\x06\xf2\x5a\x0b\xea\x68\xcd\x76\xa2\x95\x87\xc2\x8d"
#define TEST8_224 \
  "\x18\x80\x40\x05\xdd\x4f\xbd\x15\x56\x29\x9d\x6f\x9d\x93\xdf\x62"
#define TEST9_224 \
  "\xa2\xbe\x6e\x46\x32\x81\x09\x02\x94\xd9\xce\x94\x82\x65\x69\x42" \
  "\x3a\x3a\x30\x5e\xd5\xe2\x11\x6c\xd4\xa4\xc9\x87\xfc\x06\x57\x00" \
  "\x64\x91\xb1\x49\xcc\xd4\xb5\x11\x30\xac\x62\xb1\x9d\xc2\x48\xc7" \
  "\x44\x54\x3d\x20\xcd\x39\x52\xdc\xed\x1f\x06\xcc\x3b\x18\xb9\x1f" \
  "\x3f\x55\x63\x3e\xcc\x30\x85\xf4\x90\x70\x60\xd2"
#define TEST10_224 \
  "\x55\xb2\x10\x07\x9c\x61\xb5\x3a\xdd\x52\x06\x22\xd1\xac\x97\xd5" \
  "\xcd\xbe\x8c\xb3\x3a\xa0\xae\x34\x45\x17\xbe\xe4\xd7\xba\x09\xab" \
  "\xc8\x53\x3c\x52\x50\x88\x7a\x43\xbe\xbb\xac\x90\x6c\x2e\x18\x37" \
  "\xf2\x6b\x36\xa5\x9a\xe3\xbe\x78\x14\xd5\x06\x89\x6b\x71\x8b\x2a" \
  "\x38\x3e\xcd\xac\x16\xb9\x61\x25\x55\x3f\x41\x6f\xf3\x2c\x66\x74" \
  "\xc7\x45\x99\xa9\x00\x53\x86\xd9\xce\x11\x12\x24\x5f\x48\xee\x47" \
  "\x0d\x39\x6c\x1e\xd6\x3b\x92\x67\x0c\xa5\x6e\xc8\x4d\xee\xa8\x14" \
  "\xb6\x13\x5e\xca\x54\x39\x2b\xde\xdb\x94\x89\xbc\x9b\x87\x5a\x8b" \
  "\xaf\x0d\xc1\xae\x78\x57\x36\x91\x4a\xb7\xda\xa2\x64\xbc\x07\x9d" \
  "\x26\x9f\x2c\x0d\x7e\xdd\xd8\x10\xa4\x26\x14\x5a\x07\x76\xf6\x7c" \
  "\x87\x82\x73"
#define TEST7_256 \
  "\xbe\x27\x46\xc6\xdb\x52\x76\x5f\xdb\x2f\x88\x70\x0f\x9a\x73"
#define TEST8_256 \
  "\xe3\xd7\x25\x70\xdc\xdd\x78\x7c\xe3\x88\x7a\xb2\xcd\x68\x46\x52"




#define TEST9_256 \
  "\x3e\x74\x03\x71\xc8\x10\xc2\xb9\x9f\xc0\x4e\x80\x49\x07\xef\x7c" \
  "\xf2\x6b\xe2\x8b\x57\xcb\x58\xa3\xe2\xf3\xc0\x07\x16\x6e\x49\xc1" \
  "\x2e\x9b\xa3\x4c\x01\x04\x06\x91\x29\xea\x76\x15\x64\x25\x45\x70" \
  "\x3a\x2b\xd9\x01\xe1\x6e\xb0\xe0\x5d\xeb\xa0\x14\xeb\xff\x64\x06" \
  "\xa0\x7d\x54\x36\x4e\xff\x74\x2d\xa7\x79\xb0\xb3"
#define TEST10_256 \
  "\x83\x26\x75\x4e\x22\x77\x37\x2f\x4f\xc1\x2b\x20\x52\x7a\xfe\xf0" \
  "\x4d\x8a\x05\x69\x71\xb1\x1a\xd5\x71\x23\xa7\xc1\x37\x76\x00\x00" \
  "\xd7\xbe\xf6\xf3\xc1\xf7\xa9\x08\x3a\xa3\x9d\x81\x0d\xb3\x10\x77" \
  "\x7d\xab\x8b\x1e\x7f\x02\xb8\x4a\x26\xc7\x73\x32\x5f\x8b\x23\x74" \
  "\xde\x7a\x4b\x5a\x58\xcb\x5c\x5c\xf3\x5b\xce\xe6\xfb\x94\x6e\x5b" \
  "\xd6\x94\xfa\x59\x3a\x8b\xeb\x3f\x9d\x65\x92\xec\xed\xaa\x66\xca" \
  "\x82\xa2\x9d\x0c\x51\xbc\xf9\x33\x62\x30\xe5\xd7\x84\xe4\xc0\xa4" \
  "\x3f\x8d\x79\xa3\x0a\x16\x5c\xba\xbe\x45\x2b\x77\x4b\x9c\x71\x09" \
  "\xa9\x7d\x13\x8f\x12\x92\x28\x96\x6f\x6c\x0a\xdc\x10\x6a\xad\x5a" \
  "\x9f\xdd\x30\x82\x57\x69\xb2\xc6\x71\xaf\x67\x59\xdf\x28\xeb\x39" \
  "\x3d\x54\xd6"
#define TEST7_384 \
  "\x8b\xc5\x00\xc7\x7c\xee\xd9\x87\x9d\xa9\x89\x10\x7c\xe0\xaa"
#define TEST8_384 \
  "\xa4\x1c\x49\x77\x79\xc0\x37\x5f\xf1\x0a\x7f\x4e\x08\x59\x17\x39"
#define TEST9_384 \
  "\x68\xf5\x01\x79\x2d\xea\x97\x96\x76\x70\x22\xd9\x3d\xa7\x16\x79" \
  "\x30\x99\x20\xfa\x10\x12\xae\xa3\x57\xb2\xb1\x33\x1d\x40\xa1\xd0" \
  "\x3c\x41\xc2\x40\xb3\xc9\xa7\x5b\x48\x92\xf4\xc0\x72\x4b\x68\xc8" \
  "\x75\x32\x1a\xb8\xcf\xe5\x02\x3b\xd3\x75\xbc\x0f\x94\xbd\x89\xfe" \
  "\x04\xf2\x97\x10\x5d\x7b\x82\xff\xc0\x02\x1a\xeb\x1c\xcb\x67\x4f" \
  "\x52\x44\xea\x34\x97\xde\x26\xa4\x19\x1c\x5f\x62\xe5\xe9\xa2\xd8" \
  "\x08\x2f\x05\x51\xf4\xa5\x30\x68\x26\xe9\x1c\xc0\x06\xce\x1b\xf6" \
  "\x0f\xf7\x19\xd4\x2f\xa5\x21\xc8\x71\xcd\x23\x94\xd9\x6e\xf4\x46" \
  "\x8f\x21\x96\x6b\x41\xf2\xba\x80\xc2\x6e\x83\xa9"
#define TEST10_384 \
  "\x39\x96\x69\xe2\x8f\x6b\x9c\x6d\xbc\xbb\x69\x12\xec\x10\xff\xcf" \
  "\x74\x79\x03\x49\xb7\xdc\x8f\xbe\x4a\x8e\x7b\x3b\x56\x21\xdb\x0f" \
  "\x3e\x7d\xc8\x7f\x82\x32\x64\xbb\xe4\x0d\x18\x11\xc9\xea\x20\x61" \
  "\xe1\xc8\x4a\xd1\x0a\x23\xfa\xc1\x72\x7e\x72\x02\xfc\x3f\x50\x42" \
  "\xe6\xbf\x58\xcb\xa8\xa2\x74\x6e\x1f\x64\xf9\xb9\xea\x35\x2c\x71" \
  "\x15\x07\x05\x3c\xf4\xe5\x33\x9d\x52\x86\x5f\x25\xcc\x22\xb5\xe8" \
  "\x77\x84\xa1\x2f\xc9\x61\xd6\x6c\xb6\xe8\x95\x73\x19\x9a\x2c\xe6" \
  "\x56\x5c\xbd\xf1\x3d\xca\x40\x38\x32\xcf\xcb\x0e\x8b\x72\x11\xe8" \
  "\x3a\xf3\x2a\x11\xac\x17\x92\x9f\xf1\xc0\x73\xa5\x1c\xc0\x27\xaa" \
  "\xed\xef\xf8\x5a\xad\x7c\x2b\x7c\x5a\x80\x3e\x24\x04\xd9\x6d\x2a" \
  "\x77\x35\x7b\xda\x1a\x6d\xae\xed\x17\x15\x1c\xb9\xbc\x51\x25\xa4" \
  "\x22\xe9\x41\xde\x0c\xa0\xfc\x50\x11\xc2\x3e\xcf\xfe\xfd\xd0\x96" \
  "\x76\x71\x1c\xf3\xdb\x0a\x34\x40\x72\x0e\x16\x15\xc1\xf2\x2f\xbc" \
  "\x3c\x72\x1d\xe5\x21\xe1\xb9\x9b\xa1\xbd\x55\x77\x40\x86\x42\x14" \
  "\x7e\xd0\x96"



#define TEST7_512 \
  "\x08\xec\xb5\x2e\xba\xe1\xf7\x42\x2d\xb6\x2b\xcd\x54\x26\x70"
#define TEST8_512 \
  "\x8d\x4e\x3c\x0e\x38\x89\x19\x14\x91\x81\x6e\x9d\x98\xbf\xf0\xa0"
#define TEST9_512 \
  "\x3a\xdd\xec\x85\x59\x32\x16\xd1\x61\x9a\xa0\x2d\x97\x56\x97\x0b" \
  "\xfc\x70\xac\xe2\x74\x4f\x7c\x6b\x27\x88\x15\x10\x28\xf7\xb6\xa2" \
  "\x55\x0f\xd7\x4a\x7e\x6e\x69\xc2\xc9\xb4\x5f\xc4\x54\x96\x6d\xc3" \
  "\x1d\x2e\x10\xda\x1f\x95\xce\x02\xbe\xb4\xbf\x87\x65\x57\x4c\xbd" \
  "\x6e\x83\x37\xef\x42\x0a\xdc\x98\xc1\x5c\xb6\xd5\xe4\xa0\x24\x1b" \
  "\xa0\x04\x6d\x25\x0e\x51\x02\x31\xca\xc2\x04\x6c\x99\x16\x06\xab" \
  "\x4e\xe4\x14\x5b\xee\x2f\xf4\xbb\x12\x3a\xab\x49\x8d\x9d\x44\x79" \
  "\x4f\x99\xcc\xad\x89\xa9\xa1\x62\x12\x59\xed\xa7\x0a\x5b\x6d\xd4" \
  "\xbd\xd8\x77\x78\xc9\x04\x3b\x93\x84\xf5\x49\x06"
#define TEST10_512 \
  "\xa5\x5f\x20\xc4\x11\xaa\xd1\x32\x80\x7a\x50\x2d\x65\x82\x4e\x31" \
  "\xa2\x30\x54\x32\xaa\x3d\x06\xd3\xe2\x82\xa8\xd8\x4e\x0d\xe1\xde" \
  "\x69\x74\xbf\x49\x54\x69\xfc\x7f\x33\x8f\x80\x54\xd5\x8c\x26\xc4" \
  "\x93\x60\xc3\xe8\x7a\xf5\x65\x23\xac\xf6\xd8\x9d\x03\xe5\x6f\xf2" \
  "\xf8\x68\x00\x2b\xc3\xe4\x31\xed\xc4\x4d\xf2\xf0\x22\x3d\x4b\xb3" \
  "\xb2\x43\x58\x6e\x1a\x7d\x92\x49\x36\x69\x4f\xcb\xba\xf8\x8d\x95" \
  "\x19\xe4\xeb\x50\xa6\x44\xf8\xe4\xf9\x5e\xb0\xea\x95\xbc\x44\x65" \
  "\xc8\x82\x1a\xac\xd2\xfe\x15\xab\x49\x81\x16\x4b\xbb\x6d\xc3\x2f" \
  "\x96\x90\x87\xa1\x45\xb0\xd9\xcc\x9c\x67\xc2\x2b\x76\x32\x99\x41" \
  "\x9c\xc4\x12\x8b\xe9\xa0\x77\xb3\xac\xe6\x34\x06\x4e\x6d\x99\x28" \
  "\x35\x13\xdc\x06\xe7\x51\x5d\x0d\x73\x13\x2e\x9a\x0d\xc6\xd3\xb1" \
  "\xf8\xb2\x46\xf1\xa9\x8a\x3f\xc7\x29\x41\xb1\xe3\xbb\x20\x98\xe8" \
  "\xbf\x16\xf2\x68\xd6\x4f\x0b\x0f\x47\x07\xfe\x1e\xa1\xa1\x79\x1b" \
  "\xa2\xf3\xc0\xc7\x58\xe5\xf5\x51\x86\x3a\x96\xc9\x49\xad\x47\xd7" \
  "\xfb\x40\xd2"
#define SHA1_SEED "\xd0\x56\x9c\xb3\x66\x5a\x8a\x43\xeb\x6e\xa2\x3d" \
  "\x75\xa3\xc4\xd2\x05\x4a\x0d\x7d"
#define SHA224_SEED "\xd0\x56\x9c\xb3\x66\x5a\x8a\x43\xeb\x6e\xa2" \
  "\x3d\x75\xa3\xc4\xd2\x05\x4a\x0d\x7d\x66\xa9\xca\x99\xc9\xce\xb0" \
  "\x27"
#define SHA256_SEED "\xf4\x1e\xce\x26\x13\xe4\x57\x39\x15\x69\x6b" \
  "\x5a\xdc\xd5\x1c\xa3\x28\xbe\x3b\xf5\x66\xa9\xca\x99\xc9\xce\xb0" \
  "\x27\x9c\x1c\xb0\xa7"
#define SHA384_SEED "\x82\x40\xbc\x51\xe4\xec\x7e\xf7\x6d\x18\xe3" \
  "\x52\x04\xa1\x9f\x51\xa5\x21\x3a\x73\xa8\x1d\x6f\x94\x46\x80\xd3" \
  "\x07\x59\x48\xb7\xe4\x63\x80\x4e\xa3\xd2\x6e\x13\xea\x82\x0d\x65" \
  "\xa4\x84\xbe\x74\x53"
#define SHA512_SEED "\x47\x3f\xf1\xb9\xb3\xff\xdf\xa1\x26\x69\x9a" \
  "\xc7\xef\x9e\x8e\x78\x77\x73\x09\x58\x24\xc6\x42\x55\x7c\x13\x99" \
  "\xd9\x8e\x42\x20\x44\x8d\xc3\x5b\x99\xbf\xdd\x44\x77\x95\x43\x92" \
  "\x4c\x1c\xe9\x3b\xc5\x94\x15\x38\x89\x5d\xb9\x88\x26\x1b\x00\x77" \
  "\x4b\x12\x27\x20\x39"




#define TESTCOUNT 10
#define HASHCOUNT 5
#define RANDOMCOUNT 4
#define HMACTESTCOUNT 7
#define HKDFTESTCOUNT 7

#define PRINTNONE 0
#define PRINTTEXT 1
#define PRINTRAW 2
#define PRINTHEX 3
#define PRINTBASE64 4

#define PRINTPASSFAIL 1
#define PRINTFAIL 2

#define length(x) (sizeof(x)-1)

// Test arrays for hashes.
struct hash {
    const char *name;
    SHAversion whichSha;
    int hashsize;
    struct {
        const char *testarray;
        int length;
        long repeatcount;
        int extrabits;
        int numberExtrabits;
        const char *resultarray;
    } tests[TESTCOUNT];
    const char *randomtest;
    const char *randomresults[RANDOMCOUNT];
} hashes[HASHCOUNT] = {
  { "SHA1", SHA1, SHA1HashSize,
    {
       { TEST1, length(TEST1), 1, 0, 0,
        "A9993E364706816ABA3E25717850C26C9CD0D89D" },
       { TEST2_1, length(TEST2_1), 1, 0, 0,
        "84983E441C3BD26EBAAE4AA1F95129E5E54670F1" },
       { TEST3, length(TEST3), 1000000, 0, 0,
        "34AA973CD4C4DAA4F61EEB2BDBAD27316534016F" },
       { TEST4, length(TEST4), 10, 0, 0,
        "DEA356A2CDDD90C7A7ECEDC5EBB563934F460452" },
       { "", 0, 0, 0x98, 5,
        "29826B003B906E660EFF4027CE98AF3531AC75BA" },
       { "\x5e", 1, 1, 0, 0,
        "5E6F80A34A9798CAFC6A5DB96CC57BA4C4DB59C2" },
       { TEST7_1, length(TEST7_1), 1, 0x80, 3,



        "6239781E03729919C01955B3FFA8ACB60B988340" },
       { TEST8_1, length(TEST8_1), 1, 0, 0,
        "82ABFF6605DBE1C17DEF12A394FA22A82B544A35" },
       { TEST9_1, length(TEST9_1), 1, 0xE0, 3,
        "8C5B2A5DDAE5A97FC7F9D85661C672ADBF7933D4" },
       { TEST10_1, length(TEST10_1), 1, 0, 0,
        "CB0082C8F197D260991BA6A460E76E202BAD27B3" }
    }, SHA1_SEED, { "E216836819477C7F78E0D843FE4FF1B6D6C14CD4",
        "A2DBC7A5B1C6C0A8BCB7AAA41252A6A7D0690DBC",
        "DB1F9050BB863DFEF4CE37186044E2EEB17EE013",
        "127FDEDF43D372A51D5747C48FBFFE38EF6CDF7B"
     } },
  { "SHA224", SHA224, SHA224HashSize,
    {
       { TEST1, length(TEST1), 1, 0, 0,
        "23097D223405D8228642A477BDA255B32AADBCE4BDA0B3F7E36C9DA7" },
       { TEST2_1, length(TEST2_1), 1, 0, 0,
        "75388B16512776CC5DBA5DA1FD890150B0C6455CB4F58B1952522525" },
       { TEST3, length(TEST3), 1000000, 0, 0,
        "20794655980C91D8BBB4C1EA97618A4BF03F42581948B2EE4EE7AD67" },
       { TEST4, length(TEST4), 10, 0, 0,
        "567F69F168CD7844E65259CE658FE7AADFA25216E68ECA0EB7AB8262" },
       { "", 0, 0, 0x68, 5,
        "E3B048552C3C387BCAB37F6EB06BB79B96A4AEE5FF27F51531A9551C" },
       { "\x07", 1, 1, 0, 0,
        "00ECD5F138422B8AD74C9799FD826C531BAD2FCABC7450BEE2AA8C2A" },
       { TEST7_224, length(TEST7_224), 1, 0xA0, 3,
        "1B01DB6CB4A9E43DED1516BEB3DB0B87B6D1EA43187462C608137150" },
       { TEST8_224, length(TEST8_224), 1, 0, 0,
        "DF90D78AA78821C99B40BA4C966921ACCD8FFB1E98AC388E56191DB1" },
       { TEST9_224, length(TEST9_224), 1, 0xE0, 3,
        "54BEA6EAB8195A2EB0A7906A4B4A876666300EEFBD1F3B8474F9CD57" },
       { TEST10_224, length(TEST10_224), 1, 0, 0,
        "0B31894EC8937AD9B91BDFBCBA294D9ADEFAA18E09305E9F20D5C3A4" }
    }, SHA224_SEED, { "100966A5B4FDE0B42E2A6C5953D4D7F41BA7CF79FD"
        "2DF431416734BE", "1DCA396B0C417715DEFAAE9641E10A2E99D55A"
        "BCB8A00061EB3BE8BD", "1864E627BDB2319973CD5ED7D68DA71D8B"
        "F0F983D8D9AB32C34ADB34", "A2406481FC1BCAF24DD08E6752E844"
        "709563FB916227FED598EB621F"
     } },
  { "SHA256", SHA256, SHA256HashSize,
  {
       { TEST1, length(TEST1), 1, 0, 0, "BA7816BF8F01CFEA4141"
        "40DE5DAE2223B00361A396177A9CB410FF61F20015AD" },
       { TEST2_1, length(TEST2_1), 1, 0, 0, "248D6A61D20638B8"
        "E5C026930C3E6039A33CE45964FF2167F6ECEDD419DB06C1" },
       { TEST3, length(TEST3), 1000000, 0, 0, "CDC76E5C9914FB92"
        "81A1C7E284D73E67F1809A48A497200E046D39CCC7112CD0" },

       { TEST4, length(TEST4), 10, 0, 0, "594847328451BDFA"
        "85056225462CC1D867D877FB388DF0CE35F25AB5562BFBB5" },
       { "", 0, 0, 0x68, 5, "D6D3E02A31A84A8CAA9718ED6C2057BE"
        "09DB45E7823EB5079CE7A573A3760F95" },
       { "\x19", 1, 1, 0, 0, "68AA2E2EE5DFF96E3355E6C7EE373E3D"
        "6A4E17F75F9518D843709C0C9BC3E3D4" },
       { TEST7_256, length(TEST7_256), 1, 0x60, 3, "77EC1DC8"
        "9C821FF2A1279089FA091B35B8CD960BCAF7DE01C6A7680756BEB972" },
       { TEST8_256, length(TEST8_256), 1, 0, 0, "175EE69B02BA"
        "9B58E2B0A5FD13819CEA573F3940A94F825128CF4209BEABB4E8" },
       { TEST9_256, length(TEST9_256), 1, 0xA0, 3, "3E9AD646"
        "8BBBAD2AC3C2CDC292E018BA5FD70B960CF1679777FCE708FDB066E9" },
       { TEST10_256, length(TEST10_256), 1, 0, 0, "97DBCA7D"
        "F46D62C8A422C941DD7E835B8AD3361763F7E9B2D95F4F0DA6E1CCBC" },
    }, SHA256_SEED, { "83D28614D49C3ADC1D6FC05DB5F48037C056F8D2A4CE44"
        "EC6457DEA5DD797CD1", "99DBE3127EF2E93DD9322D6A07909EB33B6399"
        "5E529B3F954B8581621BB74D39", "8D4BE295BB64661CA3C7EFD129A2F7"
        "25B33072DBDDE32385B9A87B9AF88EA76F", "40AF5D3F9716B040DF9408"
        "E31536B70FF906EC51B00447CA97D7DD97C12411F4"
    } },
  { "SHA384", SHA384, SHA384HashSize,
    {
      / { TEST1, length(TEST1), 1, 0, 0,
        "CB00753F45A35E8BB5A03D699AC65007272C32AB0EDED163"
        "1A8B605A43FF5BED8086072BA1E7CC2358BAECA134C825A7" },
       { TEST2_2, length(TEST2_2), 1, 0, 0,
        "09330C33F71147E83D192FC782CD1B4753111B173B3B05D2"
        "2FA08086E3B0F712FCC7C71A557E2DB966C3E9FA91746039" },
       { TEST3, length(TEST3), 1000000, 0, 0,
        "9D0E1809716474CB086E834E310A4A1CED149E9C00F24852"
        "7972CEC5704C2A5B07B8B3DC38ECC4EBAE97DDD87F3D8985" },
       { TEST4, length(TEST4), 10, 0, 0,
        "2FC64A4F500DDB6828F6A3430B8DD72A368EB7F3A8322A70"
        "BC84275B9C0B3AB00D27A5CC3C2D224AA6B61A0D79FB4596" },
       { "", 0, 0, 0x10, 5,
        "8D17BE79E32B6718E07D8A603EB84BA0478F7FCFD1BB9399"
        "5F7D1149E09143AC1FFCFC56820E469F3878D957A15A3FE4" },
       { "\xb9", 1, 1, 0, 0,
        "BC8089A19007C0B14195F4ECC74094FEC64F01F90929282C"
        "2FB392881578208AD466828B1C6C283D2722CF0AD1AB6938" },
       { TEST7_384, length(TEST7_384), 1, 0xA0, 3,
        "D8C43B38E12E7C42A7C9B810299FD6A770BEF30920F17532"
        "A898DE62C7A07E4293449C0B5FA70109F0783211CFC4BCE3" },
       { TEST8_384, length(TEST8_384), 1, 0, 0,
        "C9A68443A005812256B8EC76B00516F0DBB74FAB26D66591"
        "3F194B6FFB0E91EA9967566B58109CBC675CC208E4C823F7" },
       { TEST9_384, length(TEST9_384), 1, 0xE0, 3,
        "5860E8DE91C21578BB4174D227898A98E0B45C4C760F0095"


        "49495614DAEDC0775D92D11D9F8CE9B064EEAC8DAFC3A297" },
       { TEST10_384, length(TEST10_384), 1, 0, 0,
        "4F440DB1E6EDD2899FA335F09515AA025EE177A79F4B4AAF"
        "38E42B5C4DE660F5DE8FB2A5B2FBD2A3CBFFD20CFF1288C0" }
    }, SHA384_SEED, { "CE44D7D63AE0C91482998CF662A51EC80BF6FC68661A3C"
        "57F87566112BD635A743EA904DEB7D7A42AC808CABE697F38F", "F9C6D2"
        "61881FEE41ACD39E67AA8D0BAD507C7363EB67E2B81F45759F9C0FD7B503"
        "DF1A0B9E80BDE7BC333D75B804197D", "D96512D8C9F4A7A4967A366C01"
        "C6FD97384225B58343A88264847C18E4EF8AB7AEE4765FFBC3E30BD485D3"
        "638A01418F", "0CA76BD0813AF1509E170907A96005938BC985628290B2"
        "5FEF73CF6FAD68DDBA0AC8920C94E0541607B0915A7B4457F7"
    } },
  { "SHA512", SHA512, SHA512HashSize,
    {
       { TEST1, length(TEST1), 1, 0, 0,
        "DDAF35A193617ABACC417349AE20413112E6FA4E89A97EA2"
        "0A9EEEE64B55D39A2192992A274FC1A836BA3C23A3FEEBBD"
        "454D4423643CE80E2A9AC94FA54CA49F" },
       { TEST2_2, length(TEST2_2), 1, 0, 0,
        "8E959B75DAE313DA8CF4F72814FC143F8F7779C6EB9F7FA1"
        "7299AEADB6889018501D289E4900F7E4331B99DEC4B5433A"
        "C7D329EEB6DD26545E96E55B874BE909" },
       { TEST3, length(TEST3), 1000000, 0, 0,
        "E718483D0CE769644E2E42C7BC15B4638E1F98B13B204428"
        "5632A803AFA973EBDE0FF244877EA60A4CB0432CE577C31B"
        "EB009C5C2C49AA2E4EADB217AD8CC09B" },
       { TEST4, length(TEST4), 10, 0, 0,
        "89D05BA632C699C31231DED4FFC127D5A894DAD412C0E024"
        "DB872D1ABD2BA8141A0F85072A9BE1E2AA04CF33C765CB51"
        "0813A39CD5A84C4ACAA64D3F3FB7BAE9" },
       { "", 0, 0, 0xB0, 5,
        "D4EE29A9E90985446B913CF1D1376C836F4BE2C1CF3CADA0"
        "720A6BF4857D886A7ECB3C4E4C0FA8C7F95214E41DC1B0D2"
        "1B22A84CC03BF8CE4845F34DD5BDBAD4" },
       { "\xD0", 1, 1, 0, 0,
        "9992202938E882E73E20F6B69E68A0A7149090423D93C81B"
        "AB3F21678D4ACEEEE50E4E8CAFADA4C85A54EA8306826C4A"
        "D6E74CECE9631BFA8A549B4AB3FBBA15" },
       { TEST7_512, length(TEST7_512), 1, 0x80, 3,
        "ED8DC78E8B01B69750053DBB7A0A9EDA0FB9E9D292B1ED71"
        "5E80A7FE290A4E16664FD913E85854400C5AF05E6DAD316B"
        "7359B43E64F8BEC3C1F237119986BBB6" },
       { TEST8_512, length(TEST8_512), 1, 0, 0,
        "CB0B67A4B8712CD73C9AABC0B199E9269B20844AFB75ACBD"
        "D1C153C9828924C3DDEDAAFE669C5FDD0BC66F630F677398"
        "8213EB1B16F517AD0DE4B2F0C95C90F8" },
       { TEST9_512, length(TEST9_512), 1, 0x80, 3,
        "32BA76FC30EAA0208AEB50FFB5AF1864FDBF17902A4DC0A6"


        "82C61FCEA6D92B783267B21080301837F59DE79C6B337DB2"
        "526F8A0A510E5E53CAFED4355FE7C2F1" },
       { TEST10_512, length(TEST10_512), 1, 0, 0,
        "C665BEFB36DA189D78822D10528CBF3B12B3EEF726039909"
        "C1A16A270D48719377966B957A878E720584779A62825C18"
        "DA26415E49A7176A894E7510FD1451F5" }
    }, SHA512_SEED, { "2FBB1E7E00F746BA514FBC8C421F36792EC0E11FF5EFC3"
        "78E1AB0C079AA5F0F66A1E3EDBAEB4F9984BE14437123038A452004A5576"
        "8C1FD8EED49E4A21BEDCD0", "25CBE5A4F2C7B1D7EF07011705D50C62C5"
        "000594243EAFD1241FC9F3D22B58184AE2FEE38E171CF8129E29459C9BC2"
        "EF461AF5708887315F15419D8D17FE7949", "5B8B1F2687555CE2D7182B"
        "92E5C3F6C36547DA1C13DBB9EA4F73EA4CBBAF89411527906D35B1B06C1B"
        "6A8007D05EC66DF0A406066829EAB618BDE3976515AAFC", "46E36B007D"
        "19876CDB0B29AD074FE3C08CDD174D42169D6ABE5A1414B6E79707DF5877"
        "6A98091CF431854147BB6D3C66D43BFBC108FD715BDE6AA127C2B0E79F"
    }
  }
};





// Test arrays for HMAC.
struct hmachash {
    const char *keyarray[5];
    int keylength[5];
    const char *dataarray[5];
    int datalength[5];
    const char *resultarray[5];
    int resultlength[5];
} hmachashes[HMACTESTCOUNT] = {
  {  {
      "\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b"
      "\x0b\x0b\x0b\x0b\x0b"
    }, { 20 }, {
      "\x48\x69\x20\x54\x68\x65\x72\x65"
// "Hi There"
    }, { 8 }, {

      "B617318655057264E28BC0B6FB378C8EF146BE00",

      "896FB1128ABBDF196832107CD49DF33F47B4B1169912BA4F53684B22",

      "B0344C61D8DB38535CA8AFCEAF0BF12B881DC200C9833DA726E9376C2E32"
      "CFF7",

      "AFD03944D84895626B0825F4AB46907F15F9DADBE4101EC682AA034C7CEB"
      "C59CFAEA9EA9076EDE7F4AF152E8B2FA9CB6",

      "87AA7CDEA5EF619D4FF0B4241A1D6CB02379F4E2CE4EC2787AD0B30545E1"
      "7CDEDAA833B7D6B8A702038B274EAEA3F4E4BE9D914EEB61F1702E696C20"
      "3A126854"



    }, { SHA1HashSize, SHA224HashSize, SHA256HashSize,
      SHA384HashSize, SHA512HashSize }
  },
  {  {
      "\x4a\x65\x66\x65" // "Jefe"
    }, { 4 }, {
      "\x77\x68\x61\x74\x20\x64\x6f\x20\x79\x61\x20\x77\x61\x6e\x74"
      "\x20\x66\x6f\x72\x20\x6e\x6f\x74\x68\x69\x6e\x67\x3f"
      // "what do ya want for nothing?"
    }, { 28 }, {
      // HMAC-SHA-1
      "EFFCDF6AE5EB2FA2D27416D5F184DF9C259A7C79",
      // HMAC-SHA-224
      "A30E01098BC6DBBF45690F3A7E9E6D0F8BBEA2A39E6148008FD05E44",
      // HMAC-SHA-256
      "5BDCC146BF60754E6A042426089575C75A003F089D2739839DEC58B964EC"
      "3843",
      // HMAC-SHA-384
      "AF45D2E376484031617F78D2B58A6B1B9C7EF464F5A01B47E42EC3736322"
      "445E8E2240CA5E69E2C78B3239ECFAB21649",
      // HMAC-SHA-512
      "164B7A7BFCF819E2E395FBE73B56E0A387BD64222E831FD610270CD7EA25"
      "05549758BF75C05A994A6D034F65F8F0E6FDCAEAB1A34D4A6B4B636E070A"
      "38BCE737"
    }, { SHA1HashSize, SHA224HashSize, SHA256HashSize,
      SHA384HashSize, SHA512HashSize }
  },
  {
    {
      "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
      "\xaa\xaa\xaa\xaa\xaa"
    }, { 20 }, {
      "\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd"
      "\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd"
      "\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd"
      "\xdd\xdd\xdd\xdd\xdd"
    }, { 50 }, {
      // HMAC-SHA-1
      "125D7342B9AC11CD91A39AF48AA17B4F63F175D3",
      // HMAC-SHA-224
      "7FB3CB3588C6C1F6FFA9694D7D6AD2649365B0C1F65D69D1EC8333EA",
      // HMAC-SHA-256
      "773EA91E36800E46854DB8EBD09181A72959098B3EF8C122D9635514CED5"
      "65FE",
      // HMAC-SHA-384
      "88062608D3E6AD8A0AA2ACE014C8A86F0AA635D947AC9FEBE83EF4E55966"
      "144B2A5AB39DC13814B94E3AB6E101A34F27",
      // HMAC-SHA-512

      "FA73B0089D56A284EFB0F0756C890BE9B1B5DBDD8EE81A3655F83E33B227"
      "9D39BF3E848279A722C806B485A47E67C807B946A337BEE8942674278859"
      "E13292FB"
    }, { SHA1HashSize, SHA224HashSize, SHA256HashSize,
      SHA384HashSize, SHA512HashSize }
  },
  {  {
      "\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f"
      "\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19"
    }, { 25 }, {
      "\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd"
      "\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd"
      "\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd"
      "\xcd\xcd\xcd\xcd\xcd"
    }, { 50 }, {
      // HMAC-SHA-1
      "4C9007F4026250C6BC8414F9BF50C86C2D7235DA",
      // HMAC-SHA-224
      "6C11506874013CAC6A2ABC1BB382627CEC6A90D86EFC012DE7AFEC5A",
      // HMAC-SHA-256
      "82558A389A443C0EA4CC819899F2083A85F0FAA3E578F8077A2E3FF46729"
      "665B",
      // HMAC-SHA-384
      "3E8A69B7783C25851933AB6290AF6CA77A9981480850009CC5577C6E1F57"
      "3B4E6801DD23C4A7D679CCF8A386C674CFFB",
      // HMAC-SHA-512
      "B0BA465637458C6990E5A8C5F61D4AF7E576D97FF94B872DE76F8050361E"
      "E3DBA91CA5C11AA25EB4D679275CC5788063A5F19741120C4F2DE2ADEBEB"
      "10A298DD"
    }, { SHA1HashSize, SHA224HashSize, SHA256HashSize,
      SHA384HashSize, SHA512HashSize }
  },
  {  {
      "\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c"
      "\x0c\x0c\x0c\x0c\x0c"
    }, { 20 }, {
      "Test With Truncation"
    }, { 20 }, {
      // HMAC-SHA-1
      "4C1A03424B55E07FE7F27BE1",
      // HMAC-SHA-224
      "0E2AEA68A90C8D37C988BCDB9FCA6FA8",
      // HMAC-SHA-256
      "A3B6167473100EE06E0C796C2955552B",
      // HMAC-SHA-384
      "3ABF34C3503B2A23A46EFC619BAEF897",
      // HMAC-SHA-512
      "415FAD6271580A531D4179BC891D87A6"



    }, { 12, 16, 16, 16, 16 }
  },
  {  {
      "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
      "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
      "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
      "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
      "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
      "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
      "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
      "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
      "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
    }, { 80, 131 }, {
      "Test Using Larger Than Block-Size Key - Hash Key First"
    }, { 54 }, {
      // HMAC-SHA-1
      "AA4AE5E15272D00E95705637CE8A3B55ED402112",
      // HMAC-SHA-224
      "95E9A0DB962095ADAEBE9B2D6F0DBCE2D499F112F2D2B7273FA6870E",
      // HMAC-SHA-256
      "60E431591EE0B67F0D8A26AACBF5B77F8E0BC6213728C5140546040F0EE3"
      "7F54",
      // HMAC-SHA-384
      "4ECE084485813E9088D2C63A041BC5B44F9EF1012A2B588F3CD11F05033A"
      "C4C60C2EF6AB4030FE8296248DF163F44952",
      // HMAC-SHA-512
      "80B24263C7C1A3EBB71493C1DD7BE8B49B46D1F41B4AEEC1121B013783F8"
      "F3526B56D037E05F2598BD0FD2215D6A1E5295E64F73F63F0AEC8B915A98"
      "5D786598"
    }, { SHA1HashSize, SHA224HashSize, SHA256HashSize,
      SHA384HashSize, SHA512HashSize }
  },
  {  {
      "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
      "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
      "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
      "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
      "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
      "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
      "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
      "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
      "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
    }, { 80, 131 }, {
      "Test Using Larger Than Block-Size Key and "
      "Larger Than One Block-Size Data",
      "\x54\x68\x69\x73\x20\x69\x73\x20\x61\x20\x74\x65\x73\x74\x20"
      "\x75\x73\x69\x6e\x67\x20\x61\x20\x6c\x61\x72\x67\x65\x72\x20"
      "\x74\x68\x61\x6e\x20\x62\x6c\x6f\x63\x6b\x2d\x73\x69\x7a\x65"



<span class="grey">Eastlake &amp; Hansen             Informational                   [Page 103]</span></pre>
<hr class="noprint"><!--NewPage--><pre class="newpage"><span id="page-104"></span>
<span class="grey"><a href="https://datatracker.ietf.org/doc/html/rfc6234">RFC 6234</a>                SHAs, HMAC-SHAs, and HKDF               May 2011</span>


      "\x20\x6b\x65\x79\x20\x61\x6e\x64\x20\x61\x20\x6c\x61\x72\x67"
      "\x65\x72\x20\x74\x68\x61\x6e\x20\x62\x6c\x6f\x63\x6b\x2d\x73"
      "\x69\x7a\x65\x20\x64\x61\x74\x61\x2e\x20\x54\x68\x65\x20\x6b"
      "\x65\x79\x20\x6e\x65\x65\x64\x73\x20\x74\x6f\x20\x62\x65\x20"
      "\x68\x61\x73\x68\x65\x64\x20\x62\x65\x66\x6f\x72\x65\x20\x62"
      "\x65\x69\x6e\x67\x20\x75\x73\x65\x64\x20\x62\x79\x20\x74\x68"
      "\x65\x20\x48\x4d\x41\x43\x20\x61\x6c\x67\x6f\x72\x69\x74\x68"
      "\x6d\x2e"
      // "This is a test using a larger than"
      //" block-size key and a"
      //    "larger than block-size data.  The key needs to be hashed "
      //    "before being used by the HMAC algorithm."
    }, { 73, 152 }, {
      // HMAC-SHA-1
      "E8E99D0F45237D786D6BBAA7965C7808BBFF1A91",
      // HMAC-SHA-224
      "3A854166AC5D9F023F54D517D0B39DBD946770DB9C2B95C9F6F565D1",
      // HMAC-SHA-256
      "9B09FFA71B942FCB27635FBCD5B0E944BFDC63644F0713938A7F51535C3A"
      "35E2",
      // HMAC-SHA-384
      "6617178E941F020D351E2F254E8FD32C602420FEB0B8FB9ADCCEBB82461E"
      "99C5A678CC31E799176D3860E6110C46523E",
      // HMAC-SHA-512
      "E37B6A775DC87DBAA4DFA9F96E5E3FFDDEBD71F8867289865DF5A32D20CD"
      "C944B6022CAC3C4982B10D5EEB55C3E4DE15134676FB6DE0446065C97440"
      "FA8C6A58"
    }, { SHA1HashSize, SHA224HashSize, SHA256HashSize,
      SHA384HashSize, SHA512HashSize }
  }
};




// Test arrays for HKDF.
struct hkdfhash {
    SHAversion whichSha;
    int ikmlength;
    const char *ikmarray;
    int saltlength;
    const char *saltarray;
    int infolength;
    const char *infoarray;
    int prklength;
    const char *prkarray;
    int okmlength;
    const char *okmarray;
} hkdfhashes[HKDFTESTCOUNT] = {
    {
    // Test Case 1
        SHA256,
        22, "\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b"


            "\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b",
        13, "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c",
        10, "\xf0\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8\xf9",
        32, "077709362C2E32DF0DDC3F0DC47BBA6390B6C73BB50F9C3122EC844A"
            "D7C2B3E5",
        42, "3CB25F25FAACD57A90434F64D0362F2A2D2D0A90CF1A5A4C5DB02D56"
            "ECC4C5BF34007208D5B887185865"
    },
    {   // Test Case 2
        SHA256,
        80, "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d"
            "\x0e\x0f\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b"
            "\x1c\x1d\x1e\x1f\x20\x21\x22\x23\x24\x25\x26\x27\x28\x29"
            "\x2a\x2b\x2c\x2d\x2e\x2f\x30\x31\x32\x33\x34\x35\x36\x37"
            "\x38\x39\x3a\x3b\x3c\x3d\x3e\x3f\x40\x41\x42\x43\x44\x45"
            "\x46\x47\x48\x49\x4a\x4b\x4c\x4d\x4e\x4f",
        80, "\x60\x61\x62\x63\x64\x65\x66\x67\x68\x69\x6a\x6b\x6c\x6d"
            "\x6e\x6f\x70\x71\x72\x73\x74\x75\x76\x77\x78\x79\x7a\x7b"
            "\x7c\x7d\x7e\x7f\x80\x81\x82\x83\x84\x85\x86\x87\x88\x89"
            "\x8a\x8b\x8c\x8d\x8e\x8f\x90\x91\x92\x93\x94\x95\x96\x97"
            "\x98\x99\x9a\x9b\x9c\x9d\x9e\x9f\xa0\xa1\xa2\xa3\xa4\xa5"
            "\xa6\xa7\xa8\xa9\xaa\xab\xac\xad\xae\xaf",
        80, "\xb0\xb1\xb2\xb3\xb4\xb5\xb6\xb7\xb8\xb9\xba\xbb\xbc\xbd"
            "\xbe\xbf\xc0\xc1\xc2\xc3\xc4\xc5\xc6\xc7\xc8\xc9\xca\xcb"
            "\xcc\xcd\xce\xcf\xd0\xd1\xd2\xd3\xd4\xd5\xd6\xd7\xd8\xd9"
            "\xda\xdb\xdc\xdd\xde\xdf\xe0\xe1\xe2\xe3\xe4\xe5\xe6\xe7"
            "\xe8\xe9\xea\xeb\xec\xed\xee\xef\xf0\xf1\xf2\xf3\xf4\xf5"
            "\xf6\xf7\xf8\xf9\xfa\xfb\xfc\xfd\xfe\xff",
        32, "06A6B88C5853361A06104C9CEB35B45C"
            "EF760014904671014A193F40C15FC244",
        82, "B11E398DC80327A1C8E7F78C596A4934"
            "4F012EDA2D4EFAD8A050CC4C19AFA97C"
            "59045A99CAC7827271CB41C65E590E09"
            "DA3275600C2F09B8367793A9ACA3DB71"
            "CC30C58179EC3E87C14C01D5C1F3434F"
            "1D87"
    },
    {   // Test Case 3
        SHA256,
        22, "\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b"
            "\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b",
        0, "",
        0, "",
        32, "19EF24A32C717B167F33A91D6F648BDF"
            "96596776AFDB6377AC434C1C293CCB04",
        42, "8DA4E775A563C18F715F802A063C5A31"
            "B8A11F5C5EE1879EC3454E5F3C738D2D"
            "9D201395FAA4B61A96C8"


    },
    {   // Test Case 4
        SHA1,
        11, "\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b",
        13, "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c",
        10, "\xf0\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8\xf9",
        20, "9B6C18C432A7BF8F0E71C8EB88F4B30BAA2BA243",
        42, "085A01EA1B10F36933068B56EFA5AD81"
            "A4F14B822F5B091568A9CDD4F155FDA2"
            "C22E422478D305F3F896"
    },
    {   // Test Case 5
        SHA1,
        80, "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d"
            "\x0e\x0f\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b"
            "\x1c\x1d\x1e\x1f\x20\x21\x22\x23\x24\x25\x26\x27\x28\x29"
            "\x2a\x2b\x2c\x2d\x2e\x2f\x30\x31\x32\x33\x34\x35\x36\x37"
            "\x38\x39\x3a\x3b\x3c\x3d\x3e\x3f\x40\x41\x42\x43\x44\x45"
            "\x46\x47\x48\x49\x4a\x4b\x4c\x4d\x4e\x4f",
        80, "\x60\x61\x62\x63\x64\x65\x66\x67\x68\x69\x6A\x6B\x6C\x6D"
            "\x6E\x6F\x70\x71\x72\x73\x74\x75\x76\x77\x78\x79\x7A\x7B"
            "\x7C\x7D\x7E\x7F\x80\x81\x82\x83\x84\x85\x86\x87\x88\x89"
            "\x8A\x8B\x8C\x8D\x8E\x8F\x90\x91\x92\x93\x94\x95\x96\x97"
            "\x98\x99\x9A\x9B\x9C\x9D\x9E\x9F\xA0\xA1\xA2\xA3\xA4\xA5"
            "\xA6\xA7\xA8\xA9\xAA\xAB\xAC\xAD\xAE\xAF",
        80, "\xB0\xB1\xB2\xB3\xB4\xB5\xB6\xB7\xB8\xB9\xBA\xBB\xBC\xBD"
            "\xBE\xBF\xC0\xC1\xC2\xC3\xC4\xC5\xC6\xC7\xC8\xC9\xCA\xCB"
            "\xCC\xCD\xCE\xCF\xD0\xD1\xD2\xD3\xD4\xD5\xD6\xD7\xD8\xD9"
            "\xDA\xDB\xDC\xDD\xDE\xDF\xE0\xE1\xE2\xE3\xE4\xE5\xE6\xE7"
            "\xE8\xE9\xEA\xEB\xEC\xED\xEE\xEF\xF0\xF1\xF2\xF3\xF4\xF5"
            "\xF6\xF7\xF8\xF9\xFA\xFB\xFC\xFD\xFE\xFF",
        20, "8ADAE09A2A307059478D309B26C4115A224CFAF6",
        82, "0BD770A74D1160F7C9F12CD5912A06EB"
            "FF6ADCAE899D92191FE4305673BA2FFE"
            "8FA3F1A4E5AD79F3F334B3B202B2173C"
            "486EA37CE3D397ED034C7F9DFEB15C5E"
            "927336D0441F4C4300E2CFF0D0900B52"
            "D3B4"
    },
    {   // Test Case 6
        SHA1,
        22, "\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b"
            "\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b",
        0, "",
        0, "",
        20, "DA8C8A73C7FA77288EC6F5E7C297786AA0D32D01",
        42, "0AC1AF7002B3D761D1E55298DA9D0506"
            "B9AE52057220A306E07B6B87E8DF21D0"


            "EA00033DE03984D34918"
    },
    {   // Test Case 7.
        SHA1,
        22, "\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c"
            "\x0c\x0c\x0c\x0c\x0c\x0c\x0c\x0c",
        0, 0,
        0, "",
        20, "2ADCCADA18779E7C2077AD2EB19D3F3E731385DD",
        42, "2C91117204D745F3500D636A62F64F0A"
            "B3BAE548AA53D423B0D1F27EBBA6F5E5"
            "673A081D70CCE7ACFC48"
    }
};



*
 * Check the hash value against the expected string, expressed in hex
 *

static const char hexdigits[ ] = "0123456789ABCDEF";
int checkmatch(const unsigned char *hashvalue,
  const char *hexstr, int hashsize)
{
  int i;
  for (i = 0; i &lt; hashsize; ++i) {
    if (*hexstr++ != hexdigits[(hashvalue[i] &gt;&gt; 4) &amp; 0xF])
      return 0;
    if (*hexstr++ != hexdigits[hashvalue[i] &amp; 0xF]) return 0;
  }
  return 1;
}



*
 * Print the string, converting non-printable characters to "."
 *
void printstr(const char *str, int len)
{
  for ( ; len-- &gt; 0; str++)
    putchar(isprint((unsigned char)*str) ? *str : '.');
}

*
 * Print the string, converting all characters to hex "## ".
 *
void printxstr(const char *str, int len)
{
  char *sep = "";
  for ( ; len-- &gt; 0; str++) {
    printf("%s%c%c", sep, hexdigits[(*str &gt;&gt; 4) &amp; 0xF],



      hexdigits[*str &amp; 0xF]);
    sep = " ";
  }
}

*
 * Print a usage message.
 *
void usage(const char *argv0)
{
  fprintf(stderr,
    "Usage:\n"
    "Common options: [-h hash] [-w|-x|-6] [-H]\n"
    "Hash a string:\n"
      "\t%s [-S expectedresult] -s hashstr [-k key] "
           "[-i info -L okm-len]\n"
    "Hash a file:\n"
      "\t%s [-S expectedresult] -f file [-k key] "
           "[-i info -L okm-len]\n"
    "Hash a file, ignoring whitespace:\n"
      "\t%s [-S expectedresult] -F file [-k key] "
           "[-i info -L okm-len]\n"
    "Additional bits to add in: [-B bitcount -b bits]\n"
    "(If -k,-i&amp;-L are used, run HKDF-SHA###.\n"
    " If -k is used, but not -i&amp;-L, run HMAC-SHA###.\n"
    " Otherwise, run SHA###.)\n"
    "Standard tests:\n"
      "\t%s [-m | -d] [-l loopcount] [-t test#] [-e]\n"
      "\t\t[-r randomseed] [-R randomloop-count] "
        "[-p] [-P|-X]\n"
    "-h\thash to test: "
      "0|SHA1, 1|SHA224, 2|SHA256, 3|SHA384, 4|SHA512\n"
    "-m\tperform hmac standard tests\n"
    "-k\tkey for hmac test\n"
    "-d\tperform hkdf standard tests\n"
    "-t\ttest case to run, 1-10\n"
    "-l\thow many times to run the test\n"
    "-e\ttest error returns\n"
    "-p\tdo not print results\n"
    "-P\tdo not print PASSED/FAILED\n"
    "-X\tprint FAILED, but not PASSED\n"
    "-r\tseed for random test\n"
    "-R\thow many times to run random test\n"
    "-s\tstring to hash\n"
    "-S\texpected result of hashed string, in hex\n"
    "-w\toutput hash in raw format\n"
    "-x\toutput hash in hex format\n"
    "-6\toutput hash in base64 format\n"

    "-B\t# extra bits to add in after string or file input\n"
    "-b\textra bits to add (high order bits of #, 0# or 0x#)\n"
    "-H\tinput hashstr or randomseed is in hex\n"
    , argv0, argv0, argv0, argv0);
  exit(1);
}

*
 * Print the results and PASS/FAIL.
 *
void printResult(uint8_t *Message_Digest, int hashsize,
    const char *hashname, const char *testtype, const char *testname,
    const char *resultarray, int printResults, int printPassFail)
{
  int i, k;
  if (printResults == PRINTTEXT) {
    printf("\nhashsize=%d\n", hashsize);
    putchar('\t');
    for (i = 0; i &lt; hashsize; ++i) {
      putchar(hexdigits[(Message_Digest[i] &gt;&gt; 4) &amp; 0xF]);
      putchar(hexdigits[Message_Digest[i] &amp; 0xF]);
      putchar(' ');
    }
    putchar('\n');
  } else if (printResults == PRINTRAW) {
    fwrite(Message_Digest, 1, hashsize, stdout);
  } else if (printResults == PRINTHEX) {
    for (i = 0; i &lt; hashsize; ++i) {
      putchar(hexdigits[(Message_Digest[i] &gt;&gt; 4) &amp; 0xF]);
      putchar(hexdigits[Message_Digest[i] &amp; 0xF]);
    }
    putchar('\n');
  } else if (printResults == PRINTBASE64) {
    unsigned char b;
    char *sm = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
               "0123456789+/";
    for (i = 0; i &lt; hashsize; i += 3) {
      putchar(sm[Message_Digest[i] &gt;&gt; 2]);
      b = (Message_Digest[i] &amp; 0x03) &lt;&lt; 4;
      if (i+1 &lt; hashsize) b |= Message_Digest[i+1] &gt;&gt; 4;
      putchar(sm[b]);
      if (i+1 &lt; hashsize) {
        b = (Message_Digest[i+1] &amp; 0x0f) &lt;&lt; 2;
        if (i+2 &lt; hashsize) b |= Message_Digest[i+2] &gt;&gt; 6;
        putchar(sm[b]);
      } else putchar('=');
      if (i+2 &lt; hashsize) putchar(sm[Message_Digest[i+2] &amp; 0x3f]);
      else putchar('=');


    }
    putchar('\n');
  }

  if (printResults &amp;&amp; resultarray) {
    printf("    Should match:\n\t");
    for (i = 0, k = 0; i &lt; hashsize; i++, k += 2) {
      putchar(resultarray[k]);
      putchar(resultarray[k+1]);
      putchar(' ');
    }
    putchar('\n');
  }

  if (printPassFail &amp;&amp; resultarray) {
    int ret = checkmatch(Message_Digest, resultarray, hashsize);
    if ((printPassFail == PRINTPASSFAIL) || !ret)
      printf("%s %s %s: %s\n", hashname, testtype, testname,
        ret ? "PASSED" : "FAILED");
  }
}



*
 * Exercise a hash series of functions.  The input is the testarray,
 * repeated repeatcount times, followed by the extrabits.  If the
 * result is known, it is in resultarray in uppercase hex.
 *
int hash(int testno, int loopno, int hashno,
  const char *testarray, int length, long repeatcount,
  int numberExtrabits, int extrabits, const unsigned char *keyarray,
  int keylen, const unsigned char *info, int infolen, int okmlen,
  const char *resultarray, int hashsize, int printResults,
  int printPassFail)
{
  USHAContext sha;
  HMACContext hmac;
  HKDFContext hkdf;
  int err, i;
  uint8_t Message_Digest_Buf[USHAMaxHashSize];
  uint8_t *Message_Digest = Message_Digest_Buf;
  char buf[20];

  if (printResults == PRINTTEXT) {
    printf("\nTest %d: Iteration %d, Repeat %ld\n\t'", testno+1,
      loopno, repeatcount);
    printstr(testarray, length);
    printf("'\n\t'");
    printxstr(testarray, length);


    printf("'\n");
    printf("    Length=%d bytes (%d bits), ", length, length * 8);
    printf("ExtraBits %d: %2.2x\n", numberExtrabits, extrabits);
  }

  if (info) Message_Digest = malloc(okmlen);
  memset(&amp;sha, '\343', sizeof(sha));
                  // force bad data into struct
  memset(&amp;hmac, '\343', sizeof(hmac));
  memset(&amp;hkdf, '\343', sizeof(hkdf));

  err = info ? hkdfReset(&amp;hkdf, hashes[hashno].whichSha,
                             keyarray, keylen) :
        keyarray ? hmacReset(&amp;hmac, hashes[hashno].whichSha,
                             keyarray, keylen) :
                   USHAReset(&amp;sha, hashes[hashno].whichSha);
  if (err != shaSuccess) {
    fprintf(stderr, "hash(): %sReset Error %d.\n",
            info ? "hkdf" : keyarray ? "hmac" : "sha", err);
    return err;
  }

  for (i = 0; i &lt; repeatcount; ++i) {
    err = info ? hkdfInput(&amp;hkdf, (const uint8_t *)testarray, length) :
          keyarray ? hmacInput(&amp;hmac, (const uint8_t *) testarray,
                               length) :
                     USHAInput(&amp;sha, (const uint8_t *) testarray,
                               length);
    if (err != shaSuccess) {
      fprintf(stderr, "hash(): %sInput Error %d.\n",
              info ? "hkdf" : keyarray ? "hmac" : "sha", err);
      return err;
    }
  }

  if (numberExtrabits &gt; 0) {
    err = info ? hkdfFinalBits(&amp;hkdf, extrabits, numberExtrabits) :
          keyarray ? hmacFinalBits(&amp;hmac, (uint8_t) extrabits,
                                   numberExtrabits) :
                     USHAFinalBits(&amp;sha, (uint8_t) extrabits,
                                   numberExtrabits);
    if (err != shaSuccess) {
      fprintf(stderr, "hash(): %sFinalBits Error %d.\n",
              info ? "hkdf" : keyarray ? "hmac" : "sha", err);
      return err;
    }
  }

  err = info ? hkdfResult(&amp;hkdf, 0, info, infolen,


                          Message_Digest, okmlen) :
        keyarray ? hmacResult(&amp;hmac, Message_Digest) :
                   USHAResult(&amp;sha, Message_Digest);
  if (err != shaSuccess) {
    fprintf(stderr, "hash(): %s Result Error %d, could not compute "
      "message digest.\n",
      info ? "hkdf" : keyarray ? "hmac" : "sha", err);
    return err;
  }

  sprintf(buf, "%d", testno+1);
  printResult(Message_Digest, info ? okmlen : hashsize,
    hashes[hashno].name, info ? "hkdf standard test" :
    keyarray ? "hmac standard test" : "sha standard test", buf,
    resultarray, printResults, printPassFail);

  return err;
}



*
 * Exercise an HKDF series.  The input is the testarray,
 * repeated repeatcount times, followed by the extrabits.  If the
 * result is known, it is in resultarray in uppercase hex.
 *
int hashHkdf(int testno, int loopno, int hashno,
  int printResults, int printPassFail)
{
  int err;
  unsigned char prk[USHAMaxHashSize+1];
  uint8_t okm[255 * USHAMaxHashSize+1];
  char buf[20];

  if (printResults == PRINTTEXT) {
    printf("\nTest %d: Iteration %d\n\tSALT\t'", testno+1, loopno);
    printxstr(hkdfhashes[testno].saltarray,
      hkdfhashes[testno].saltlength);
    printf("'\n\tIKM\t'");
    printxstr(hkdfhashes[testno].ikmarray,
      hkdfhashes[testno].ikmlength);
    printf("'\n\tINFO\t'");
    printxstr(hkdfhashes[testno].infoarray,
      hkdfhashes[testno].infolength);
    printf("'\n");
    printf("    L=%d bytes\n", hkdfhashes[testno].okmlength);
  }

  // Run hkdf() against the test vectors
  err = hkdf(hkdfhashes[testno].whichSha,



             (const uint8_t *) hkdfhashes[testno].saltarray,
             hkdfhashes[testno].saltlength,
             (const uint8_t *) hkdfhashes[testno].ikmarray,
             hkdfhashes[testno].ikmlength,
             (const uint8_t *) hkdfhashes[testno].infoarray,
             hkdfhashes[testno].infolength, okm,
             hkdfhashes[testno].okmlength);
  if (err != shaSuccess) {
    fprintf(stderr, "hashHkdf(): hkdf Error %d.\n", err);
    return err;
  }
  sprintf(buf, "hkdf %d", testno+1);
  printResult(okm, hkdfhashes[testno].okmlength,
    USHAHashName(hkdfhashes[testno].whichSha), "hkdf standard test",
    buf, hkdfhashes[testno].okmarray, printResults, printPassFail);

  * Now run hkdfExtract() by itself against the test vectors
  * to verify the intermediate results.
  err = hkdfExtract(hkdfhashes[testno].whichSha,
                    (const uint8_t *) hkdfhashes[testno].saltarray,
                    hkdfhashes[testno].saltlength,
                    (const uint8_t *) hkdfhashes[testno].ikmarray,
                    hkdfhashes[testno].ikmlength, prk);
  if (err != shaSuccess) {
    fprintf(stderr, "hashHkdf(): hkdfExtract Error %d.\n", err);
    return err;
  }
  sprintf(buf, "hkdfExtract %d", testno+1);
  printResult(prk, USHAHashSize(hkdfhashes[testno].whichSha),
    USHAHashName(hkdfhashes[testno].whichSha), "hkdf standard test",
    buf, hkdfhashes[testno].prkarray, printResults, printPassFail);

  * Now run hkdfExpand() by itself against
 the test vectors
  * using the intermediate results from
 hkdfExtract.
  err = hkdfExpand(hkdfhashes[testno].whichSha, prk,
    USHAHashSize(hkdfhashes[testno].whichSha),
    (const uint8_t *)hkdfhashes[testno].infoarray,
    hkdfhashes[testno].infolength, okm, hkdfhashes[testno].okmlength);
  if (err != shaSuccess) {
    fprintf(stderr, "hashHkdf(): hkdfExpand Error %d.\n", err);
    return err;
  }
  sprintf(buf, "hkdfExpand %d", testno+1);
  printResult(okm, hkdfhashes[testno].okmlength,
    USHAHashName(hkdfhashes[testno].whichSha), "hkdf standard test",
    buf, hkdfhashes[testno].okmarray, printResults, printPassFail);

  return err;


}

*
 * Exercise a hash series of functions.  The input is a filename.
 * If the result is known, it is in resultarray in uppercase hex.
 *
int hashfile(int hashno, const char *hashfilename, int bits,
  int bitcount, int skipSpaces, const unsigned char *keyarray,
  int keylen, const unsigned char *info, int infolen, int okmlen,
  const char *resultarray, int hashsize,
  int printResults, int printPassFail)
{
  USHAContext sha;
  HMACContext hmac;
  HKDFContext hkdf;
  int err, nread, c;
  unsigned char buf[4096];
  uint8_t Message_Digest_Buf[USHAMaxHashSize];
  uint8_t *Message_Digest = Message_Digest_Buf;
  unsigned char cc;
  FILE *hashfp = (strcmp(hashfilename, "-") == 0) ? stdin :
    fopen(hashfilename, "r");

  if (!hashfp) {
    fprintf(stderr, "cannot open file '%s'\n", hashfilename);
    return shaStateError;
  }

  if (info) Message_Digest = malloc(okmlen);
  memset(&amp;sha, '\343', sizeof(sha));
          // force bad data into struct
  memset(&amp;hmac, '\343', sizeof(hmac));
  memset(&amp;hkdf, '\343', sizeof(hkdf));
  err = info ? hkdfReset(&amp;hkdf, hashes[hashno].whichSha,
                             keyarray, keylen) :
        keyarray ? hmacReset(&amp;hmac, hashes[hashno].whichSha,
                             keyarray, keylen) :
                   USHAReset(&amp;sha, hashes[hashno].whichSha);
  if (err != shaSuccess) {
    fprintf(stderr, "hashfile(): %sReset Error %d.\n",
            info ? "hkdf" : keyarray ? "hmac" : "sha", err);
    return err;
  }

  if (skipSpaces)
    while ((c = getc(hashfp)) != EOF) {
      if (!isspace(c)) {
        cc = (unsigned char)c;
        err = info ? hkdfInput(&amp;hkdf, &amp;cc, 1) :



              keyarray ? hmacInput(&amp;hmac, &amp;cc, 1) :
                         USHAInput(&amp;sha, &amp;cc, 1);
        if (err != shaSuccess) {
          fprintf(stderr, "hashfile(): %sInput Error %d.\n",
                  info ? "hkdf" : keyarray ? "hmac" : "sha", err);
          if (hashfp != stdin) fclose(hashfp);
          return err;
        }
      }
    }
  else
    while ((nread = fread(buf, 1, sizeof(buf), hashfp)) &gt; 0) {
      err = info ? hkdfInput(&amp;hkdf, buf, nread) :
            keyarray ? hmacInput(&amp;hmac, buf, nread) :
                       USHAInput(&amp;sha, buf, nread);
      if (err != shaSuccess) {
        fprintf(stderr, "hashfile(): %s Error %d.\n",
                info ? "hkdf" : keyarray ? "hmacInput" :
                "shaInput", err);
        if (hashfp != stdin) fclose(hashfp);
        return err;
      }
    }

  if (bitcount &gt; 0)
    err = info ? hkdfFinalBits(&amp;hkdf, bits, bitcount) :
          keyarray ? hmacFinalBits(&amp;hmac, bits, bitcount) :
                   USHAFinalBits(&amp;sha, bits, bitcount);
  if (err != shaSuccess) {
    fprintf(stderr, "hashfile(): %s Error %d.\n",
            info ? "hkdf" : keyarray ? "hmacFinalBits" :
            "shaFinalBits", err);
    if (hashfp != stdin) fclose(hashfp);
    return err;
  }

  err = info ? hkdfResult(&amp;hkdf, 0, info, infolen,
                          Message_Digest, okmlen) :
        keyarray ? hmacResult(&amp;hmac, Message_Digest) :
                   USHAResult(&amp;sha, Message_Digest);
  if (err != shaSuccess) {
    fprintf(stderr, "hashfile(): %s Error %d.\n",
            info ? "hkdf" : keyarray ? "hmacResult" :
            "shaResult", err);
    if (hashfp != stdin) fclose(hashfp);
    return err;
  }



  printResult(Message_Digest, info ? okmlen : hashsize,
    hashes[hashno].name, "file", hashfilename, resultarray,
    printResults, printPassFail);

  if (hashfp != stdin) fclose(hashfp);
  if (info) free(Message_Digest);
  return err;
}

*
 * Exercise a hash series of functions through multiple permutations.
 * The input is an initial seed.  That seed is replicated 3 times.
 * For 1000 rounds, the previous three results are used as the input.
 * This result is then checked, and used to seed the next cycle.
 * If the result is known, it is in resultarrays in uppercase hex.
 *
void randomtest(int hashno, const char *seed, int hashsize,
    const char **resultarrays, int randomcount,
    int printResults, int printPassFail)
{
  int i, j; char buf[20];
  unsigned char SEED[USHAMaxHashSize], MD[1003][USHAMaxHashSize];

  * INPUT: Seed - A random seed n bits long *
  memcpy(SEED, seed, hashsize);
  if (printResults == PRINTTEXT) {
    printf("%s random test seed= '", hashes[hashno].name);
    printxstr(seed, hashsize);
    printf("'\n");
  }

  for (j = 0; j &lt; randomcount; j++) {
    // MD0 = MD1 = MD2 = Seed;
    memcpy(MD[0], SEED, hashsize);
    memcpy(MD[1], SEED, hashsize);
    memcpy(MD[2], SEED, hashsize);
    for (i=3; i&lt;1003; i++) {
      // Mi = MDi-3 || MDi-2 || MDi-1;
      USHAContext Mi;
      memset(&amp;Mi, '\343', sizeof(Mi));
              // force bad data into struct
      USHAReset(&amp;Mi, hashes[hashno].whichSha);
      USHAInput(&amp;Mi, MD[i-3], hashsize);
      USHAInput(&amp;Mi, MD[i-2], hashsize);
      USHAInput(&amp;Mi, MD[i-1], hashsize);
      // MDi = SHA(Mi);
      USHAResult(&amp;Mi, MD[i]);
    }




    // MDj = Seed = MDi;
    memcpy(SEED, MD[i-1], hashsize);

    // OUTPUT: MDj
    sprintf(buf, "%d", j);
    printResult(SEED, hashsize, hashes[hashno].name, "random test",
      buf, resultarrays ? resultarrays[j] : 0, printResults,
      (j &lt; RANDOMCOUNT) ? printPassFail : 0);
  }
}

*
 * Look up a hash name.
 *
int findhash(const char *argv0, const char *opt)
{
  int i;
  const char *names[HASHCOUNT][2] = {
    { "0", "sha1" }, { "1", "sha224" }, { "2", "sha256" },
    { "3", "sha384" }, { "4", "sha512" }
  };
  for (i = 0; i &lt; HASHCOUNT; i++)
    if ((strcmp(opt, names[i][0]) == 0) ||
        (scasecmp(opt, names[i][1]) == 0))
      return i;

  fprintf(stderr, "%s: Unknown hash name: '%s'\n", argv0, opt);
  usage(argv0);
  return 0;
}

*
 * Run some tests that should invoke errors.
 *
void testErrors(int hashnolow, int hashnohigh, int printResults,
    int printPassFail)
{
  USHAContext usha;
  uint8_t Message_Digest[USHAMaxHashSize];
  int hashno, err;

  for (hashno = hashnolow; hashno &lt;= hashnohigh; hashno++) {
    memset(&amp;usha, '\343', sizeof(usha));
                       // force bad data
    USHAReset(&amp;usha, hashno);
    USHAResult(&amp;usha, Message_Digest);
    err = USHAInput(&amp;usha, (const unsigned char *)"foo", 3);
    if (printResults == PRINTTEXT)
      printf ("\nError %d. Should be %d.\n", err, shaStateError);


    if ((printPassFail == PRINTPASSFAIL) ||
        ((printPassFail == PRINTFAIL) &amp;&amp; (err != shaStateError)))
      printf("%s se: %s\n", hashes[hashno].name,
        (err == shaStateError) ? "PASSED" : "FAILED");

    err = USHAFinalBits(&amp;usha, 0x80, 3);
    if (printResults == PRINTTEXT)
      printf ("\nError %d. Should be %d.\n", err, shaStateError);
    if ((printPassFail == PRINTPASSFAIL) ||
        ((printPassFail == PRINTFAIL) &amp;&amp; (err != shaStateError)))
      printf("%s se: %s\n", hashes[hashno].name,
        (err == shaStateError) ? "PASSED" : "FAILED");

    err = USHAReset(0, hashes[hashno].whichSha);
    if (printResults == PRINTTEXT)
       printf("\nError %d. Should be %d.\n", err, shaNull);
    if ((printPassFail == PRINTPASSFAIL) ||
        ((printPassFail == PRINTFAIL) &amp;&amp; (err != shaNull)))
       printf("%s usha null: %s\n", hashes[hashno].name,
        (err == shaNull) ? "PASSED" : "FAILED");

    switch (hashno) {
      case SHA1: err = SHA1Reset(0); break;
      case SHA224: err = SHA224Reset(0); break;
      case SHA256: err = SHA256Reset(0); break;
      case SHA384: err = SHA384Reset(0); break;
      case SHA512: err = SHA512Reset(0); break;
    }
    if (printResults == PRINTTEXT)
       printf("\nError %d. Should be %d.\n", err, shaNull);
    if ((printPassFail == PRINTPASSFAIL) ||
        ((printPassFail == PRINTFAIL) &amp;&amp; (err != shaNull)))
       printf("%s sha null: %s\n", hashes[hashno].name,
        (err == shaNull) ? "PASSED" : "FAILED");
  }
}

// replace a hex string in place with its value

int unhexStr(char *hexstr)
{
  char *o = hexstr;
  int len = 0, nibble1 = 0, nibble2 = 0;
  if (!hexstr) return 0;
  for ( ; *hexstr; hexstr++) {
    if (isalpha((int)(unsigned char)(*hexstr))) {
      nibble1 = tolower((int)(unsigned char)(*hexstr)) - 'a' + 10;
    } else if (isdigit((int)(unsigned char)(*hexstr))) {
      nibble1 = *hexstr - '0';



    } else {
      printf("\nError: bad hex character '%c'\n", *hexstr);
    }
    if (!*++hexstr) break;
    if (isalpha((int)(unsigned char)(*hexstr))) {
      nibble2 = tolower((int)(unsigned char)(*hexstr)) - 'a' + 10;
    } else if (isdigit((int)(unsigned char)(*hexstr))) {
      nibble2 = *hexstr - '0';
    } else {
      printf("\nError: bad hex character '%c'\n", *hexstr);
    }
    *o++ = (char)((nibble1 &lt;&lt; 4) | nibble2);
    len++;
  }
  return len;
}

int main(int argc, char **argv)
{
  int i, err;
  int loopno, loopnohigh = 1;
  int hashno, hashnolow = 0, hashnohigh = HASHCOUNT - 1;
  int testno, testnolow = 0, testnohigh;
  int ntestnohigh = 0;
  int printResults = PRINTTEXT;
  int printPassFail = 1;
  int checkErrors = 0;
  char *hashstr = 0;
  int hashlen = 0;
  const char *resultstr = 0;
  char *randomseedstr = 0;
  int runHmacTests = 0;
  int runHkdfTests = 0;
  char *hmacKey = 0;
  int hmaclen = 0;
  char *info = 0;
  int infolen = 0, okmlen = 0;
  int randomcount = RANDOMCOUNT;
  const char *hashfilename = 0;
  const char *hashFilename = 0;
  int extrabits = 0, numberExtrabits = 0;
  int strIsHex = 0;

  if ('A' != 0x41) {
    fprintf(stderr, "%s: these tests require ASCII\n", argv[0]);
  }

  while ((i = getopt(argc, argv,


                      "6b:B:def:F:h:i:Hk:l:L:mpPr:R:s:S:t:wxX")) != -1)
    switch (i) {
      case 'b': extrabits = strtol(optarg, 0, 0); break;
      case 'B': numberExtrabits = atoi(optarg); break;
      case 'd': runHkdfTests = 1; break;
      case 'e': checkErrors = 1; break;
      case 'f': hashfilename = optarg; break;
      case 'F': hashFilename = optarg; break;
      case 'h': hashnolow = hashnohigh = findhash(argv[0], optarg);
        break;
      case 'H': strIsHex = 1; break;
      case 'i': info = optarg; infolen = strlen(optarg); break;
      case 'k': hmacKey = optarg; hmaclen = strlen(optarg); break;
      case 'l': loopnohigh = atoi(optarg); break;
      case 'L': okmlen = strtol(optarg, 0, 0); break;
      case 'm': runHmacTests = 1; break;
      case 'P': printPassFail = 0; break;
      case 'p': printResults = PRINTNONE; break;
      case 'R': randomcount = atoi(optarg); break;
      case 'r': randomseedstr = optarg; break;
      case 's': hashstr = optarg; hashlen = strlen(hashstr); break;
      case 'S': resultstr = optarg; break;
      case 't': testnolow = ntestnohigh = atoi(optarg) - 1; break;
      case 'w': printResults = PRINTRAW; break;
      case 'x': printResults = PRINTHEX; break;
      case 'X': printPassFail = 2; break;
      case '6': printResults = PRINTBASE64; break;
      default: usage(argv[0]);
      }

  if (strIsHex) {
    hashlen = unhexStr(hashstr);
    unhexStr(randomseedstr);
    hmaclen = unhexStr(hmacKey);
    infolen = unhexStr(info);
  }
  testnohigh = (ntestnohigh != 0) ? ntestnohigh:
               runHmacTests ? (HMACTESTCOUNT-1) :
               runHkdfTests ? (HKDFTESTCOUNT-1) :
               (TESTCOUNT-1);
  if ((testnolow &lt; 0) ||
      (testnohigh &gt;= (runHmacTests ? HMACTESTCOUNT : TESTCOUNT)) ||
      (hashnolow &lt; 0) || (hashnohigh &gt;= HASHCOUNT) ||
      (hashstr &amp;&amp; (testnolow == testnohigh)) ||
      (randomcount &lt; 0) ||
      (resultstr &amp;&amp; (!hashstr &amp;&amp; !hashfilename &amp;&amp; !hashFilename)) ||
      ((runHmacTests || hmacKey) &amp;&amp; randomseedstr) ||
      (hashfilename &amp;&amp; hashFilename) ||


      (info &amp;&amp; ((infolen &lt;= 0) || (okmlen &lt;= 0))) ||
      (info &amp;&amp; !hmacKey))
    usage(argv[0]);

  *
   *  Perform SHA/HMAC tests
   *
  for (hashno = hashnolow; hashno &lt;= hashnohigh; ++hashno) {
    if (printResults == PRINTTEXT)
      printf("Hash %s\n", hashes[hashno].name);
    err = shaSuccess;

    for (loopno = 1; (loopno &lt;= loopnohigh) &amp;&amp; (err == shaSuccess);
         ++loopno) {
      if (hashstr)
        err = hash(0, loopno, hashno, hashstr, hashlen, 1,
          numberExtrabits, extrabits, (const unsigned char *)hmacKey,
          hmaclen, (const uint8_t *) info, infolen, okmlen, resultstr,
          hashes[hashno].hashsize, printResults, printPassFail);

      else if (randomseedstr)
        randomtest(hashno, randomseedstr, hashes[hashno].hashsize, 0,
          randomcount, printResults, printPassFail);

      else if (hashfilename)
        err = hashfile(hashno, hashfilename, extrabits,
                       numberExtrabits, 0,
                       (const unsigned char *)hmacKey, hmaclen,
                       (const uint8_t *) info, infolen, okmlen,
                       resultstr, hashes[hashno].hashsize,
                       printResults, printPassFail);

      else if (hashFilename)
        err = hashfile(hashno, hashFilename, extrabits,
                       numberExtrabits, 1,
                       (const unsigned char *)hmacKey, hmaclen,
                       (const uint8_t *) info, infolen, okmlen,
                       resultstr, hashes[hashno].hashsize,
                       printResults, printPassFail);

      else // standard tests  {
        for (testno = testnolow;
             (testno &lt;= testnohigh) &amp;&amp; (err == shaSuccess); ++testno) {
          if (runHmacTests) {
            err = hash(testno, loopno, hashno,
                       hmachashes[testno].dataarray[hashno] ?
                       hmachashes[testno].dataarray[hashno] :
                       hmachashes[testno].dataarray[1] ?


                       hmachashes[testno].dataarray[1] :
                       hmachashes[testno].dataarray[0],
                       hmachashes[testno].datalength[hashno] ?
                       hmachashes[testno].datalength[hashno] :
                       hmachashes[testno].datalength[1] ?
                       hmachashes[testno].datalength[1] :
                       hmachashes[testno].datalength[0],
                       1, 0, 0,
                       (const unsigned char *)(
                        hmachashes[testno].keyarray[hashno] ?
                        hmachashes[testno].keyarray[hashno] :
                        hmachashes[testno].keyarray[1] ?
                        hmachashes[testno].keyarray[1] :
                        hmachashes[testno].keyarray[0]),
                       hmachashes[testno].keylength[hashno] ?
                       hmachashes[testno].keylength[hashno] :
                       hmachashes[testno].keylength[1] ?
                       hmachashes[testno].keylength[1] :
                       hmachashes[testno].keylength[0],
                       0, 0, 0,
                       hmachashes[testno].resultarray[hashno],
                       hmachashes[testno].resultlength[hashno],
                       printResults, printPassFail);
          } else if (runHkdfTests) {
            err = hashHkdf(testno, loopno, hashno,
                       printResults, printPassFail);
          } else { // sha tests
            err = hash(testno, loopno, hashno,
                       hashes[hashno].tests[testno].testarray,
                       hashes[hashno].tests[testno].length,
                       hashes[hashno].tests[testno].repeatcount,
                       hashes[hashno].tests[testno].numberExtrabits,
                       hashes[hashno].tests[testno].extrabits,
                       0, 0, 0, 0, 0,
                       hashes[hashno].tests[testno].resultarray,
                       hashes[hashno].hashsize,
                       printResults, printPassFail);
          }
        }
        if (!runHmacTests &amp;&amp; !runHkdfTests) {
          randomtest(hashno, hashes[hashno].randomtest,
            hashes[hashno].hashsize, hashes[hashno].randomresults,
            RANDOMCOUNT, printResults, printPassFail);
        }
      }
    }
  }




  // Test some error returns
  if (checkErrors) {
    testErrors(hashnolow, hashnohigh, printResults, printPassFail);
  }

  return 0;
}



*
 * Compare two strings, case independently.
 * Equivalent to strcasecmp() found on some systems.
 *
int scasecmp(const char *s1, const char *s2)
{
  for (;;) {
    char u1 = tolower((int)(unsigned char)(*s1++));
    char u2 = tolower((int)(unsigned char)(*s2++));
    if (u1 != u2)
      return u1 - u2;
    if (u1 == '\0')
      return 0;
   }
}


*/
