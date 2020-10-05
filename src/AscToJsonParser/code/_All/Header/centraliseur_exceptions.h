#ifndef __CENTRALISEUR_EXCEPTION__ 
#define __CENTRALISEUR_EXCEPTION__ 

#include <exception>
#include <string>

class UnknownCommandException : public std::exception{
public:
	explicit UnknownCommandException(const char* message) : m_msg(message){}
	explicit UnknownCommandException(const std::string& message) : m_msg(message){}
	virtual ~UnknownCommandException() throw(){}
	virtual const char* what() const throw(){
		return m_msg.c_str();
	}
protected:
	std::string m_msg;
};

class ReceiveErrorException : public std::exception{
public:
	explicit ReceiveErrorException(const char* message) : m_msg(message){}
	explicit ReceiveErrorException(const std::string& message) : m_msg(message){}
	virtual ~ReceiveErrorException() throw(){}
	virtual const char* what() const throw(){
		return m_msg.c_str();
	}
protected:
	std::string m_msg;
};

class InvalidArgumentException : public std::exception{
public:
	explicit InvalidArgumentException(const char* message) : m_msg(message){}
	explicit InvalidArgumentException(const std::string& message) : m_msg(message){}
	virtual ~InvalidArgumentException() throw(){}
	virtual const char* what() const throw(){
		return m_msg.c_str();
	}
protected:
	std::string m_msg;
};

class InputFileException : public std::exception{
public:
	explicit InputFileException(const char* message) : m_msg(message){}
	explicit InputFileException(const std::string& message) : m_msg(message){}
	virtual ~InputFileException() throw(){}
	virtual const char* what() const throw(){
		return m_msg.c_str();
	}
protected:
	std::string m_msg;
};

#endif // !__CENTRALISEUR_EXCEPTION__ 