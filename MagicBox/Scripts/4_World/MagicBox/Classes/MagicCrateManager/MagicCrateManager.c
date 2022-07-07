class MagicCrateManager: JMModuleBase
{	
	static MagicCrateManager GetInstance()
	{
		return MagicCrateManager.Cast(GetModuleManager().GetModule(MagicCrateManager));
	}
	
	//static const string SETTINGS_FILE = "$mission:MagicBox\\Settings.json";
	static const string SETTINGS_FILE = "$profile:MagicCrateManagerSettings.json";
	
	protected ref array<MagicBoxProxy> m_PossibleCrates = {};
	protected MagicBox m_CurrentCrate;
	protected ref MagicCrateManagerSettings m_MagicCrateManagerSettings;
		
	void SetCurrentCrate(MagicBox crate)
	{
		m_CurrentCrate = crate;
	}
		
	MagicBox GetCurrentCrate()
	{
		return m_CurrentCrate;
	}
	
	override void OnMissionStart()
	{
		// Idk why this is being called but it is and its crashing some people
		if (GetGame().IsClient() && GetGame().IsMultiplayer()) {
			return;
		}
		
		/*
		if (!MakeDirectory("$mission:MagicBox")) {
			Error("Failed to create MagicBox settings directory");
			return;
		}*/
		
		if (!FileExist(SETTINGS_FILE)) {
			CopyFile("MagicBox\\Scripts\\Data\\MagicCrateManagerSettings.json", SETTINGS_FILE);
		}
		
		m_MagicCrateManagerSettings = new MagicCrateManagerSettings();
		JsonFileLoader<MagicCrateManagerSettings>.JsonLoadFile(SETTINGS_FILE, m_MagicCrateManagerSettings);
		
		// Create all possible crate locations
		foreach (MagicBoxLocation box_location: m_MagicCrateManagerSettings.PossibleBoxPositions) {
			MagicBoxProxy crate = MagicBoxProxy.Cast(GetGame().CreateObject("MagicBoxProxy", box_location.Position));
			crate.SetOrientation(box_location.Orientation);
			crate.SetPosition(box_location.Position);
			crate.Update();
			m_PossibleCrates.Insert(crate);
		}
		
		// assign a totally random crate on mission start
		if (m_PossibleCrates.Count() == 1) {
			// Edge case bug with the logic that selects a new crate requires me to force this to set
			m_CurrentCrate = TurnProxyIntoCrate(m_PossibleCrates[0]);
		} else {
			ShuffleNewCrate(null);
		}
	}
	
	override void OnMissionFinish()
	{
		foreach (MagicBoxProxy crate: m_PossibleCrates) {
			GetGame().ObjectDelete(crate);
		}
		
		GetGame().ObjectDelete(m_CurrentCrate);
	}
	
	override void OnInvokeConnect(PlayerBase player, PlayerIdentity identity)
	{
		player.SetMagicBoxCurrencyType(m_MagicCrateManagerSettings.GetMagicCrateExchangeType());
	}
	
	MagicBox ShuffleNewCrate(MagicBox current_crate)
	{
		vector current_position = vector.Zero;
		if (current_crate) {
			current_position = current_crate.GetPosition();
		}
		
		// if there wont be any crates to find. dont need to be stuck in a while loop
		if (m_PossibleCrates.Count() < 2) {
			return current_crate;
		}
		
		if (current_crate) {
			TurnCrateIntoProxy(current_crate);
		}
		
		MagicBoxProxy new_proxy = GetRandomSafeCrate(current_position);
		if (!new_proxy) {
			Error("Failed to find new suitable crate location");
			return null;
		}
		
		m_CurrentCrate = TurnProxyIntoCrate(new_proxy);
		return m_CurrentCrate;
	}
	
	MagicBoxProxy GetRandomSafeCrate(vector pos_to_check)
	{
		// Grab new position that isnt the old position
		for (int i = 0; i < 100; i++) {
			MagicBoxProxy random_loc = m_PossibleCrates.GetRandomElement();
			if (vector.Distance(random_loc.GetPosition(), pos_to_check) > 0.5) {
				return random_loc;
			}
		}
		
		return null;
	}
	
	MagicBox TurnProxyIntoCrate(MagicBoxProxy crate)
	{
		MagicBox new_crate = MagicBox.Cast(GetGame().CreateObject("MagicBox", crate.GetPosition()));
		new_crate.SetPosition(crate.GetPosition());
		new_crate.SetOrientation(crate.GetOrientation());
		new_crate.SetProxyObject(crate);
		new_crate.SetCostToOpen(m_MagicCrateManagerSettings.MagicCratePrice);
		new_crate.Update();
		crate.SetPosition(crate.GetPosition() - "0 1000 0");
		crate.Update();
		return new_crate;
	}
	
	MagicBoxProxy TurnCrateIntoProxy(MagicBox crate)
	{
		if (!crate || !crate.GetProxyObject()) {
			return null;
		}

		MagicBoxProxy proxy = crate.GetProxyObject();
		proxy.SetPosition(proxy.GetPosition() + "0 1000 0");
		proxy.Update();
		GetGame().ObjectDelete(crate);
		return proxy;
	}
		
	MagicCrateManagerSettings GetSettings()
	{
		return m_MagicCrateManagerSettings;
	}
	
	override bool IsServer()
	{
		return true;
	}
	
	override bool IsClient()
	{
		return false;
	}
}