@echo off
set GRADLE_VERSION=8.9
where gradle >nul 2>nul
if %ERRORLEVEL% EQU 0 (
  gradle %*
  exit /b %ERRORLEVEL%
)
echo Gradle is not installed. Run this project through GitHub Actions or install Gradle 8.9.
exit /b 1
