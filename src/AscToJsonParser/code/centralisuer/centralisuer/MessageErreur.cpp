#include "stdafx.h"
#include "MessageErreur.h"

MessageErreur::MessageErreur()
{
	m_message = MessageType::ERREUR;
}

MessageErreur::~MessageErreur()
{
}

void MessageErreur::handleMessage(nlohmann::json message)
{
	//TODO
}