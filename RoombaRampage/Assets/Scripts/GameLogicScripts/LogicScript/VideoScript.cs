using System;

public class VideoScript : ScriptBase
{
    #region Entity ID
    private uint EntityID;

    public override void Awake(uint id)
    {
        EntityID = id;
    }
    #endregion

    public override void Start()
    {
        InternalCall.m_StartVideo(EntityID);
    }

    public override void Update()
    {

        if (InternalCall.m_HasVideoFinish(EntityID))
        {
            Console.WriteLine("Video Done");
        }

    }
}