// TestACE2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ACEClient.h"

int main(int argc, char* argv[])
{
	printf("Hello World!\n");


	CACEClient Client(NULL);
	Client.Connect("", "");


	return 0;
}

