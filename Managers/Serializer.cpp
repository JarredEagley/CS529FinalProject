/* Start Header -------------------------------------------------------

	Copyright (C) 20xx DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.

	File Name:			Serializer.cpp
	Purpose:			Handles turning json files into something usable.
						This is pretty much just a singleton which has
						a loadJson function. 
	Language:			C++, compiled using Microsoft Visual Studio 2019.
	Platform:			Compiled using Visual Studio 2019, Windows 10.
	Project:			JarredEagley_Milestone2
	Author:				Jarred Eagley, jarred.eagley, SID: 400000520
	Creation date:		10/24/2020

- End Header --------------------------------------------------------*/

#include "Serializer.h"

#include <fstream>
#include <iostream>

#include "rapidjson/istreamwrapper.h"

Serializer* Serializer::instance = nullptr;

void Serializer::destroySingleton()
{
	delete instance; // this will do more if i start keeping the jsons i need loaded.
} 

rapidjson::Document Serializer::loadJson(std::string filePath)
{
	std::ifstream inputStream(filePath);
	// Make sure the stream opened.
	if (inputStream.is_open())
	{
		// Read in JSON from stream.
		rapidjson::IStreamWrapper jsonStreamWrapper{ inputStream };
		rapidjson::Document doc{};
		doc.ParseStream(jsonStreamWrapper);

		// Make sure parsing was successful!
		if (doc.HasParseError())
		{
			std::cerr << "Error: " << doc.GetParseError() << "\n"
				<< "Offset: " << doc.GetErrorOffset() << "\n";
			return NULL; 
		}
		
		return doc;
	}
	else
	{
		// Failed to open input stream.
		std::cerr << "Error: Failed to open input stream for file '" << filePath << "'" << std::endl;
		return NULL;
	}
}