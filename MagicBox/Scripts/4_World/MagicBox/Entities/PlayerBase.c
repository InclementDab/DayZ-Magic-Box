modded class PlayerBase
{	
	static const int MAGIC_BOX_SET_CURRENCY_TYPE = 84363;
	
	protected string m_MagicBoxCurrencyType = "NONE";
	
	void PlayerBase()
	{
	}
	
	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, rpc_type, ctx);
		
		switch (rpc_type) {
			case MAGIC_BOX_SET_CURRENCY_TYPE: {
				Param1<string> currency_params;
				if (!ctx.Read(currency_params)) {
					break;
				}
				
				m_MagicBoxCurrencyType = currency_params.param1;
			}
		}
	}
	
	void SetMagicBoxCurrencyType(string currency_type)
	{
		if (!GetGame().IsServer()) {
			Error("SetMagicBoxCurrencyType must be called on server");
			return;
		}
		
		m_MagicBoxCurrencyType = currency_type;
		GetGame().RPCSingleParam(this, MAGIC_BOX_SET_CURRENCY_TYPE, new Param1<string>(currency_type), true, GetIdentity());
	}
	
	// These are wrappers so we dont NEED mods like trader for the crate to work
	// it also means that you can do things like Expansion, or whatever!
	int GetMagicBoxCurrency()
	{
		switch (m_MagicBoxCurrencyType) {
			case "EXPANSION": {
				
				return -1;
			}
			
			case "TRADER": {
				
				return -1;
			}
			
			case "NONE": {
				return int.MAX;
			}
		}
		
		Error("[MagicBox] Invalid currency type " + m_MagicBoxCurrencyType);
		return -1;
	}
	
	void DeductMagicBoxCurrency(int currency)
	{
	}
}