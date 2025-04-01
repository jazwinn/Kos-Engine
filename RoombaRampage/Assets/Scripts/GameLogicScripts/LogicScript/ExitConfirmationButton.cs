using System;
using System.IO;


public class ExitConfirmationButton : ScriptBase
{
    #region Entity ID
    private uint EntityID;

    public override void Awake(uint id)
    {

        EntityID = id;
        InternalCall.m_InternalCallGetChildrenID(EntityID);
    }
    #endregion

    public override void Start()
    {
    }

    public override void Update()
    {
        if(PauseMenuButtons.confirmQuit == true)
        {
            InternalCall.m_UnHideEntityandchildren(EntityID);
            //InternalCall.m_InternalCallGetChildrenID
        }
        else
        {
            InternalCall.m_HideEntityandchildren(EntityID);

        }
    }
}