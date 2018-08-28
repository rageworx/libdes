#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <string>
#include <iostream>

using namespace std;

#include "libdes.h"

static char* me = NULL;

#ifdef _WIN32
	#define PATH_TOK	"\\"
#else
	#define PATH_TOK	"/"
#endif 

void usage()
{
    printf( "Usage: %s -e/-d key [input-file] [output-file]\n", me );
}

void stripme( char* ref )
{
	if ( ref != NULL )
	{
		me = strtok( ref, PATH_TOK );
		while( true )
		{
			char* x = strtok( NULL, PATH_TOK );
			if ( x == NULL )
			{
				break;
			}
			me = x;
		}
	}
}

bool readfile( string fn, char* &data, unsigned &datalen )
{
	datalen = 0;
	
	if ( fn.size() > 0 )
	{
		FILE* fp = fopen( fn.c_str(), "rb" );
		if ( fp != NULL )
		{
			fseek( fp, 0L, SEEK_END );
			unsigned fsz = ftell( fp );
			rewind( fp );
			
			data = new char[ fsz ];
			if ( data != NULL )
			{
				fread( data, 1, fsz, fp );
				datalen = fsz;
			}
			
			fclose( fp );
			
			if ( datalen > 0 )
				return true;
		}
	}
	
	return false;
}

bool writefile( string fn, char* data, unsigned datalen )
{
	if ( fn.size() > 0 )
	{
		FILE* fp = fopen( fn.c_str(), "wb" );
		if ( fp != NULL )
		{
			fwrite( data, 1, datalen, fp );
			fclose( fp );
			
			return true;
		}
	}
	
	return false;
}

int main(int argc, char **argv)
{
	stripme( argv[0] );
	
    if(argc < 4)
    {
        usage();
        return 1;
    }

    string enc_dec = argv[1];
    if( ( enc_dec != "-e") && ( enc_dec != "-d" ) )
    {
        usage();
        return 2;
    }

    string input,output;
    if(argc > 3)
        input  = argv[3];
    if(argc > 4)
        output = argv[4];

    ui64 key = strtoull(argv[2], nullptr, 16);
    libDES des(key);

    if(enc_dec == "-e")
    {
		//return des.encryptFile(input.c_str(), output.c_str());
		
		char* 	 src    = NULL;
		char*    out    = NULL;
		unsigned srclen = 0;
		unsigned outlen = 0;
		
		if ( readfile( input, src, srclen ) == true )
		{
			if ( des.encryptBuff( src, srclen, out, outlen ) == true )
			{
				writefile( output, out, outlen );
			}
			else
			{
				printf( "Failed to encrypt on buffer.\n" );
			}
		}
		
		delete[] src;
		delete[] out;
	}
	
    if(enc_dec == "-d")
	{
		//return des.decryptFile(input.c_str(), output.c_str());

		char* 	 src    = NULL;
		char*    out    = NULL;
		unsigned srclen = 0;
		unsigned outlen = 0;
		
		if ( readfile( input, src, srclen ) == true )
		{
			if ( des.decryptBuff( src, srclen, out, outlen ) == true )
			{
				writefile( output, out, outlen );
			}
			else
			{
				printf( "Failed to decrypt on buffer.\n" );
			}
		}
		
		delete[] src;
		delete[] out;
	}

    return 0;
}
