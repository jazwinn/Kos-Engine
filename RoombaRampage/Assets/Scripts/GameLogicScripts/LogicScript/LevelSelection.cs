using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Policy;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

public class LevelSelection : ScriptBase
{
    private uint EntityID;
    public static string SceneName;
    public static bool LevelSelected = false;

    public override void Awake(uint id)
    {
        EntityID = id;
    }

    public override void Start()
    {
    }

    public override void Update()
    {
        if(InternalCall.m_InternalCallIsKeyPressed(keyCode.Y) && LevelSelected == true)
        {
            InternalCall.m_UnloadAllScene();
            InternalCall.m_InternalCallLoadScene(SceneName);
        }
        else if (InternalCall.m_InternalCallIsKeyPressed(keyCode.N))
        {
            InternalCall.m_InternalCallCloseWindow();
        }
    }
}
