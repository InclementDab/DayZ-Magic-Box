modded class ActionTakeItemToHands
{
	override void OnExecute(ActionData action_data)
	{
		if (!action_data.m_Target.GetObject()) {
			return;
		}
		
		super.OnExecute(action_data);
	}
}