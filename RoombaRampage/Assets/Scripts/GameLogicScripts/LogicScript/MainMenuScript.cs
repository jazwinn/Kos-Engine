using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Policy;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

public class MainMenuScript : ScriptBase
{
    private uint EntityID;
    private string commandPromptPrefab;

   // public bool animPlay = false;
    public override void Awake(uint id)
    {
        EntityID = id;
        
        commandPromptPrefab = "Command Prompt";
    }


    public override void Start()
    {
        
    }

    public override void Update()
    {
        if (InternalCall.m_InternalCallIsKeyTriggered(keyCode.LMB))
        {
            InternalCall.m_InternalCallPlayAudio(EntityID, "aud_buttonClick01");

            InternalCall.m_InternalGetButtonComponent(EntityID, out Vector2 position, out Vector2 scale, out bool isClick);
            if (isClick == true)
            {
                isClick = false;
                InternalCall.m_InternalCallAddPrefab(commandPromptPrefab, 0, 0, 0);
            }
        }
    }
}