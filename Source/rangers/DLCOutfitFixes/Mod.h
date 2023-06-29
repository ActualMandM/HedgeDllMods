#pragma once

#include <StringHelper.h>

inline const int MAX_OUTFIT = 8;

const char* dlcNames[MAX_OUTFIT][2] = {
	{ "dlcddx", "explorer" },	// Adventurer's Gloves and Shoes
	{ "dlcmhr", "hunter" },		// Rathalos
	{ "dlcmhr", "airou" },		// Felyne Rathalos
	{ "dlcholoA", "korone" },	// Inugami Korone Collaboration Sonic Gloves & Shoes
	{ "dlccrm", "advshoes" },	// Sonic Adventure 2 Shoes
	{ "dlcxmas", "xmas" },		// Holiday Cheer Suit
	{ "chr", "birthday" },		// Sonic's Birthday Party Outfit
	{ "chr", "crown" }			// Frontier Elite
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
