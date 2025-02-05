using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Policy;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

public class LevelOneButton : ScriptBase
{
    private uint EntityID;
    //private string SceneName;
    private bool LevelSelected = false;

    // public bool animPlay = false;
    public override void Awake(uint id)
    {
        EntityID = id;
    }


    public override void Start()
    {
    }

    public override void Update()
    {
        if (InternalCall.m_InternalCallIsKeyTriggered(keyCode.LMB))
        {
            InternalCall.m_InternalGetButtonComponent(EntityID, out Vector2 position, out Vector2 scale, out bool isClick);
            if (isClick == true)
            {
                isClick = false;
                InternalCall.m_InternalCallPlayAudio(EntityID, "aud_buttonClick01");
                LevelSelection.LevelSelected = true;
                LevelSelection.SceneName = "Level1";
            }
        }
    }
}
