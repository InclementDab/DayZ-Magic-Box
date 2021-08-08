class CrateWeaponEntry
{
	string Item;
	ref array<string> Attachments = {};
	string Mag;
	
	void CrateWeaponEntry(string item, array<string> attachments = null, string magazine = "")
	{
		Item = item;
		if (attachments) {
			Attachments.Copy(attachments);
		}
		
		Mag = magazine;
	}
	
	ItemBase CreateEntry(vector position, vector orientation)
	{
		ItemBase item = ItemBase.Cast(GetGame().CreateObject(Item, position, false, false, false));
		if (!item) {
			return item;
		}
		
		item.SetPosition(position);
		item.SetOrientation(orientation);
		item.Update();
		
		Weapon_Base weapon;		
		if (Class.CastTo(weapon, item) && GetGame().IsKindOf(Mag, "Magazine_Base")) { 
			AttachMagazine(weapon, Mag);				
		}
		
		foreach (string attachment: Attachments) {
			item.GetInventory().CreateAttachment(attachment);
		}
		
		return item;
	}
	
	// https://feedback.bistudio.com/T149240 thanks jacob
	static void AttachMagazine(Weapon_Base weapon, string magazine_type)
	{
		int stateId = -1;
	
		if (weapon.IsInherited(SKS_Base)) {
			return;
		} 
		
		else if (weapon.IsInherited(BoltActionRifle_InnerMagazine_Base)) {
			return;
		} 
		
		else if (weapon.IsInherited(DoubleBarrel_Base)) {
			return;
		} 
		
		else if (weapon.IsInherited(Pistol_Base)) {
			stateId = PistolStableStateID.CLO_DIS_BU0_MA1;
		} 
		
		else if (weapon.IsInherited(CZ527_Base)) {
			stateId = CZ527StableStateID.CZ527_CLO_BU0_MA1;
		} 
		
		else if (weapon.IsInherited(Repeater_Base)) {
			return;
		} 
		
		else if (weapon.IsInherited(RifleBoltFree_Base)) {
			stateId = RBFStableStateID.RBF_CLO_BU0_MA1;
		} 
		
		else if (weapon.IsInherited(RifleBoltLock_Base)) {
			stateId = RBLStableStateID.RBL_OPN_BU0_MA1;
		} 
		
		else if (weapon.IsInherited(Mp133Shotgun_Base)) {
			return;
		}
	
		InventoryLocation il = new InventoryLocation;
		il.SetAttachment(weapon, NULL, InventorySlots.MAGAZINE);
	
		// using any of the inventory sync for existing spawning magazines also works
		// e.g. GameInventory.LocationSyncMoveEntity
	
		EntityAI mag = SpawnEntity(magazine_type, il, ECE_IN_INVENTORY, RF_DEFAULT);
		GetGame().RemoteObjectDelete(mag);
		GetGame().RemoteObjectDelete(weapon);
	
		pushToChamberFromAttachedMagazine(weapon, weapon.GetCurrentMuzzle());
	
		ScriptReadWriteContext ctx = new ScriptReadWriteContext;
		ctx.GetWriteContext().Write(stateId);
		weapon.LoadCurrentFSMState(ctx.GetReadContext(), GetGame().SaveVersion());
		
		GetGame().RemoteObjectCreate(weapon);
		GetGame().RemoteObjectCreate(mag);
		
		//weapon.ValidateAndRepair();
	}
}