uint32_t midAsmHookFunctionAddress = 0x6621A0;
uint32_t midAsmHookReturnAddress = 0x74251B;

const char* volatile const shader = "SysDiffuse";

ASMHOOK replaceShader()
{
	__asm
	{
		add esp, 4
		push shader
		call[midAsmHookFunctionAddress]
		jmp[midAsmHookReturnAddress]
	}
}

extern "C" __declspec(dllexport) void Init()
{
	WRITE_JUMP(0x742516, replaceShader);
}
