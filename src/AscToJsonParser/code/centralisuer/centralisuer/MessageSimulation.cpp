#include "stdafx.h"
#include "MessageSimulation.h"

#include "algorithm.h"
#include "maillage_fixe.h"
#include "maillage_variable.h"
#include "optimal.h"
#include "connector.h"
#include "CentraliseurConnecteur.h"

MessageSimulation::MessageSimulation()
{
	m_message = MessageType::SIMULATION;
}

MessageSimulation::~MessageSimulation()
{
}

void MessageSimulation::handleMessage(nlohmann::json message)
{
	if (m_epilobe->is_generated())
	{
		m_reader->write(vectorPointToJSON(execute()).dump());
	}
}

std::vector<Point3D> MessageSimulation::execute()
{
	Algorithm *algorithm = nullptr;

	DatasHandler handler;
	//handler.load((int)round(m_epilobe->direction()), (int)round(m_epilobe->force()), 25,std::string("fill"));

	Connector *connector = new CentraliseurConnecteur(&handler);
	std::cout << "message algorithme : " << m_algorithme->algorithm() << std::endl;
	switch (m_algorithme->algorithm())
	{
	case MAILLAGE_FIXE:
		algorithm = new MaillageFixe(connector, m_algorithme->duration(), (int)m_algorithme->dimension());
		algorithm->get_starting_points() = m_contour->contour_initial();
		break;
	case MAILLAGE_VARIABLE:
		algorithm = new MaillageVariable(connector, m_algorithme->duration(), (int)m_algorithme->dimension());
		algorithm->get_starting_points() = m_contour->contour_initial();
		break;
	case OPTIMAL:
		algorithm = new Optimal(connector, m_algorithme->duration(), (int)m_algorithme->dimension(), m_contour->contour_initial(), 0.1, 3);
		break;
	case MESSAGE_UNDEFINED:
		break;
	default:
		break;
	}

	std::vector<Point3D> result = algorithm->propagation();
	delete algorithm;
	delete connector;
	return result;
}

void MessageSimulation::set_algorithm(MessageAlgorithme *algorithme)
{
	m_algorithme = algorithme;
}
void MessageSimulation::set_epilobe(MessageEpilobe *epilobe)
{
	m_epilobe = epilobe;
}
void MessageSimulation::set_contour(MessageContour *contour)
{
	m_contour = contour;
}

void MessageSimulation::set_reader(Reader *reader)
{
	m_reader = reader;
}

nlohmann::json MessageSimulation::vectorPointToJSON(std::vector<Point3D> points)
{
	nlohmann::json result;

	for (std::vector<Point3D>::const_iterator it = points.begin(); it != points.end(); ++it)
	{
		nlohmann::json point;
		point.push_back(it->x());
		point.push_back(it->y());
		point.push_back(it->z());
		result.push_back(point);
	}
	return result;
}