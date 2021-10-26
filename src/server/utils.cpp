#include "main.h"

bool MemoryCompare(const BYTE* data, const BYTE* pattern, const char* mask)
{
	for (; *mask; ++mask, ++data, ++pattern)
	{
		if (*mask == 'x' && *data != *pattern)
			return false;
	}
	return (*mask) == NULL;
}

unsigned long FindPattern(const char* pattern, const char* mask)
{
	unsigned long i;
	unsigned long ulSize;
	unsigned long ulAddress;
#ifdef _WIN32

	MODULEINFO mInfo = { 0 };

	ulAddress = (unsigned long)GetModuleHandle(NULL);
	GetModuleInformation(GetCurrentProcess(), GetModuleHandle(NULL), &mInfo, sizeof(MODULEINFO));
	ulSize = (unsigned long)mInfo.SizeOfImage;

#else
	ulAddress = 0x804b480;
	ulSize = 0x8128B80 - ulAddress;
#endif

	for (i = 0; i < ulSize; ++i)
	{
		if (MemoryCompare((BYTE*)(ulAddress + i), (BYTE*)pattern, mask))
			return (unsigned long)(ulAddress + i);
	}
	return 0;
}

BYTE GetPacketID(Packet* p)
{
	if (p == 0)
		return 255;

	if ((unsigned char)p->data[0] == 36)
	{
		assert(p->length > sizeof(unsigned char) + sizeof(unsigned long));
		return (unsigned char)p->data[sizeof(unsigned char) + sizeof(unsigned long)];
	}
	else return (unsigned char)p->data[0];
}