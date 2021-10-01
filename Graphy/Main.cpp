#include "Core/GrPch.h"
#include "Application/TreeEditor.h"
#include "Application/MainMenu.h"

class Graphy : public Application
{
public:
	Graphy(const wchar_t* title, u32 width = 1600, u32 height = 900, u32 bufferWidth = 320, u32 bufferHeight = 200, int flags = WS_OVERLAPPED) : 
		Application(title, width, height, bufferWidth, bufferHeight, flags) {
		
		setLayer(new MainMenu);
	}
};

int main()
{
	Application* app = new Graphy(L"Graphy", 1600U, 900U, 570U, 320U, WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME);
	app->Run();
}