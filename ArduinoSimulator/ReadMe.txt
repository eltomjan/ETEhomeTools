========================================================================
    APPLICATION : ArduinoSimulator Project Overview
========================================================================

This is a VS2008 project using Arduino aprox. 1.6.3, it has virtual ILI9340 LCD.
It can be used to examine almost unchanged Arduino code.
There is also Cage.cpp/h demo made from existing INO, there are also 2 big XOR packed fonts
(+small symbol bitmaped) and quick print made by improvement in Adafruit_GFX/ILI9340 libs.
ArduinoSimulatorEXE.zip contains compiled app build on W7.

This file contains a summary of what you will find in each of the files that
make up your ArduinoSimulator application.

ArduinoSimulator.vcproj
    This is the main project file for VC++ projects generated using an Application Wizard. 
    It contains information about the version of Visual C++ that generated the file, and 
    information about the platforms, configurations, and project features selected with the
    Application Wizard.

Cage.cpp
    This is the main Arduino application source file.
    Contains the code to display the form.

ArduinoWindow.h
    Contains the implementation of .NET form class and InitializeComponent() function.

AssemblyInfo.cpp
    Contains custom attributes for modifying assembly metadata.
