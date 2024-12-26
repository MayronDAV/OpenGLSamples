#pragma once

#include "core/application.h"

extern glb::application* glb::create_application();


int main(int p_argc, char** p_argv)
{
    auto app = glb::create_application();
	app->run();
	delete app;
}