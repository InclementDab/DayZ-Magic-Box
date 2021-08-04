modded class PlayerBase
{
	// These are wrappers so we dont NEED mods like trader for the crate to work
	// it also means that you can do things like Expansion, or whatever!
	int GetMagicBoxCurrency()
	{
		return 1000;
	}
	
	void DeductMagicBoxCurrency(int currency)
	{
	}
}