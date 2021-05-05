#include "stdafx.h"
#include "MessageActions.h"

MessageActions::MessageActions()
{
	m_message = MessageType::ACTIONS;
}

MessageActions::~MessageActions()
{
}

void MessageActions::handleMessage(nlohmann::json message)
{
	//TODO
}