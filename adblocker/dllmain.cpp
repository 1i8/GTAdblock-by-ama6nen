#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
#include <cstdint>
#include <vector>
#include <list>
#include <string>


#pragma region exports
void CustomVM00000109_Start() {

}

void CustomVM00000109_End() {

}

void CustomVM00000103_Start() {

}

void CustomVM00000103_End() {

}
#pragma endregion

#pragma region util
uintptr_t find_pattern(const char* pat, intptr_t offset = 0) {
	static auto pat2byt = [](const char* pat) {
		std::vector<int> bytes{};
		auto start = (char*)pat;
		auto end = start + strlen(pat);

		for (auto curr = start; curr < end; ++curr) {
			if (*curr == '?') {
				++curr;
				if (*curr == '?')
					++curr;
				bytes.push_back(-1);
			}
			else
				bytes.push_back(strtoul(curr, &curr, 16));
		}
		return bytes;
	};

	auto hmod = GetModuleHandleW(nullptr);
	auto nthdrs = PIMAGE_NT_HEADERS(uintptr_t(hmod) + PIMAGE_DOS_HEADER(hmod)->e_lfanew);
	auto opthdr = &nthdrs->OptionalHeader;
	auto start = uintptr_t(hmod) + opthdr->BaseOfCode;
	auto end = opthdr->SizeOfCode;
	auto bytes = pat2byt(pat);
	size_t patsize = bytes.size();
	auto data = bytes.data();
	auto found = false;
	if (!start || !end || patsize < 1)
		return uintptr_t(0x0);
	for (size_t i = start; i <= start + end; i++) {
		for (size_t j = 0; j < patsize; j++) {
			if (*((PBYTE)i + j) == data[j] || data[j] == -1)
				found = true;
			else {
				found = false;
				break;
			}
		}
		if (found)
			return uintptr_t(i + offset);
	}
	return uintptr_t(0);
}

uintptr_t get_call(uintptr_t call) {
	if (call < 15)
		return uintptr_t();
	auto offset = *(int32_t*)(call + 1);
	return uintptr_t(call + offset + 0x5);
}

uintptr_t get_call(const char* pattern, int offset = 0) {
	return get_call(find_pattern(pattern) + offset);
}
#pragma endregion

#pragma region sdk
#pragma pack(push, 1)
#define GTClass struct __declspec(align(1)) alignas(1) 

struct World;

GTClass Tile{
	char pad[144];
};
GTClass WorldTileMap{
   public:
	void* vftable;
	int width;
	int height;
	char pad0[8];

   public:
	std::vector<Tile> tiles;
	World* world;
};
GTClass WorldObject{
   public:
	void* vtable;
	float posx;
	float posy;
	uint16_t item_id;
	uint8_t amount;
	uint8_t flags;
	uint32_t object_id;
	char padding[24];
};
GTClass WorldObjectMap{
   public:
	void* vftable;
	uint32_t object_counter;
	uint32_t unk;
   public:
	std::list<WorldObject> objects;
};
GTClass World{
	 public:
	void* vtable;
	uint8_t unk1;
	uint8_t unk2;
	short version;
	char pad1[4];
	char tilemap[56];
	WorldObjectMap worldobjectmap;
	std::string name;
	char unk3[8];
	short disableads;
};
GTClass GameLogic{
	char pad[248];
	bool local_building;
	char pad1[15];
	World* world;
};
#pragma pack(pop)

#pragma endregion

HMODULE self = NULL;
void injection() {
	Sleep(1000);
	GameLogic* gamelogic = nullptr;
	while (true) {

		if (!gamelogic) {
			static auto address = (GameLogic * (__cdecl*)()) get_call("E8 ? ? ? ? 0F BF 0F");
			gamelogic = address();
		}
		else {
			if (gamelogic->world)
				gamelogic->world->disableads = 1; 
			//first time ever weather is sunset because its also weather id 1, and this value is used 1st time
			//after first world enter any number aside from 0 will disable ads but not affect weather otherwise

		}
		Sleep(250);
	}
	FreeLibraryAndExitThread(self, 1);
}


BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		hModule = self;
		CreateThread(nullptr, 0, LPTHREAD_START_ROUTINE(injection), nullptr, 0, nullptr);
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

