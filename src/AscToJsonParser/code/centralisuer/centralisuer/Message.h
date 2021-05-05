#pragma once
#include "EnumMessage.h"
#include "json.hpp"
class Message
{
public:
	Message();
	~Message();

	Message *set_next(Message *next);
	void message(nlohmann::json message);

	virtual void handleMessage(nlohmann::json message) = 0;

protected:
	MessageType m_message;
	Message *m_next;
};
