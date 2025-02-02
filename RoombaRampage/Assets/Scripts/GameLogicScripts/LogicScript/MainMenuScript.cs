using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Policy;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Xml;

public class MainMenuScript : ScriptBase
{
    private uint EntityID;
    private string commandPromptPrefab;
    public bool animPlay = false;
    public override void Awake(uint id)
    {
        EntityID = id;
        
        commandPromptPrefab = "Command Prompt";

        Console.WriteLine("Spawning command prompt...");
        InternalCall.m_InternalCallAddPrefab(commandPromptPrefab, 0, 0, 0); // Add proper coordinates and rotation if necessary

        if (animPlay == false)
        {
            animPlay = true;
            InternalCall.m_UnloadAllScene();
            InternalCall.m_InternalCallLoadScene("MainGameplay");
        }


    }


    public override void Start()
    {
        
    }

    public override void Update()
    {
    
    
    }


}