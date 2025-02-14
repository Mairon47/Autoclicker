Raw-Input AutoClicker (Toggled by Mouse5)
A small Win32 application that uses raw input to detect Mouse5 (extra mouse button) clicks, toggling an auto-clicking thread on and off. A simple GUI indicates whether the autoclicker is active or inactive.

How It Works

Raw Input Registration:
The application registers for raw mouse input (RIDEV_INPUTSINK), so it can detect Mouse5 clicks even if the window is not focused.

Toggle on Mouse5:
In the WM_INPUT message, if the code detects RI_MOUSE_BUTTON_5_DOWN, it flips a global atomic flag isAutoClicking.

AutoClick Thread:
When toggled on, a separate thread repeatedly sends left mouse clicks (MOUSEEVENTF_LEFTDOWN / MOUSEEVENTF_LEFTUP) at a 1ms interval.
When toggled off, the thread stops.

Building
Open the .cpp file in Visual Studio (or another Win32-compatible C++ IDE).
Compile as a Windows application (e.g. wWinMain).
Run the resulting .exe.

Usage
Start/Stop: Click the “Inactive” button (or press Mouse5) to toggle. It becomes “Active” when auto-clicking is on.
Exit: Press the “Exit” button or close the window.

Notes
Administrator Privileges: Some contexts or games may require you to run the program as admin for it to work properly.
Anti-Cheat: Many games block synthetic mouse events. This tool may not work in those scenarios.
Responsibility: Use responsibly. This is a demonstration for how to set up raw input and basic autoclicker functionality.

Enjoy using (and learning from) this simple raw-input autoclicker code!
