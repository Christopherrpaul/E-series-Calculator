E-SERIES CALCULATOR (Win32)

A lightweight Windows GUI application for finding the nearest standard E-series value (E3–E192).

---

FEATURES

* Supports E3, E6, E12, E24, E48, E96, E192 series
* Calculates nearest standard value
* Displays percentage error
* Supports engineering notation (e.g. 1e3, 4.7E2)
* Native Win32 application (no dependencies)

---

QUICK START

1. Open this folder

2. Run the program:
   Double-click:
   E-Series-Calculator.exe

---

CREATE A DESKTOP ICON (RECOMMENDED)

To create a proper desktop shortcut with the resistor icon:

1. Right-click:
   CreateDesktopIcon.ps1

2. Click:
   Run with PowerShell

This will create a desktop icon named:
E-Series Calculator

Double-click that icon to launch the program.

---

IF WINDOWS BLOCKS THE SCRIPT

You may see a security warning.

If so:

1. Click "More info"
2. Click "Run anyway"

OR:

Open PowerShell and run:

Set-ExecutionPolicy -Scope Process -ExecutionPolicy Bypass

Then run the script again.

---

FILES INCLUDED

E-Series-Calculator.exe   - Main application
resistor_icon.ico         - Icon used for shortcut
CreateDesktopIcon.ps1     - Creates desktop shortcut
src\                      - Source code

---

BUILDING FROM SOURCE (OPTIONAL)

Requires MinGW (GCC) and windres.

Commands:

windres app.rc app_res.o
gcc E_Series.c app_res.o -o E-Series-Calculator.exe -lgdi32 -mwindows

---

NOTES

* No installation required
* No external dependencies
* Works on Windows 10/11
* Fully portable

---

AUTHOR

E-Series engineering
