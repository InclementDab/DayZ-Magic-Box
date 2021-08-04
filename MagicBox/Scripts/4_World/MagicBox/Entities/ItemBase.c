enum MysteryBoxWeaponState
{
	NORMAL = 0,
	PREVIEW = 1,
	AWAITING_USER = 2,
	TAKEN = 3
};

modded class ItemBase
{
	protected MysteryBoxWeaponState m_MysteryBoxWeaponState;
	
	void ItemBase()
	{
		RegisterNetSyncVariableInt("m_MysteryBoxWeaponState", 0, 3);
	}
	
	MysteryBoxWeaponState GetMysteryBoxState()
	{
		return m_MysteryBoxWeaponState;
	}
	
	override void EEItemLocationChanged(notnull InventoryLocation oldLoc, notnull InventoryLocation newLoc)
	{
		super.EEItemLocationChanged(oldLoc, newLoc);
		
		if (GetGame().IsServer() && GetMysteryBoxState() == MysteryBoxWeaponState.AWAITING_USER) {
			SetMysteryBoxState(MysteryBoxWeaponState.TAKEN);
		}
	}
	
	void SetMysteryBoxState(MysteryBoxWeaponState value)
	{
		if (!GetGame().IsServer()) {
			return;
		}
		
		m_MysteryBoxWeaponState = value;
		SetSynchDirty();
	}
	
	override bool CanPutIntoHands(EntityAI parent)
	{
		return super.CanPutIntoHands(parent) && GetMysteryBoxState() != MysteryBoxWeaponState.PREVIEW;
	}
	
	override bool CanPutInCargo(EntityAI parent)
	{
		return super.CanPutInCargo(parent) && GetMysteryBoxState() != MysteryBoxWeaponState.PREVIEW;
	}
	
	override bool IsTakeable()
	{
		return super.IsTakeable() && GetMysteryBoxState() != MysteryBoxWeaponState.PREVIEW;
	}
	
	override bool CanDisplayCargo()
	{
		return super.CanDisplayCargo() && GetMysteryBoxState() != MysteryBoxWeaponState.PREVIEW;
	}
	
	override bool IsInventoryVisible()
	{
		return super.IsInventoryVisible() && GetMysteryBoxState() != MysteryBoxWeaponState.PREVIEW;
	}
	
	void PlayCrateFailSound()
	{
		EffectSound sound;
		if (!PlaySoundSet(sound, "BoxFail_SoundSet", 0, 0)) {
			Error("Failed to play jingle!");
			return;
		}
	}
	
	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, rpc_type, ctx);
		
		switch (rpc_type) {
			case MagicBox.RPC_CRATE_FAIL: {
				PlayCrateFailSound();
				break;
			}
		}
	}
}