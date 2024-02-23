#pragma once

#include "INIReader.h"
#include "IOHelper.h"

class ModIniData
{
public:
	struct Data
	{
		std::string title;
		std::string description;
		std::string version;
		std::string date;
		std::string author;
		std::string authorURL;

		std::string id;
		std::string dll;
		std::string code;
		std::string schema;
		std::string saveFile;

		std::string config;

		/// <summary>
		/// Reads the configuration for the active mod.
		/// </summary>
		static Data ReadConfig(std::string path)
		{
			Data modInfo;

			if (IOHelper::FileExists(path))
			{
				INIReader reader(path);

				modInfo.title = reader.Get("Desc", "Title", "");
				modInfo.description = reader.Get("Desc", "Description", "");
				modInfo.version = reader.Get("Desc", "Version", "");
				modInfo.date = reader.Get("Desc", "Date", "");
				modInfo.author = reader.Get("Desc", "Author", "");
				modInfo.authorURL = reader.Get("Desc", "AuthorURL", "");

				modInfo.id = reader.Get("Main", "ID", "");
				modInfo.dll = reader.Get("Main", "DLLFile", "");
				modInfo.code = reader.Get("Main", "CodeFile", "");
				modInfo.schema = reader.Get("Main", "ConfigSchemaFile", "");
				modInfo.saveFile = reader.Get("Main", "SaveFile", "");

				modInfo.config = path;
			}

			return modInfo;
		}
	};
};