#ifndef __DES_TOOLKIT_H__
#define __DES_TOOLKIT_H__

#include "des.h"

class DESToolKit
{
	public:
		// any numbers above zero in key2 and key3, enables DES3.
		DESToolKit( ui64 key1, ui64 key2, ui64 key3, ui64 iv) 
		: des(key1), des3(key1,key2,key3), iv(iv), last_block(iv), flag3(false)
		{
			if ( ( key2 > 0 ) && ( key3 > 0 ) )
			{
				flag3 = true;
			}
		}

	public:
		ui64 encrypt(ui64 block)
		{
			if ( flag3 == true )
			{
				last_block = des3.encrypt( block ^ last_block );
			}
			else
			{
				last_block = des.encrypt( block ^ last_block );
			}
			
			return last_block;
		}
		
		ui64 decrypt(ui64 block)
		{
			ui64 result = 0;
			
			if ( flag3 == true )
			{
				result = des3.decrypt(block) ^ last_block;
			}
			else
			{
				result = des.decrypt(block) ^ last_block;
			}
			
			last_block = block;
			return result;
		}
		
		void reset()
		{
			last_block = iv;
		}

	private:
		DES 	des;
		DES3 	des3;
		ui64 	iv;
		ui64 	last_block;
		bool    flag3;
};

#endif /// of __DES_TOOLKIT_H__
