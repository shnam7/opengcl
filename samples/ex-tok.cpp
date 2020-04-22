//
//		* extok.cpp
//
//		OpenGCL Example: tokenizer operations
//
//		Written by Soo-Hyuk Nam.
//			2003. 3. 18. Tue.
//

#include <stdio.h>

#define GTOK_SUPPORT_STL
#include "gtok.h"

int test1()
{
	const char *str = "A;B;C:D;E;F;\n";
	GTokenizer gtok( str, ";\n" );
	while ( gtok.hasMoreTokens() )
	{
		char buf[256];
		char sep;
		unsigned tokLen=0;
		if ( !gtok.getNext(buf, sizeof(buf), &sep, &tokLen) )
		{
			printf( "Strange error...! must be bug.\n" );
			return -1;
		}
		printf( "Token=<%s> Len=%d Separator=<%c>\n", buf, tokLen, sep );
	}
	printf( "Test1 Done.\n\n" );
	return 0;
}

int stl_test()
{
	const char *str = "Name1=Val1\nName2=Val2\n";
	GTokenizer gtok;
	gtok.associate( str, "\n" );
	while ( gtok.hasMoreTokens() )
	{
		std::string pair;
		if ( !gtok.getNext(pair) )
		{
			printf( "Strange error...! must be bug.\n" );
			return -1;
		}
		printf( "Token Pair=<%s>\n", pair.c_str() );

		GTokenizer gtok2( pair.c_str(), "=" );
		std::string name, val;
		gtok2.getNext( name );
		gtok2.getNext( val );
		printf( "Name=<%s> Value=<%s>\n", name.c_str(), val.c_str() );
	}
	printf( "STL Test Done.\n\n" );
	return 0;
}

int main()
{
	test1();
	stl_test();
	return 0;
}
