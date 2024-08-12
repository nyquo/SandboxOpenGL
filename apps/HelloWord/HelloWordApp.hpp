#pragma once

#include <Application.hpp>
#include <Layer.hpp>
#include <Events/Event.hpp>
#include <Logger.hpp>

class HelloWordLayer : public core::Layer {

public:
	void onUpdate() override {
		//core::Logger::logDebug("Update custom layer!");

		//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		//GLenum error = glGetError();
		//core::Logger::logError(error);

		////glClearColor(0.008f, 0.082f, 0.149f, 1.0f);

		//glClear(GL_COLOR_BUFFER_BIT);
		//error = glGetError();
		//core::Logger::logError(error);
	}


	void onEvent(core::Event& event) override {

	}
};

class HelloWordApp : public core::Application {
public:
	HelloWordApp() {
		getWindow().setLayer(new HelloWordLayer());
	}

};
