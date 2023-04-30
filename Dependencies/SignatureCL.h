#pragma once

#include <CommonLoaderAPI.h>

inline bool sigValid = true;

inline void* sigScan(const char* pattern, const char* mask)
{
	CommonLoader_GetAPI()->SetState(CMN_LOADER_STATE_SKIP_SIG_VALIDATION, 1);
	return CommonLoader_GetAPI()->ScanSignature(pattern, mask);
}

#define SIG_SCAN(x, ...) \
	void* x(); \
	void* x##Addr = x(); \
	void* x() \
	{ \
		static const char* x##Data[] = { __VA_ARGS__ }; \
		if (!x##Addr) \
		{ \
			for (int i = 0; i < _countof(x##Data); i += 2) \
			{ \
				x##Addr = sigScan(x##Data[i], x##Data[i + 1]); \
				printf("[Signature] %s received: 0x%llx\n", #x, x##Addr); \
				if (x##Addr) \
					return x##Addr; \
			} \
			sigValid = false; \
		} \
		return x##Addr; \
	}
