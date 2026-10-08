@echo off
echo =================================================================
echo   Splatoon 1 (Wii U - Gambit PC) - DirectX 11 3D Map Renderer
echo =================================================================
echo Controls:
echo   - WASD:                Move Inkling Player (Camera-Relative)
echo   - Mouse / Arrows:      Aim ^& Look in 3D (Yaw ^& Pitch)
echo   - Space:               Jump with Hang Time
echo   - Shift:               Submerge in Squid Form (Fast swim in ink!)
echo   - Left Click:          Fire Splattershot 3D Ink Droplets
echo   - Right Click / R:     Throw Splat Bomb (Parabolic Arc)
echo   - 1 / 2:               Switch Ink Team (Orange ^<--^> Cyan)
echo   - TAB / M:             Switch Stage (Inkopolis Plaza ^<--^> Walleye Warehouse)
echo   - C:                   Toggle Camera (3rd-Person Follow ^<--^> Turntable)
echo   - ESC:                 Close Window
echo =================================================================
"%~dp0build\Release\Gambit.exe" %*
if %errorlevel% neq 0 (
    echo.
    echo Renderer exited with code %errorlevel%.
    pause
)
