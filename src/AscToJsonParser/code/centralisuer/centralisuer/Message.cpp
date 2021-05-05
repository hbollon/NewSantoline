#include "stdafx.h"
#include "Message.h"

#include <iostream>

Message::Message()
{
}

Message::~Message()
{
}

Message *Message::set_next(Message *next)
{
	m_next = next;
	return m_next;
}

void Message::message(nlohmann::json message)
{
	if (messageTypeFromString(message["type"]) == m_message)
	{
		handleMessage(message);
	}
	else if (m_next != nullptr)
	{
		m_next->message(message);
	}
	else
	{
		std::cerr << "message[\"type\"] = " << message["type"] << " : Type de message non reconnu" << std::endl;
	}
}
