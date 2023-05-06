#pragma once

#include <StringHelper.h>

inline const int MAX_OUTFIT = 6;

const char* dlcNames[MAX_OUTFIT][2] = {
	{ "dlcddx", "explorer" },
	{ "dlcmhr", "hunter" },
	{ "dlcmhr", "airou" },
	{ "dlcholoA", "korone" },
	{ "dlccrm", "advshoes" },
	{ "dlcxmas", "xmas" }
};

const char* GetModelName(uint8_t outfit, const char* model, const char* type)
{
	std::stringstream modelName;

	modelName << dlcNames[outfit][0] << "_" << model << "_" << dlcNames[outfit][1];

	if (type != nullptr)
	{
		modelName << "_" << type;
	}

	return modelName.str().c_str();
}
