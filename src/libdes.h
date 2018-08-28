#ifndef __LIBDES_H__
#define __LIBDES_H__

#include "destk.h"

#ifndef ui64
	#define ui64 uint64_t
#endif

#ifndef ui32
	#define ui32 uint32_t
#endif

#ifndef ui8
	#define ui8  uint8_t
#endif

class libDES
{
	public:
		// any numbers above zero in key2 and key3, enables DES3.
		libDES( ui64 key1, ui64 key2 = 0, ui64 key3 = 0 );
		virtual ~libDES();
	
	public:
		bool encryptFile( const char* srcfn = 0, const char* outfn = 0 );
		bool decryptFile( const char* srcfn = 0, const char* outfn = 0 );

	public:
		bool encryptBuff( const char* sb, unsigned sbsz, char* &ob, unsigned &obsz );
		bool decryptBuff( const char* sb, unsigned sbsz, char* &ob, unsigned &obsz );

	private:
		bool cipherFile( const char* srcfn, const char* outfn, bool isdecrypt );
		bool cipherBuff( const char* src, unsigned srcsz, char* &out, unsigned &outsz, bool isdecrypt );

	private:
		DESToolKit 	destk;
};

#endif /// of __LIBDES_H__
