#ifndef PERSISTENCEMACROS_H
#define PERSISTENCEMACROS_H

#include <string>
#include <map>


#define DECLARE_RUNTIME_CLASS \
private: \
	static const std::string m_class_name; \
public:  \
	virtual bool IsOfType(const std::string&);


#define IMPLEMENT_RUNTIME_CLASS(class_name) \
const std::string class_name##::m_class_name = #class_name; \
bool class_name##::IsOfType(const std::string& name) \
{ \
	return (name == m_class_name); \
}

#endif