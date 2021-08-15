modded class PlayerBase
{	
	static const int MAGIC_BOX_SET_CURRENCY_TYPE = 84363;
	
	protected MagicBoxCurrencyType m_MagicBoxCurrencyType = MagicBoxCurrencyType.NONE;
	
	void PlayerBase()
	{
		RegisterNetSyncVariableInt("m_MagicBoxCurrencyType");
	}
		
	void SetMagicBoxCurrencyType(MagicBoxCurrencyType currency_type)
	{
		if (!GetGame().IsServer()) {
			Error("SetMagicBoxCurrencyType must be called on server");
			return;
		}
		
		m_MagicBoxCurrencyType = currency_type;
		SetSynchDirty();
	}
	
	MagicBoxCurrencyType GetMagicBoxCurrencyType()
	{
		return m_MagicBoxCurrencyType;
	}
	
	// These are wrappers so we dont NEED mods like trader for the crate to work
	// it also means that you can do things like Expansion, or whatever!
	// needs to work on client and server
	int GetMagicBoxCurrency()
	{
		int amount;
		switch (m_MagicBoxCurrencyType) {
			case MagicBoxCurrencyType.EXPANSION: {
				JMModuleBase expansion_market = GetExpansionMarketModule();
				if (!expansion_market) {
					Error("[MagicBox] Expansion Market not found");
					break;
				}
				
				// cursed_monies
				TIntArray money = {};
				Param2<PlayerBase, TIntArray> player_worth_params(this, money);
				g_Script.CallFunctionParams(expansion_market, "GetPlayerWorth", amount, player_worth_params);				
				return amount;
			}
			
			case MagicBoxCurrencyType.TRADER: {
				// Trader mod function
				g_Script.CallFunction(this, "getPlayerCurrencyAmount", amount, null);
				return amount;
			}
			
			case MagicBoxCurrencyType.TRADERPLUS: {
				
				typename trader_plus_helper_type = String("TraderPlusHelper").ToType();
				if (!trader_plus_helper_type) {
					Error("TraderPlus not found!");
					break;
				}
				
				Class trader_plus_helper = trader_plus_helper_type.Spawn();
				
				Param2<PlayerBase, int> trader_plus_params(this, 0);
				g_Script.CallFunctionParams(trader_plus_helper, "GetPlayerMoney", amount, trader_plus_params);
				Print(amount);
				
				return amount;
			}
			
			case MagicBoxCurrencyType.NONE: {
				return int.MAX;
			}
		}
		
		Error("[MagicBox] Invalid currency type " + typename.EnumToString(MagicBoxCurrencyType, m_MagicBoxCurrencyType));
		return -1;
	}
	
	// only needs to work on server
	void DeductMagicBoxCurrency(int amount)
	{
		switch (m_MagicBoxCurrencyType) {
			case MagicBoxCurrencyType.EXPANSION: {
				JMModuleBase expansion_market = GetExpansionMarketModule();
				if (!expansion_market) {
					Error("[MagicBox] Expansion Market not found");
					break;
				}
				
				bool success;
				TIntArray money();
				Param4<PlayerBase, int, TIntArray, bool> find_params(this, amount, money, true);
				g_Script.CallFunctionParams(expansion_market, "FindMoneyAndCountTypesEx", success, find_params);
				if (!success) {
					Error("[MagicBox] FindMoneyAndCountTypes failed! (oh no)");
					break;
				}
				
				array<ItemBase> spawned_money = {};
				int removed;
				g_Script.CallFunction(expansion_market, "RemoveMoney", removed, this);
				EntityAI parent = this;
				if (removed - amount > 0) {
					Param3<PlayerBase, EntityAI, int> spawn_params(this, parent, removed - amount);
					g_Script.CallFunctionParams(expansion_market, "SpawnMoneyEx", spawned_money, spawn_params);
				}
				
				break;
			}
			
			case MagicBoxCurrencyType.TRADER: {
				// Trader mod function
				g_Script.CallFunction(this, "deductPlayerCurrency", null, amount);
				break;
			}
		}		
	}
	
	JMModuleBase GetExpansionMarketModule()
	{
		typename expansion_market = String("ExpansionMarketModule").ToType();
		if (!expansion_market) {
			return null;
		}
		
		JMModuleBase module = GetModuleManager().GetModule(expansion_market);
		if (!module) {
			return null;
		}
		
		return module;
	}
}