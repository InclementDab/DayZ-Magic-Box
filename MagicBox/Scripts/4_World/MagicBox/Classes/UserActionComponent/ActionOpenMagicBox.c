class ActionOpenMagicBox: ActionInteractBase
{
	protected PlayerBase m_Player;
	protected ActionTarget m_ActionTarget;
	
	void ActionOpenMagicCrate()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_HUDCursorIcon = CursorIcons.OpenDoors;
	}
	
	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}
		
	override bool HasTarget()
	{
		return true;
	}
	
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		m_Player = player;
		m_ActionTarget = target;
		MagicBox crate = MagicBox.Cast(target.GetObject());
		
		int amnt = player.GetMagicBoxCurrency();
		
		return (crate && !crate.IsOpening() && amnt >= crate.GetCostToOpen());
	}
	
	override void OnEndServer(ActionData action_data)
	{
		MagicBox crate = MagicBox.Cast(action_data.m_Target.GetObject());
		if (!crate) {
			return;
		}
		
		if (crate.TryOpenCrate(action_data.m_Player)) {
			action_data.m_Player.DeductMagicBoxCurrency(crate.GetCostToOpen());
		}
	}
	
	override string GetText()
	{		
		if (!m_ActionTarget) {
			return "";
		}
		
		MagicBox crate = MagicBox.Cast(m_ActionTarget.GetObject());
		if (!crate) {
			return "Invalid Crate";
		}
		
		if (m_Player && m_Player.GetMagicBoxCurrencyType() == MagicBoxCurrencyType.NONE) {
			return "Open Crate";
		}
	
		return string.Format("Open Crate [%1]", crate.GetCostToOpen());
	}
}