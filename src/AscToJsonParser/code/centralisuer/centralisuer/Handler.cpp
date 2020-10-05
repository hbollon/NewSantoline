#include "stdafx.h"
#include "Handler.h"

#include <string>
#include <fstream>

#include "json.hpp"

#include "EnumMessage.h"

#include "Message.h"
#include "MessageActions.h"
#include "MessageAlgorithme.h"
#include "MessageContour.h"
#include "MessageEpilobe.h"
#include "MessageErreur.h"
#include "MessageSimulation.h"

Handler::Handler(Reader* reader) : m_reader(reader)
{
}

Handler::~Handler()
{
}

void Handler::run() 
{
	
	MessageAlgorithme algorithme;
	MessageEpilobe epilobe;
	MessageActions actions;
	MessageContour contour;
	MessageSimulation simulation;
	MessageErreur erreur;

	algorithme.set_next(&epilobe)
		->set_next(&actions)
		->set_next(&contour)
		->set_next(&simulation)
		->set_next(&erreur);

	simulation.set_algorithm(&algorithme);
	simulation.set_epilobe(&epilobe);
	simulation.set_contour(&contour);
	simulation.set_reader(m_reader);

	m_running = true;
	nlohmann::json message;
	while (m_running){
		try 
		{
			message = nlohmann::json::parse(m_reader->read());
			algorithme.message(message);
		}
		catch (const nlohmann::detail::type_error e)
		{
			std::cerr << "Handler::Run() Erreur de parsing : " << e.what() << std::endl;
		}
		catch (const nlohmann::json::parse_error e) 
		{		
			std::cerr << "Handler::Run() Erreur de parsing : " << e.what() << std::endl;
		}
	}
}