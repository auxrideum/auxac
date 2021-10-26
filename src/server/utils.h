bool MemoryCompare(const BYTE* data, const BYTE* pattern, const char* mask);
unsigned long FindPattern(const char* pattern, const char* mask);
BYTE GetPacketID(Packet* p);