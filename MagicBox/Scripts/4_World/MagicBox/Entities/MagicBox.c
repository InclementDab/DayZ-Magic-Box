class MagicBox: ItemBase
{
	static const int RPC_CRATE_OPEN = 84364;
	static const int RPC_CRATE_CLOSE = 84365;
	static const int RPC_CRATE_FAIL = 84366;
	
	static const float ANIMATION_DURATION = 5.5;
	static const float USER_WAIT_TIME = 12.0;

	protected bool m_IsOpening;
	protected int m_CostToOpen;
	protected ItemBase m_DisplayedWeapon;
	protected vector m_DisplayedWeaponPosition;
	protected int m_RollCount;
	
	protected ref Timer m_BoxTimer = new Timer(CALL_CATEGORY_GAMEPLAY);
	protected ref Timer m_WeaponSwapTimer = new Timer(CALL_CATEGORY_GAMEPLAY);
	
	protected ChemlightLight m_Light;
	protected Particle m_Aura;
	protected MagicBoxProxy m_HiddenProxyObject;
	
	void MagicBox()
	{
		if (IsMissionClient()) {
			m_Light = ChemlightLight.Cast(ScriptedLightBase.CreateLight(ChemlightLight));
			m_Light.AttachOnObject(this, GetWeaponRiseStartPosition() + "0.01 0 0"); 
			m_Light.SetColorToBlue();
			m_Light.SetCastShadow(false);
			
			m_Aura = Particle.PlayOnObject(ParticleList.MYSTERY_BOX_AURA, this);
		}
		
		RegisterNetSyncVariableBool("m_IsOpening");
		RegisterNetSyncVariableInt("m_CostToOpen");
		
		if (GetGame().IsServer()) {
			MagicCrateManager manager = MagicCrateManager.GetInstance();
			if (manager) {
				MagicCrateManagerSettings settings = manager.GetSettings();
				if (settings) {
					// update magic crate price always
					SetCostToOpen(settings.MagicCratePrice);
				}
			}
		}
	}
	
	void ~MagicBox()
	{
		if (m_Light) {
			m_Light.Destroy();
		}
		
		if (m_Aura && GetGame()) {
			m_Aura.Stop();	
			GetGame().ObjectDelete(m_Aura);
		}
				
		delete m_BoxTimer;
		delete m_WeaponSwapTimer;
	}
	
	void SetProxyObject(MagicBoxProxy proxy)
	{
		m_HiddenProxyObject = proxy;
	}
	
	MagicBoxProxy GetProxyObject()
	{
		return m_HiddenProxyObject;
	}
	
	// Server side
	bool TryOpenCrate(notnull PlayerBase player)
	{
		if (!GetGame().IsServer()) {
			return false;
		}
		
		if (player.GetMagicBoxCurrency() < m_CostToOpen) {
			return false;
		}
		
		if (m_IsOpening) {
			return false;
		}
		
		m_IsOpening = true;
		SetSynchDirty();
		
		// Resetting so we dont delete on next crate open
		m_DisplayedWeapon = null;
		
		SetAnimationPhase("lid", 1.0);
		m_BoxTimer.Run(0.01, this, "OnBoxTimer", new Param2<float, int>(ANIMATION_DURATION, GetGame().GetTime()), true);
		m_WeaponSwapTimer.Run(0.01, this, "WeaponSwapBoxTimer", new Param2<float, int>(ANIMATION_DURATION, GetGame().GetTime()), false);
		
		GetGame().RPCSingleParam(this, RPC_CRATE_OPEN, null, true);
		
		return true;
	}
	
	// Crate succeded
	void OnCrateSucceed(ItemBase item)
	{
		m_RollCount++;
		m_DisplayedWeapon.SetMysteryBoxState(MysteryBoxWeaponState.AWAITING_USER);
		m_BoxTimer.Run(0.01, this, "OnWaitTimer", new Param2<float, int>(USER_WAIT_TIME, GetGame().GetTime()), true);
	}
	
	// Crate failed
	void OnCrateFail()
	{
		GetGame().RPCSingleParam(m_DisplayedWeapon, RPC_CRATE_FAIL, null, true);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DelayRunFail, 6500);
	}
	
	private void DelayRunFail()
	{
		m_BoxTimer.Run(0.01, this, "OnFailTimer", new Param2<float, int>(5.0, GetGame().GetTime()), true);
	}
	
	void CloseCrate()
	{
		SetAnimationPhase("lid", 0.0);
		
		m_IsOpening = false;
		SetSynchDirty();
				
		// If the weapon was not taken, delete it
		if (m_DisplayedWeapon.GetMysteryBoxState() != MysteryBoxWeaponState.TAKEN) {
			GetGame().ObjectDelete(m_DisplayedWeapon);
		}
		
		// Resetting so we dont delete on next crate open
		m_DisplayedWeapon = null;
		
		GetGame().RPCSingleParam(this, RPC_CRATE_CLOSE, null, true);
	}
	
	void WeaponSwapBoxTimer(float duration, int start_time)
	{
		float tfactor = (1 / duration) * (GetGame().GetTime() - start_time) / 1000;		
		if (tfactor >= 1) {
			m_WeaponSwapTimer.Stop();
			return;
		}
			
		if (m_DisplayedWeapon) {
			m_DisplayedWeapon.Delete();
		}
		
		m_DisplayedWeapon = null;
		
		m_WeaponSwapTimer.Run(0.5 * tfactor, this, "WeaponSwapBoxTimer", new Param2<float, int>(duration, start_time), false);
	}
		
	void OnBoxTimer(float duration, int start_time)
	{
		float tfactor = (1 / duration) * (GetGame().GetTime() - start_time) / 1000;
		MagicCrateManagerSettings settings = MagicCrateManager.GetInstance().GetSettings();
		// timer is done, we can get out
		if (tfactor >= 1) {
			m_BoxTimer.Stop();
			// It failed
			if (Math.RandomIntInclusive(0, 100) < settings.BoxFailChance && settings.CanCrateChangeLocation && m_RollCount > settings.MinimumRollBeforeFailChance && settings.PossibleBoxPositions.Count() > 1) {
				GetGame().ObjectDelete(m_DisplayedWeapon);
				m_DisplayedWeapon = CreatePreviewItem("Bear_Pink", m_DisplayedWeaponPosition, GetOrientation() + Vector(180, 0, 0));
				OnCrateFail();
				return;
			}
			
			// Im replicating this here since if the item gets deleted the tick before success,
			// the user would get nothing
			while (!m_DisplayedWeapon) {
				m_DisplayedWeapon = CreatePreviewItem(settings.PossibleBoxItems.GetRandomElement(), m_DisplayedWeaponPosition, GetOrientation());
			}
			
			OnCrateSucceed(m_DisplayedWeapon);
			return;
		}
		
		m_DisplayedWeaponPosition = Math.SmoothLerpVector(GetWeaponRiseStartPosition(), GetWeaponRiseEndPosition(), tfactor);
		while (!m_DisplayedWeapon) {
			m_DisplayedWeapon = CreatePreviewItem(settings.PossibleBoxItems.GetRandomElement(), m_DisplayedWeaponPosition, GetOrientation());
		}
		
		m_DisplayedWeapon.SetPosition(m_DisplayedWeaponPosition);
		m_DisplayedWeapon.Update();
	}
	
	void OnFailTimer(float duration, int start_time)
	{
		float tfactor = (1 / duration) * (GetGame().GetTime() - start_time) / 1000;
		m_DisplayedWeaponPosition = Math.SmoothLerpVector(GetWeaponRiseEndPosition(), GetWeaponRiseEndPosition() + "0 10 0", tfactor);
		if (m_DisplayedWeapon) {
			m_DisplayedWeapon.SetPosition(m_DisplayedWeaponPosition);
			m_DisplayedWeapon.Update();
		}
		
		if (tfactor >= 1) {
			m_BoxTimer.Stop();
			CloseCrate();
			// make new crate!
			MagicCrateManager.GetInstance().ShuffleNewCrate(this);
		}
	}
	
	void OnWaitTimer(float duration, int start_time)
	{
		float tfactor = (1 / duration) * (GetGame().GetTime() - start_time) / 1000;
		m_DisplayedWeaponPosition = Math.SmoothLerpVector(GetWeaponRiseEndPosition(), GetWeaponRiseStartPosition(), tfactor);
		
		if (m_DisplayedWeapon) {
			if (m_DisplayedWeapon.GetMysteryBoxState() == MysteryBoxWeaponState.TAKEN) {
				m_BoxTimer.Stop();
				CloseCrate();
				return;
			}
			
			m_DisplayedWeapon.SetPosition(m_DisplayedWeaponPosition);
			m_DisplayedWeapon.Update();
		}
		
		if (tfactor >= 1) {
			m_BoxTimer.Stop();
			CloseCrate();
			return;
		}
	}

	ItemBase CreatePreviewItem(CrateWeaponEntry entry, vector position, vector orientation)
	{
		if (!AllowedToSpawnType(entry.Item)) {
			return null;
		}
		
		ItemBase item = entry.CreateEntry(position, orientation);
		if (!item) {
			Print("Could not create item " + entry.Item);
			return item;
		}
		
		item.SetMysteryBoxState(MysteryBoxWeaponState.PREVIEW);
		return item;
	}
	
	ItemBase CreatePreviewItem(string type, vector position, vector orientation)
	{
		if (!AllowedToSpawnType(type)) {
			return null;
		}
		
		ItemBase item = ItemBase.Cast(GetGame().CreateObject(type, position, false, false, false));
		if (!item) {
			Print("Could not create item " + type);
			return item;
		}
		
		item.SetPosition(position);
		item.SetMysteryBoxState(MysteryBoxWeaponState.PREVIEW);
		item.SetOrientation(orientation);
		item.Update();
		
		return item;
	}
	
	bool AllowedToSpawnType(string type)
	{		
		return true;
	}

	bool IsOpening()
	{
		return m_IsOpening;
	}
	
	int GetCostToOpen()
	{
		return m_CostToOpen;
	}
	
	void SetCostToOpen(int cost)
	{
		if (!GetGame().IsServer()) {
			return;
		}
		
		m_CostToOpen = cost;
		SetSynchDirty();
	}
	
	void OnCrateOpenClient()
	{
		EffectSound sound1;
		if (!PlaySoundSet(sound1, "BoxJingle_SoundSet", 0, 0)) {
			Error("Failed to play soundset BoxJingle_SoundSet");
			return;
		}
		
		EffectSound sound2;
		if (!PlaySoundSet(sound2, "BoxOpen_SoundSet", 0, 0)) {
			Error("Failed to play soundset BoxOpen_SoundSet");
			return;
		}
	}
	
	void OnCrateCloseClient()
	{
		EffectSound sound;
		if (!PlaySoundSet(sound, "BoxClose_SoundSet", 0, 0)) {
			Error("Failed to play soundset BoxClose_SoundSet");
			return;
		}
	}
	
	// This might be hack but itll do
	override bool OnStoreLoad(ParamsReadContext ctx, int version)
	{
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(Delete, 3500);
		return super.OnStoreLoad(ctx, version);
	}
	
	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, rpc_type, ctx);
		
		switch (rpc_type) {
			case RPC_CRATE_OPEN: {
				OnCrateOpenClient();
				break;
			}
			
			case RPC_CRATE_CLOSE: {
				OnCrateCloseClient();
				break;
			}
		}
	}
	
	vector GetWeaponRiseStartPosition()
	{
		return ModelToWorld(GetMemoryPointPos("weapon_start"));
	}
	
	vector GetWeaponRiseEndPosition()
	{
		return ModelToWorld(GetMemoryPointPos("weapon_end"));
	}
		
	override void SetActions()
	{
		super.SetActions();
		AddAction(ActionOpenMagicBox);
	}
	
	override bool CanPutIntoHands(EntityAI parent)
	{
		return false;
	}
	
	override bool CanPutInCargo(EntityAI parent)
	{
		return false;
	}
	
	override bool IsTakeable()
	{
		return false;
	}
	
	override bool CanDisplayCargo()
	{
		return false;
	}
	
	override bool IsInventoryVisible()
	{
		return false;
	}
}