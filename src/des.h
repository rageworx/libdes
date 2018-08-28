#ifndef __DES_H__
#define __DES_H__

#include <cstdint>

#ifndef ui64
	#define ui64 uint64_t
#endif

#ifndef ui32
	#define ui32 uint32_t
#endif

#ifndef ui8
	#define ui8  uint8_t
#endif

class DES
{
	public:
		DES(ui64 key);
		virtual ~DES() {}
	
	public:
		ui64 des(ui64 block, bool mode);

		ui64 encrypt(ui64 block);
		ui64 decrypt(ui64 block);

		static ui64 encrypt(ui64 block, ui64 key);
		static ui64 decrypt(ui64 block, ui64 key);

	protected:
		void keygen(ui64 key);

		ui64 ip(ui64 block);
		ui64 fp(ui64 block);

		void feistel(ui32 &L, ui32 &R, ui32 F);
		ui32 f(ui32 R, ui64 k);

	private:
		// sub_key[16] means 48 bits each
		ui64 sub_key[16]; 
};

class DES3
{
	public:
		DES3(ui64 k1, ui64 k2, ui64 k3) : des1(k1), des2(k2), des3(k3) {}
		virtual ~DES3() {}
		ui64 encrypt(ui64 block) \
			{ return des3.encrypt(des2.decrypt(des1.encrypt(block))); }
		ui64 decrypt(ui64 block) \
			{ return des1.decrypt(des2.encrypt(des3.decrypt(block))); }

	private:
		DES des1;
		DES des2;
		DES des3;
};

#endif /// of __DES_H__
