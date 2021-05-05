#pragma once
#include <string>
#include <utility>
#include "json.hpp"
#include "Message.h"
#include "Configuration.h"

class MessageEpilobe : public Message
{
public:
	MessageEpilobe();
	~MessageEpilobe();
	bool can_generate();
	void generate();
	bool is_generated();
	double direction() const;
	double force() const;
	std::pair<double, double> origin() const;
	std::pair<double, double> dimension() const;
	bool subzone();
	bool alti_to_ascii();
	bool windninja();
	virtual void handleMessage(nlohmann::json message);

private:
	bool m_generated;
	std::string m_axe;
	double m_direction;
	double m_force;
	int m_nb_process;
	std::pair<double, double> m_origin;
	std::pair<double, double> m_dimension;
	Configs m_configs;
};