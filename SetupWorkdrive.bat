@echo off

cd /D "%~dp0"

IF exist "P:\MagicBox\" (
	echo Removing existing link P:\MagicBox
	rmdir "P:\MagicBox\"
)

echo Creating link P:\MagicBox
mklink /J "P:\MagicBox\" "%cd%\MagicBox\"

echo Done