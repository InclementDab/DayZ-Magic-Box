modded class ActionTakeItem
{
	override void OnExecuteServer(ActionData action_data)
	{
		if (!action_data.m_Target.GetObject()) {
			return;
		}
		
		super.OnExecuteServer(action_data);
	}
}