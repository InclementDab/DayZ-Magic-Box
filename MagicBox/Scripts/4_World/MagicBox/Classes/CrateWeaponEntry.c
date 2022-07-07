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
		ItemBase item = ItemBase.Cast(GetGame().CreateObjectEx(Item, position, ECE_NONE));
		if (!item) {
			return item;
		}
		
		item.SetPosition(position);
		item.SetOrientation(orientation);
		item.Update();
		
		Weapon_Base weapon;		
		if (Class.CastTo(weapon, item) && GetGame().IsKindOf(Mag, "Magazine_Base")) { 
			weapon.SpawnAmmo(Mag);
		}
		
		foreach (string attachment: Attachments) {
			item.GetInventory().CreateAttachment(attachment);
		}
		
		return item;
	}
}