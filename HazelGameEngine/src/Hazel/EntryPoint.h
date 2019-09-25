#pragma once
#ifdef HZ_PLATFORM_WINDOWS

extern Hazel::Application* Hazel::CreateApplication();

//Move Entry point to Engine
int main(int argc, char** argv)
{
	printf("Hazel Engine Start!");
	auto app = Hazel::CreateApplication();
	app->Run();
	delete app;
	return 0;
}
#endif