#pragma once
#ifdef HZ_PLATFORM_WINDOWS

extern Hazel::Application* Hazel::CreateApplication();

//Move Entry point to Engine
int main(int argc, char** argv)
{
	printf("Hazel Engine Start!\n");
	Hazel::Log::Init();
	HZ_CORE_WARN("Initialized Log");
	int a = 100;
	HZ_INFO("App start: Var={0}" , a);
	auto app = Hazel::CreateApplication();
	app->Run();
	delete app;
	return 0;
}
#endif