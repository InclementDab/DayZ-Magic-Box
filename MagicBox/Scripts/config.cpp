
class CfgPatches
{
	class MagicBox_Scripts
	{
        units[] = {};
        weapons[] = {};
        requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Scripts", "JM_CF_Scripts", "DF_Scripts"};
	};
};

class CfgMods 
{
	class DZ_MagicBox
	{
		name = "DayZ Magic Box";
		dir = "MagicBox";
		credits = "InclementDab & Zeroy";
		author = "InclementDab";
		creditsJson = "MagicBox/Scripts/Data/Credits.json";
		versionPath = "MagicBox/Scripts/Data/Version.hpp";
		inputs = "MagicBox/Scripts/Data/Inputs.xml";
		type = "mod";
		dependencies[] =
		{
			"Game", "World", "Mission"
		};
		class defs
		{
			class imageSets
			{
				files[]= {};
			};
			class engineScriptModule
			{
				value = "";
				files[] =
				{
					"MagicBox/scripts/common",
					"MagicBox/scripts/1_core"
				};
			};

			class gameScriptModule
			{
				value="";
				files[] = 
				{
					"MagicBox/scripts/common",
					"MagicBox/scripts/3_Game"
				};
			};
			class worldScriptModule
			{
				value="";
				files[] = 
				{
					"MagicBox/scripts/common",
					"MagicBox/scripts/4_World"
				};
			};

			class missionScriptModule 
			{
				value="";
				files[] = 
				{
					"MagicBox/scripts/common",
					"MagicBox/scripts/5_Mission"
				};
			};
		};
	};
};
