using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Versioning;
using System.Security.Policy;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

public class ExitGameButton : ScriptBase
{
    public static string yolo;// see moving object script

    private uint EntityID;
    public float speed;

    private Queue<int> queue = new Queue<int>();

    public override void GetEntityID(uint id)
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
                InternalCall.m_InternalCallCloseWindow();
            }
        }   
    }
}