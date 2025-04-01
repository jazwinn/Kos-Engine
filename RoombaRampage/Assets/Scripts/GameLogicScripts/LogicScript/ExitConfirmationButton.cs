using System;
using System.IO;


public class ExitConfirmationButton : ScriptBase
{
    #region Entity ID
    private uint EntityID;
    private uint ConfirmID;
    private int[] childrenIDList;


    public override void Awake(uint id)
    {

        EntityID = id;
        childrenIDList = InternalCall.m_InternalCallGetChildrenID(EntityID);
    }
    #endregion

    public override void Start()
    {
    }

    public override void Update()
    {
        if(PauseMenuButtons.confirmQuit == true)
        {
            InternalCall.m_UnHideEntityandchildren((uint)childrenIDList[1]);
            InternalCall.m_UnHideEntityandchildren((uint)childrenIDList[2]);

        }
    }
}