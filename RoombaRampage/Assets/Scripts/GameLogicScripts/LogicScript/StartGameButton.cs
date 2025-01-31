using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Versioning;
using System.Security.Policy;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

public class StartGameButton : ScriptBase
{
    public static string yolo;// see moving object script

    private uint EntityID;
    public float speed;

    private Queue<int> queue = new Queue<int>();

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
            ButtomComponent bc = Component.Get<ButtomComponent>(EntityID);
            if (bc.m_IsClick == true)
            {
                InternalCall.m_InternalCallPlayAudio(EntityID, "aud_buttonClick01");
                InternalCall.m_UnloadAllScene();
                InternalCall.m_InternalCallLoadScene("MainGameplay");
            }
        }   
    }
}