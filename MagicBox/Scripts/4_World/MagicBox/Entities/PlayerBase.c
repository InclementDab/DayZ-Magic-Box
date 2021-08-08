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
				
				return -1;
			}
			
			case MagicBoxCurrencyType.TRADER: {
				
				return -1;
			}
			
			case MagicBoxCurrencyType.NONE: {
				return int.MAX;
			}
		}
		
		Error("[MagicBox] Invalid currency type " + typename.EnumToString(MagicBoxCurrencyType, m_MagicBoxCurrencyType));
		return -1;
	}
	
	void DeductMagicBoxCurrency(int currency)
	{
	}
}