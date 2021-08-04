class CfgPatches
{
	class MagicCrate_Models
	{
		requiredVersion=0.1;
		requiredAddons[]={"DZ_Data"};
	};
};

class CfgVehicles
{
    class HouseNoDestruct;
	class Inventory_Base;
	class MagicBoxProxy: HouseNoDestruct
	{
		scope = 2;
		displayName = "Magic Crate Proxy";
		descriptionShort = "Magic Crate Proxy";
		model = "\MagicBox\Models\magic_box\magic_box_proxy.p3d";
	};

    class MagicBox: Inventory_Base
	{
		scope = 2;
		displayName = "Magic Crate";
		descriptionShort = "Magic Crate";
		model = "\MagicBox\Models\magic_box\magic_box.p3d";
		carveNavmesh = 1;
		heavyItem = 1;
		weight = 10000;
		itemSize[] = {6,3};
		itemBehaviour = 0;
		absorbency = 0;
		rotationFlags = 2;
		lootTag[] = {"Farm","Forester"};
		physLayer = "item_large";
		hiddenSelections[] = {""};
		storageCategory = SC_building;
		hiddenSelectionsTextures[] = {"\MagicBox\Models\magic_box\data\magic_box_co.paa","\MagicBox\Models\magic_box\data\magic_box_co.paa","\MagicBox\Models\magic_box\data\magic_box_co.paa"};
		hiddenSelectionsMaterials[] = {"\MagicBox\Models\magic_box\data\magic_box.rvmat","\MagicBox\Models\magic_box\data\magic_box.rvmat","\MagicBox\Models\magic_box\data\magic_box.rvmat"};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 100;
					healthLevels[] = {{1.0,{"\MagicBox\Models\magic_box\data\magic_box.rvmat"}},{0.7,{"\MagicBox\Models\magic_box\data\magic_box.rvmat"}},{0.5,{"\MagicBox\Models\magic_box\data\magic_box_damage.rvmat"}},{0.3,{"\MagicBox\Models\magic_box\data\magic_box_damage.rvmat"}},{0.0,{"\MagicBox\Models\magic_box\data\magic_box_destruct.rvmat"}}};
				};
			};
			class GlobalArmor
			{
				class Projectile
				{
					class Health
					{
						damage = 0;
					};
					class Blood
					{
						damage = 0;
					};
					class Shock
					{
						damage = 0;
					};
				};
			};
		};
		class AnimationSources
		{
			class lid
			{
				source = "user";
				initPhase = 0;
				animPeriod = 0.5;
			};
		};
		class AnimEvents
		{
			class SoundWeapon
			{
				class movement
				{
					soundSet = "seachest_movement_SoundSet";
					id = 1;
				};
				class pickUpItem_Light
				{
					soundSet = "pickUpSeaChest_SoundSet";
					id = 796;
				};
				class pickUpItem
				{
					soundSet = "pickUpSeaChest_SoundSet";
					id = 797;
				};
				class drop
				{
					soundset = "seachest_drop_SoundSet";
					id = 898;
				};
			};
		};
	};

};

class CfgSoundShaders
{
	class Box_SoundShaderBase
	{
		volume = 1;
		frequency = 1;
		range = 10;
		radius = 25;
		limitation = 0;
	};

	class BoxJingle_SoundShader: Box_SoundShaderBase
	{
		samples[] = {{"MagicBox\Models\magic_box\magic_box_open", 1}};
	};

	class BoxFail_SoundShader: Box_SoundShaderBase
	{
		volume = 5;
		samples[] = {{"MagicBox\Models\magic_box\magic_box_fail", 1}};
	};

	class BoxOpen_SoundShader: Box_SoundShaderBase
	{
		samples[] = {{"DZ\sounds\environment\buildings\doors\wood\doorWoodSmallOpenABit_4", 1}};
	};

	class BoxClose_SoundShader: Box_SoundShaderBase
	{
		samples[] = {{"DZ\sounds\environment\buildings\doors\wood\doorWoodClose_06", 1}};
	};
};

