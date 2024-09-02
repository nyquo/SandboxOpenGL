#pragma once

#include "CoreExport.hpp"

#include "Events/Event.hpp"

namespace core {

	class CORE_API Layer {
	public:
		virtual void onUpdate() = 0;
		virtual void onEvent(Event& event) = 0;
	};

}