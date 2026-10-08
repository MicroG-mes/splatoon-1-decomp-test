@echo off
set "JAVA_HOME=C:\Program Files\Microsoft\jdk-21.0.12.101-hotspot"
set "PATH=%JAVA_HOME%\bin;%PATH%"
echo =================================================================
echo   Launching Ghidra 12.1.4 (NSA Reverse Engineering Suite)
echo =================================================================
call "C:\Users\manel\ghidra\ghidra_12.1.4_PUBLIC\ghidraRun.bat"
