#include <cstdio>
#include <iostream>

int main(void)
{
	FILE* pFile = NULL;
	fopen_s( &pFile, "input.txt", "r" );
	if(!pFile)
	{
		return -1;
	}

	char szChar[7][256] = {'\0',};
	//fscanf_s( pFile, "%s",szChar, 20);
	int i = 0;

	int szCharCNt = _countof(szChar);

	while(fgets((char*)( szChar+ i++ ), _countof(szChar[0]), pFile) != NULL)
	{

		if(i >= szCharCNt)
			break;
		
	};

	fclose(pFile);

	return 0;	
}