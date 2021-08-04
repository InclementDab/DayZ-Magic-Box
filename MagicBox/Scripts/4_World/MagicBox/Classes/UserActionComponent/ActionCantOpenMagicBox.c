class ActionCantOpenMagicBox: ActionInteractBase
{
	protected ActionTarget m_ActionTarget;
	
	void ActionCantOpenMagicCrate()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_HUDCursorIcon = CursorIcons.CloseDoors;
	}
	
	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTCursorParent(UAMaxDistances.DEFAULT);
	}
	
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		m_ActionTarget = target;
		MagicBox crate = MagicBox.Cast(target.GetObject());
		return (crate && !crate.IsOpening() && player.getPlayerCurrencyAmount() < crate.GetCostToOpen());
	}
	
	override bool HasTarget()
	{
		return true;
	}
	
	override string GetText()
	{
		if (!m_ActionTarget) {
			return "Not Enough Money!";
		}
		
		MagicBox crate = MagicBox.Cast(m_ActionTarget.GetObject());
		if (!crate) {
			return "Invalid Crate";
		}
	
		return string.Format("Not Enough Money [%1]", crate.GetCostToOpen());
	}
}