// ArduinoSimulator.cpp : main project file.

#include "ArduinoWindow.h"

using namespace ArduinoSimulator;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// Create the main window and run it
	Application::Run(gcnew ArduinoWindow());
	return 0;
}
