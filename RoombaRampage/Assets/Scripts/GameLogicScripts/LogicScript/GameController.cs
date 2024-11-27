using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Policy;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

public class GameController : ScriptBase
{
    #region Entity ID
    private uint EntityID;

    public override void GetEntityID(uint id)
    {
        EntityID = id;
    }
    #endregion

    public static bool gameIsPaused;

    public bool easyMode;

    public override void Start()
    {
        easyMode = false;
        gameIsPaused = false;
    }

    public override void Update()
    {
        if (InternalCall.m_InternalCallIsKeyTriggered(keyCode.R))
        {
            InternalCall.m_InternalCallSetTimeScale(1);
            InternalCall.m_UnloadAllScene();
            InternalCall.m_InternalCallLoadScene("MainScene");
        }

        if (InternalCall.m_InternalCallIsKeyTriggered(keyCode.P))
        {
            if (!easyMode)
            {
                easyMode = true;
                EnemyController.enemySpeed = 0.2f;
            }

            else
            {
                easyMode = false;
                EnemyController.enemySpeed = 1f;
            }
            
        }

        if (InternalCall.m_InternalCallIsKeyTriggered(keyCode.ESC))
        {
            if (!gameIsPaused)
            {
                Console.WriteLine("Game is paused");
                InternalCall.m_InternalCallSetTimeScale(0);
                gameIsPaused = true;
            }

            else
            {
                Console.WriteLine("Game is unpaused");
                InternalCall.m_InternalCallSetTimeScale(1);
                gameIsPaused = false;
            }
            
        }
    }
}