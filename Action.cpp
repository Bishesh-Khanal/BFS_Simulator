#include "Action.h"

Action::Action()
{
}

Action::Action(const std::string& name, const std::string& type, const Vec2& pos)
	: m_name(name)
	, m_type(type)
	, m_mousePos(pos)
{
}

const std::string& Action::name() const
{
	return m_name;
}

const std::string& Action::type() const
{
	return m_type;
}

const Vec2& Action::pos() const
{
	return m_mousePos;
}
