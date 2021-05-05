#include "stdafx.h"
#include "MessageAlgorithme.h"

#include <algorithm>

MessageAlgorithme::MessageAlgorithme() : m_temperature(30), m_water_reserve(40), m_duration(1), m_nbProcess(1), m_algorithm("maillage variable"), m_dimension(25)
{
	m_message = MessageType::ALGORITHME;
}

MessageAlgorithme::~MessageAlgorithme()
{
}

double MessageAlgorithme::temperature() const
{
	return m_temperature;
}

double MessageAlgorithme::water_reserve() const
{
	return m_water_reserve;
}

double MessageAlgorithme::duration() const
{
	return m_duration;
}

int MessageAlgorithme::nbProcess() const
{
	return m_nbProcess;
}

EAlgorithme MessageAlgorithme::algorithm() const
{
	EAlgorithme result;
	char *temp = _strdup(m_algorithm.c_str());
	std::transform(temp, temp + strlen(temp), temp, tolower);
	if (m_algorithm == "maillage fixe")
	{
		result = MAILLAGE_FIXE;
	}
	else if (m_algorithm == "maillage variable")
	{
		result = MAILLAGE_VARIABLE;
	}
	else if (m_algorithm == "optimal")
	{
		result = OPTIMAL;
	}
	else
	{
		result = ALGORITHM_UNDEFINED;
	}
	return result;
}

double MessageAlgorithme::dimension() const
{
	return m_dimension;
}

void MessageAlgorithme::handleMessage(nlohmann::json message)
{
	if (message["json"]["paramSim"].find("temperature") != message["json"]["paramSim"].end())
	{
		m_temperature = message["json"]["paramSim"]["temperature"].get<double>();
	}
	if (message["json"]["paramSim"].find("waterReserve") != message["json"]["paramSim"].end())
	{
		m_water_reserve = message["json"]["paramSim"]["waterReserve"].get<double>();
	}
	if (message["json"]["paramSim"].find("duration") != message["json"]["paramSim"].end())
	{
		m_duration = message["json"]["paramSim"]["duration"].get<double>();
	}
	if (message["json"]["paramSim"].find("nbProcess") != message["json"]["paramSim"].end())
	{
		m_nbProcess = message["json"]["paramSim"]["nbProcess"].get<int>();
	}
	if (message["json"]["paramSim"].find("algorithm") != message["json"]["paramSim"].end())
	{
		m_algorithm = message["json"]["paramSim"]["algorithm"].get<std::string>();
	}
	if (message["json"]["paramSim"].find("dimension") != message["json"]["paramSim"].end())
	{
		m_dimension = message["json"]["paramSim"]["dimension"].get<double>();
	}
}