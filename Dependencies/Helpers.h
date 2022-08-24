#pragma once

#define _CONCAT2(x, y) x##y
#define CONCAT2(x, y) _CONCAT(x, y)
#define INSERT_PADDING(length) \
	uint8_t CONCAT2(pad, __LINE__)[length]

#define ASSERT_OFFSETOF(type, field, offset) \
	static_assert(offsetof(type, field) == offset, "offsetof assertion failed")

#define ASSERT_SIZEOF(type, size) \
	static_assert(sizeof(type) == size, "sizeof assertion failed")

#ifdef BASE_ADDRESS
const HMODULE MODULE_HANDLE = GetModuleHandle(nullptr);

#define ASLR(address) \
	((size_t)MODULE_HANDLE + (size_t)address - (size_t)BASE_ADDRESS)
#endif

#define FUNCTION_PTR(returnType, callingConvention, function, location, ...) \
	returnType (callingConvention *function)(__VA_ARGS__) = (returnType(callingConvention*)(__VA_ARGS__))(location)

#define PROC_ADDRESS(libraryName, procName) \
	GetProcAddress(LoadLibrary(TEXT(libraryName)), procName)

#define HOOK(returnType, callingConvention, functionName, location, ...) \
	typedef returnType callingConvention _##functionName(__VA_ARGS__); \
	_##functionName* original##functionName = (_##functionName*)(location); \
	returnType callingConvention implOf##functionName(__VA_ARGS__)

#define INSTALL_HOOK(functionName) \
	{ \
		DetourTransactionBegin(); \
		DetourUpdateThread(GetCurrentThread()); \
		DetourAttach((void**)&original##functionName, implOf##functionName); \
		DetourTransactionCommit(); \
	}

#define VTABLE_HOOK(returnType, callingConvention, className, functionName, ...) \
	typedef returnType callingConvention className##functionName(className* This, __VA_ARGS__); \
	className##functionName* original##className##functionName; \
	returnType callingConvention implOf##className##functionName(className* This, __VA_ARGS__)

#define INSTALL_VTABLE_HOOK(className, object, functionName, functionIndex) \
	{ \
		void** addr = &(*(void***)object)[functionIndex]; \
		if (*addr != implOf##className##functionName) \
		{ \
			original##className##functionName = (className##functionName*)*addr; \
			DWORD oldProtect; \
			VirtualProtect(addr, sizeof(void*), PAGE_EXECUTE_READWRITE, &oldProtect); \
			*addr = implOf##className##functionName; \
			VirtualProtect(addr, sizeof(void*), oldProtect, &oldProtect); \
		} \
	}

#define WRITE_MEMORY(location, type, ...) \
	{ \
		const type data[] = { __VA_ARGS__ }; \
		DWORD oldProtect; \
		VirtualProtect((void*)(location), sizeof(data), PAGE_EXECUTE_READWRITE, &oldProtect); \
		memcpy((void*)(location), data, sizeof(data)); \
		VirtualProtect((void*)(location), sizeof(data), oldProtect, &oldProtect); \
	}

#define WRITE_JUMP(location, function) \
	{ \
		WRITE_MEMORY(location, uint8_t, 0xE9); \
		WRITE_MEMORY(location + 1, uint32_t, (uint32_t)(function) - (size_t)(location) - 5); \
	}

#define WRITE_CALL(location, function) \
	{ \
		WRITE_MEMORY(location, uint8_t, 0xE8); \
		WRITE_MEMORY(location + 1, uint32_t, (uint32_t)(function) - (size_t)(location) - 5); \
	}

#define WRITE_NOP(location, count) \
	{ \
		DWORD oldProtect; \
		VirtualProtect((void*)(location), (size_t)(count), PAGE_EXECUTE_READWRITE, &oldProtect); \
		for (size_t i = 0; i < (size_t)(count); i++) \
			*((uint8_t*)(location) + i) = 0x90; \
		VirtualProtect((void*)(location), (size_t)(count), oldProtect, &oldProtect); \
	}

#define ASMHOOK void __declspec(naked)

inline uint32_t readUnalignedU32(void* memory)
{
	uint8_t* p = (uint8_t*)memory;
	return p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24);
}

// From brianuuu's Common.h (https://github.com/brianuuu/DllMods/blob/785b18899efc8824e769db904372da2660f97c1f/Dependencies/Common.h)
namespace Helpers
{
	inline uint32_t GetMultiLevelAddress(uint32_t initAddress, std::vector<uint32_t> offsets)
	{
		uint32_t address = *(uint32_t*)initAddress;
		for (uint32_t i = 0; i < offsets.size(); i++)
		{
			uint32_t const& offset = offsets[i];
			address += offset;

			if (i < offsets.size() - 1)
			{
				address = *(uint32_t*)address;
			}
		}
		return address;
	}

	inline bool CheckCurrentStage(char const* stageID)
	{
		char const* currentStageID = (char*)0x01E774D4;
		return strcmp(currentStageID, stageID) == 0;
	}

	inline uint32_t GetCurrentStageID()
	{
		uint32_t stageIDAddress = GetMultiLevelAddress(0x1E66B34, { 0x4, 0x1B4, 0x80, 0x0 });
		return *(uint32_t*)stageIDAddress;
	}

	inline bool IsCurrentStageMission()
	{
		uint32_t stageID = GetCurrentStageID();
		return (stageID & 0xFF00) > 0 && (stageID & 0xFF) <= 0x11;
	}
}
