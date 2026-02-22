$WshShell = New-Object -comObject WScript.Shell
$Desktop = [Environment]::GetFolderPath("Desktop")

$Shortcut = $WshShell.CreateShortcut("$Desktop\E-Series Calculator.lnk")
$Shortcut.TargetPath = "$PSScriptRoot\E-Series-Calculator.exe"
$Shortcut.IconLocation = "$PSScriptRoot\resistor_icon.ico"
$Shortcut.Save()

Write-Host "Desktop shortcut created!"
Pause