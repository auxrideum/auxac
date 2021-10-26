struct PAWNArray
{
	cell* address;
	size_t length;
};

namespace Callback
{
	std::list<AMX*>& GetAMXList();

	cell OnLoadLibrary(int iPlayerId, const char* szPath);
}