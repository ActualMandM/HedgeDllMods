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

void GetSonicName(char* result, uint8_t outfit, const char* model, const char* type)
{
	strcpy(result, dlcNames[outfit][0]);
	strcat(result, "_");
	strcat(result, model);
	strcat(result, "_");
	strcat(result, dlcNames[outfit][1]);

	if (type != nullptr)
	{
		strcat(result, "_");
		strcat(result, type);
	}
}

void GetFriendName(char* result, const char* model, const char* type)
{
	strcpy(result, "chr_");
	strcat(result, model);

	if (type != nullptr)
		strcat(result, type);

	strcat(result, "_birthday");
}
