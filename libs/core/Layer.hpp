#pragma once

#include "Events/Event.hpp"

namespace core {

	class Layer {
	public:
		virtual void onUpdate() = 0;
		virtual void onEvent(Event& event) = 0;
	};

}