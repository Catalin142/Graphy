#include "Core/GrPch.h"
#include "Application/TheoryTab.h"
#include "Application/MainMenu.h"

class Graphy : public Application
{
public:
	Graphy(const wchar_t* title, u32 width = 1600, u32 height = 900, u32 bufferWidth = 320, u32 bufferHeight = 200, int flags = WS_OVERLAPPED) : 
		Application(title, width, height, bufferWidth, bufferHeight, flags) 
	{
		setLayer(new MainMenu);
		TheoryTab::loadTheory("Resources/Editor/Tips.txt");
		Layer::m_BufferDim = { (float)getBuffer()->getWidth(), (float)getBuffer()->getHeight() };
	}
};

int main()
{
	Application* app = new Graphy(L"Graphy", 1600U, 900U, 570U, 320U, WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME);
	app->Run();
}