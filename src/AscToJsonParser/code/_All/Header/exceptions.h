#include <exception>
#include <string>

class DisconnectedException : public std::exception{
public:
	explicit DisconnectedException(const char* message) : m_msg(message){}
	explicit DisconnectedException(const std::string& message) : m_msg(message){}
	virtual ~DisconnectedException() throw(){}
	virtual const char* what() const throw(){
		return m_msg.c_str();
	}
protected:
	std::string m_msg;
};