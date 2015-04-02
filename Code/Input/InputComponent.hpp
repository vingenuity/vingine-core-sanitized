#ifndef INCLUDED_INPUT_COMPONENT_HPP
#define INCLUDED_INPUT_COMPONENT_HPP

//-----------------------------------------------------------------------------------------------
#include <vector>

#include "../Component.hpp"
#include "Action.hpp"

//-----------------------------------------------------------------------------------------------
struct InputComponent : public Component
{
	InputComponent() { }

	std::vector< Action > m_queuedActions;
};

#endif //INCLUDED_INPUT_COMPONENT_HPP
