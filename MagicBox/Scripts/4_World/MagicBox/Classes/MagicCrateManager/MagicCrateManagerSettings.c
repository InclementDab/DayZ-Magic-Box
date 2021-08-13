class MagicCrateManagerSettings
{
	// EXPANSION, TRADER, NONE
	string MagicCrateExchangeType = "NONE";
	
	// only valid for EXPANSION and TRADER types
	int MagicCratePrice = 950;
	
	bool CanCrateChangeLocation = true;
	int MinimumRollBeforeFailChance = 5;
	int BoxFailChance = 20;
	ref array<ref MagicBoxLocation> PossibleBoxPositions = {
		/*new MagicBoxLocation("8812.011719 88.768845 5389.596680", "0.000000 -0.000000 -0.000000"),
		new MagicBoxLocation("8815.119141 88.768814 5390.998047", "-63.912487 -0.000000 -0.00000"),
		new MagicBoxLocation("8809.294922 88.768845 5392.334473", "98.406143 -0.000000 -0.000000"),*/
	};
	
		// DONT do optics on this stuff, most of its broken
	ref array<ref CrateWeaponEntry> PossibleBoxItems = {
		/*new CrateWeaponEntry("M4A1", {
			"M4_MPBttstck", 
			"M4_MPHndgrd", 
			"M4_CarryHandleOptic",
		}, "Mag_STANAG_30Rnd"), 
		new CrateWeaponEntry("AK101", {
			"AK_WoodBttstck", 
			"AK_WoodHndgrd",
		}, "Mag_AK101_30Rnd"), 
		new CrateWeaponEntry("AKM", {
			"AK_WoodBttstck", 
			"AK_WoodHndgrd",
		}, "Mag_AKM_30Rnd"),
		new CrateWeaponEntry("AKS74U", {
			"AK_WoodBttstck", 
			"AK_WoodHndgrd",
		}, "Mag_AK74_30Rnd"), 
		new CrateWeaponEntry("FAL", {
			"Fal_OeBttstck",  
		}, "Mag_FAL_20Rnd"),
		new CrateWeaponEntry("MP5K", {
			"MP5k_StockBttstck", 
		}, "Mag_MP5_30Rnd"),
		new CrateWeaponEntry("Saiga", {
			"Saiga_Bttstck", 
		}, "Mag_Saiga_8Rnd"),
		new CrateWeaponEntry("Scout"),
		new CrateWeaponEntry("SVD"),
		new CrateWeaponEntry("ASVAL"),
		new CrateWeaponEntry("VSS"),
		new CrateWeaponEntry("Colt1911"),
		new CrateWeaponEntry("MakarovIJ70"), 
		new CrateWeaponEntry("FNX45"),
		new CrateWeaponEntry("Izh18"),
		new CrateWeaponEntry("Winchester70"),
		new CrateWeaponEntry("Mosin9130"), 
		new CrateWeaponEntry("SKS"),
		new CrateWeaponEntry("Izh43Shotgun"),
		new CrateWeaponEntry("Flaregun"),
		new CrateWeaponEntry("M67Grenade"),*/
	};
	
	MagicBoxCurrencyType GetMagicCrateExchangeType()
	{
		string currency_type = MagicCrateExchangeType;
		currency_type.ToLower();
		
		switch (currency_type) {
			case "none": return MagicBoxCurrencyType.NONE;
			case "expansion": return MagicBoxCurrencyType.EXPANSION;
			case "trader": return MagicBoxCurrencyType.TRADER;
		}
		
		Error("[MagicBox] Invalid currency type " + currency_type);
		return -1;
	}
}