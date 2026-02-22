E-SERIES CALCULATOR – INSTALLATION GUIDE

------------------------------------------------------------
SINGLE RECOMMENDED INSTALLATION METHOD
------------------------------------------------------------

1. Download the project from GitHub:
   - Click "Code" → "Download ZIP"

2. Extract the ZIP file:
   - Right-click the ZIP → "Extract All"

3. Move the entire extracted folder to a permanent location:

   Recommended location:
       C:\Users\<your name>\Documents\E-Series-Calculator

4. Open the folder

5. Run:
       CreateDesktopIcon.ps1


------------------------------------------------------------
CREATE A DESKTOP ICON (USING SCRIPT)
------------------------------------------------------------

1. Right-click:
       CreateDesktopIcon.ps1

2. Select:
       Run with PowerShell

3. Allow it to run if prompted

4. A desktop icon will be created automatically


 
------------------------------------------------------------
MOVING THE PROGRAM – FILE VS FOLDER
------------------------------------------------------------

Although it is possible to move only the .exe file, this is NOT recommended. Instead, 
   Move the entire folder. 
   Delete the desktop shortcut.
	Repeat the procedure under CREATE A DESKTOP ICON (USING SCRIPT)


------------------------------------------------------------
FILES NEEDED ONLY FOR RECOMPILING
------------------------------------------------------------

These files are only needed if modifying the program:

    E_Series.c
    app.rc
    resource.h
    resistor_icon.ico

They are NOT required to run the application.

Note:
    ".gitignore" has no functional purpose for running
    or rebuilding the program. It is a leftover Git file
    and may be deleted.




---

AUTHOR

Christopher Paul


