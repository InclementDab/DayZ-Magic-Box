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
				typename expansion_market = String("ExpansionMarketModule").ToType();
				if (!expansion_market) {
					Error("[MagicBox] Expansion not running");
					return -1;
				}
				
				JMModuleBase module = GetModuleManager().GetModule(expansion_market);
				if (!module) {
					Error("[MagicBox] Expansion Market not found");
					return -1;
				}
				
				TIntArray money();
				Param2<PlayerBase, TIntArray> player_worth_params(this, money);
				int expansion_market_currency;
				g_Script.CallFunctionParams(module, "GetPlayerWorth", expansion_market_currency, player_worth_params);				
				return expansion_market_currency;
			}
			
			case MagicBoxCurrencyType.TRADER: {
				int value;
				// Trader mod function
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
				
				break;
			}
			
			case MagicBoxCurrencyType.TRADER: {
				// Trader mod function
				g_Script.CallFunction(this, "deductPlayerCurrency", null, amount);
				break;
			}
		}		
	}
}