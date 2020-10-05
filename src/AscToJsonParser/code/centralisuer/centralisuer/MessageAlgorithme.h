#pragma once
#include <string>

#include "json.hpp"

#include "Message.h"

enum EAlgorithme 
{
	ALGORITHM_UNDEFINED, 
	MAILLAGE_FIXE, 
	MAILLAGE_VARIABLE, 
	OPTIMAL
};

class MessageAlgorithme : public Message
{
public:
	MessageAlgorithme();
	~MessageAlgorithme();
	double temperature() const;
	double water_reserve() const;
	double duration() const;
	int nbProcess() const;
	EAlgorithme algorithm() const;
	double dimension() const;
	virtual void handleMessage(nlohmann::json message);
private:
	double m_temperature;
	double m_water_reserve;
	double m_duration;
	int m_nbProcess;
	std::string m_algorithm;
	double m_dimension;
};