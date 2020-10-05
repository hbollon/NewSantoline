#pragma once

#include <cctype>
#include <string>
#include <algorithm>

enum MessageType
{
	MESSAGE_UNDEFINED,
	ALGORITHME,
	EPILOBE,
	ACTIONS,
	CONTOUR,
	SIMULATION,
	ERREUR
};

static MessageType messageTypeFromChar(const char* str)
{
	char* copy = _strdup(str);
	std::transform(copy, copy + strlen(copy), copy, std::tolower);
	MessageType result = MessageType::MESSAGE_UNDEFINED;
	if (strcmp(str, "parametrealgorithme") == 0)
	{
		result = MessageType::ALGORITHME;
	}
	else if (strcmp(str, "parametreepilobe") == 0)
	{
		result = MessageType::EPILOBE;
	}
	else if (strcmp(str, "actions") == 0)
	{
		result = MessageType::ACTIONS;
	}
	else if (strcmp(str, "contour") == 0)
	{
		result = MessageType::CONTOUR;
	}
	else if (strcmp(str, "lancersimulation") == 0)
	{
		result = MessageType::SIMULATION;
	}
	else if (strcmp(str, "erreur") == 0)
	{
		result = MessageType::ERREUR;
	}
	return result;
}

static MessageType messageTypeFromString(const std::string str) {
	return messageTypeFromChar(str.c_str());
}