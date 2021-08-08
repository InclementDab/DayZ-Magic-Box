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
	int GetMagicBoxCurrency()
	{
		switch (m_MagicBoxCurrencyType) {
			case MagicBoxCurrencyType.EXPANSION: {
				JMModuleBase expansion_market = GetExpansionMarketModule();
				if (!expansion_market) {
					Error("[MagicBox] Expansion Market not found");
					break;
				}
				
				// cursed_monies
				TIntArray money();
				Param2<PlayerBase, TIntArray> player_worth_params(this, money);
				int expansion_market_currency;
				g_Script.CallFunctionParams(expansion_market, "GetPlayerWorth", expansion_market_currency, player_worth_params);				
				return expansion_market_currency;
			}
			
			case MagicBoxCurrencyType.TRADER: {
				// Trader mod function
				int value;
				g_Script.CallFunction(this, "getPlayerCurrencyAmount", value, null);
				return value;
			}
			
			case MagicBoxCurrencyType.NONE: {
				return int.MAX;
			}
		}
		
		Error("[MagicBox] Invalid currency type " + typename.EnumToString(MagicBoxCurrencyType, m_MagicBoxCurrencyType));
		return -1;
	}
	
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
				
				int removed;
				g_Script.CallFunction(expansion_market, "RemoveMoney", removed, this);
				Print(removed - amount);
				if (removed - amount > 0) {
					g_Script.CallFunctionParams(expansion_market, "SpawnMoney", null, new Param3<PlayerBase, EntityAI, int>(this, this, removed - amount));
					g_Script.CallFunctionParams(expansion_market, "CheckSpawn", null, new Param2<PlayerBase, EntityAI>(this, this));
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