#include "libdes.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>

using namespace std;

libDES::libDES( ui64 key1, ui64 key2, ui64 key3 ) 
 : destk( key1, key2, key3, (ui64) 0x0000000000000000 )
{
}

libDES::~libDES()
{
}

////

bool libDES::encryptFile( const char* srcfn, const char* outfn )
{
    return cipherFile( srcfn, outfn, false );
}

bool libDES::decryptFile( const char* srcfn, const char* outfn )
{
    return cipherFile( srcfn, outfn, true );
}

bool libDES::cipherFile( const char* srcfn, const char* outfn, bool isdecrypt )
{
	if ( ( srcfn == NULL ) || ( outfn == NULL ) )
		return false;
	
	bool     retb = true;
    ifstream ifile;
    ofstream ofile;
    ui64     buffer;

    ifile.open( srcfn, ios::binary | ios::in | ios::ate );
    ofile.open( outfn, ios::binary | ios::out );

    ui64 size = ifile.tellg();
	
	if ( size == 0 )
		return false;
    
	ifile.seekg(0, ios::beg);

    ui64 block = size / 8;
	
    if( isdecrypt == true ) 
	{
		block--;
	}

    for(ui64 i = 0; i < block; i++)
    {
        ifile.read((char*) &buffer, 8);

        if( isdecrypt == true )
		{
			buffer = destk.decrypt(buffer);
		}
        else
		{
            buffer = destk.encrypt(buffer);
		}

        ofile.write( (char*) &buffer, 8 );
    }

    if( isdecrypt == true )
    {
        // Read last line of file
        ifile.read((char*) &buffer, 8);
        buffer = destk.decrypt(buffer);

        // Amount of padding on file
        ui8 padding = 0;

        // Check for and record padding on end
        while( ( buffer & 0x00000000000000FF ) == 0 )
        {
            buffer >>= 8;
            padding++;
        }

        buffer >>= 8;
        padding++;

        if( padding != 8 )
		{
			ofile.write((char*) &buffer, 8 - padding);
		}
    }
	else
    {
        // Amount of padding needed
        ui8 padding = 8 - (size % 8);

        // Padding cannot be 0 (pad full block)
        if (padding == 0)
            padding  = 8;

        // Read remaining part of file
        buffer = (ui64) 0;
		
        if(padding != 8)
        {
			ifile.read((char*) &buffer, 8 - padding);
		}

        // Pad block with a 1 followed by 0s
        ui8 shift = padding * 8;
        buffer <<= shift;
        buffer  |= (ui64) 0x0000000000000001 << (shift - 1);

        buffer = destk.encrypt(buffer);
        ofile.write( (char*) &buffer, 8 );
    }

    ifile.close();
    ofile.close();
	
    return retb;
}

bool libDES::encryptBuff( const char* sb, unsigned sbsz, char* &ob, unsigned &obsz )
{
	return cipherBuff( sb, sbsz, ob, obsz, false );
}

bool libDES::decryptBuff( const char* sb, unsigned sbsz, char* &ob, unsigned &obsz )
{
	return cipherBuff( sb, sbsz, ob, obsz, true );
}

bool libDES::cipherBuff( const char* src, unsigned srcsz, char* &out, unsigned &outsz, bool isdecrypt )
{
	if ( ( src == NULL ) || ( srcsz == 0 ) )
		return false;
	
	bool retb = true;

    ui64 block = srcsz / 8;
	
    if( isdecrypt == true ) 
	{
		block--;
	}
	
	out = new char[ block * 8 ];
	
	if ( out == NULL )
		return false;

	ui64 buffer = 0;
	
    for( ui64 i = 0; i < block; i++ )
    {
		memcpy( &buffer, &src[ i * 8 ] , 8 );
		
        if( isdecrypt == true )
		{
			buffer = destk.decrypt(buffer);
		}
        else
		{
            buffer = destk.encrypt(buffer);
		}

		memcpy( &out[ i * 8 ], &buffer, 8 );
    }
	
	outsz = block * 8;

	// process left bytes within 8.
    if( isdecrypt == true )
    {
		unsigned cpsz = 8;
		
		if ( ( block*8 + 8 ) > srcsz )
		{
			cpsz = srcsz - ( block*8 );
		}

		if ( cpsz > 0 )
		{
			memcpy( &buffer, &src[ block * 8 ] , cpsz );
			
			buffer = destk.decrypt(buffer);

			// Amount of padding on file
			ui8 padding = 0;

			// Check for and record padding on end
			while( ( buffer & 0x00000000000000FF ) == 0 )
			{
				buffer >>= 8;
				padding++;
			}

			buffer >>= 8;
			padding++;

			if( padding != 8 )
			{
				memcpy( &out[ block * 8 ], &buffer, 8 - padding );
				outsz += 8 - padding;
			}
		}
    }
	else
    {
        // Amount of padding needed
        ui8 padding = 8 - (srcsz % 8);

        // Padding cannot be 0 (pad full block)
        if (padding == 0)
            padding  = 8;

        // Read remaining part of file
        buffer = (ui64) 0;
		
        if(padding != 8)
        {
			memcpy( &buffer, &src[ block * 8 ] , 8 - padding );
		}

        // Pad block with a 1 followed by 0s
        ui8 shift = padding * 8;
        buffer <<= shift;
        buffer  |= (ui64) 0x0000000000000001 << (shift - 1);

        buffer = destk.encrypt(buffer);
		memcpy( &out[ block * 8 ], &buffer, 8 );
		outsz += 8;
    }
	
	return retb;
}