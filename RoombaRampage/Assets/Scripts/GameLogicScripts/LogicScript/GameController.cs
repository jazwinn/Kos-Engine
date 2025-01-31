using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Policy;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Xml;

public class GameController : ScriptBase
{
    #region Entity ID
    private uint EntityID;

    public override void Awake(uint id)
    {
        EntityID = id;

        InternalCall.m_InternalCallPlayAudio(EntityID, "aud_testLoop");

        LevelDoorLocker.doorLocked = false;

        isSceneActive = true;
        float r = 1.0f;
        InternalCall.m_InternalCallSetTimeScale(ref r);
        easyMode = false;
        gameIsPaused = false;

        EnemyController.enemySpeed = 3;
        EnemyController.playerIsDead = false;
        EnemyController.forceDeath = false;
        EnemyController.totalEnemiesKilled = 0;

        pauseMenuID = InternalCall.m_InternalCallGetTagIDs("UIPauseMenu");

        foreach (int pMenuID in pauseMenuID)
        {
            InternalCall.m_InternalGetTextComponent((uint)pMenuID, ref pauseMenuText, ref pauseMenuFontFileName, ref pauseMenuFontLayer, ref pauseMenuFontSize, ref pauseMenuFontColor);
            InternalCall.m_InternalSetTextComponent((uint)pMenuID, pauseMenuText, pauseMenuFontFileName, ref pauseMenuHideLayer, ref pauseMenuFontSize, ref pauseMenuFontColor);
        }

        foreach (int ID in InternalCall.m_InternalCallGetTagIDs("UIQuitConfirmation"))
        {
            TextComponent textComponent = GetComponent.GetTextComponent((uint)ID);
            textComponent.m_fontLayer = -1000;
            SetComponent.SetTextComponent((uint)ID, textComponent);
        }
    }
    #endregion

    public static bool gameIsPaused;

    public bool easyMode;

    private int[] pauseMenuID;
    private string pauseMenuText;
    private string pauseMenuFontFileName;
    private int pauseMenuFontLayer;
    private float pauseMenuFontSize;
    private Vector3 pauseMenuFontColor;
    private int pauseMenuShowLayer = 100;
    private int pauseMenuHideLayer = -1000;

    public static bool isSceneActive;

    private bool confirmQuit;

    public override void Start()
    {
       
    }

    public override void Update()
    {

        //Console.WriteLine(InternalCall.m_InternalCallGetGameTime());

        if (InternalCall.m_InternalCallIsKeyTriggered(keyCode.R))
        {
            if (confirmQuit)
            {
                confirmQuit = false;
                foreach (int id in InternalCall.m_InternalCallGetTagIDs("UIQuitConfirmation"))
                {
                    TextComponent textComponent = GetComponent.GetTextComponent((uint)id);
                    textComponent.m_fontLayer = -1000;
                    SetComponent.SetTextComponent((uint)id, textComponent);
                }
                return;
            }

            InternalCall.m_InternalCallStopAllAudio();

            isSceneActive = false;
            float f = 1;
            InternalCall.m_InternalCallSetTimeScale(ref f);
            InternalCall.m_UnloadAllScene();
            InternalCall.m_InternalCallLoadScene("MainGameplay");
        }

        if (InternalCall.m_InternalCallIsKeyTriggered(keyCode.BACKSPACE))
        {
            if (confirmQuit)
            {
                InternalCall.m_InternalCallCloseWindow();
            }

            else
            {
                foreach(int id in InternalCall.m_InternalCallGetTagIDs("UIQuitConfirmation"))
                {
                    TextComponent textComponent = GetComponent.GetTextComponent((uint)id);
                    textComponent.m_fontLayer = 100;
                    SetComponent.SetTextComponent((uint)id, textComponent);
                }

                confirmQuit = true;
            }
        }

        //Pausing
        if (InternalCall.m_InternalCallIsKeyTriggered(keyCode.ESC))
        {
            if (confirmQuit)
            {
                confirmQuit = false;
                foreach (int id in InternalCall.m_InternalCallGetTagIDs("UIQuitConfirmation"))
                {
                    TextComponent textComponent = GetComponent.GetTextComponent((uint)id);
                    textComponent.m_fontLayer = -1000;
                    SetComponent.SetTextComponent((uint)id, textComponent);
                }
                return;
            }

            if (!gameIsPaused)
            {
                float i = 0;
                InternalCall.m_InternalCallSetTimeScale(ref i);
                foreach (int pMenuID in pauseMenuID)
                {
                    InternalCall.m_InternalGetTextComponent((uint)pMenuID, ref pauseMenuText, ref pauseMenuFontFileName, ref pauseMenuFontLayer, ref pauseMenuFontSize, ref pauseMenuFontColor);
                    InternalCall.m_InternalSetTextComponent((uint)pMenuID, pauseMenuText, pauseMenuFontFileName, ref pauseMenuShowLayer, ref pauseMenuFontSize, ref pauseMenuFontColor);
                }
                gameIsPaused = true;
            }

            else
            {
                float i = 1;
                InternalCall.m_InternalCallSetTimeScale(ref i);
                foreach (int pMenuID in pauseMenuID)
                {
                    InternalCall.m_InternalGetTextComponent((uint)pMenuID, ref pauseMenuText, ref pauseMenuFontFileName, ref pauseMenuFontLayer, ref pauseMenuFontSize, ref pauseMenuFontColor);
                    InternalCall.m_InternalSetTextComponent((uint)pMenuID, pauseMenuText, pauseMenuFontFileName, ref pauseMenuHideLayer, ref pauseMenuFontSize, ref pauseMenuFontColor);
                }
                gameIsPaused = false;
            }
            
        }

        #region Cheat Codes

        //Hold down LCTRL+Key to activate
        if (InternalCall.m_InternalCallIsKeyPressed(keyCode.LeftControl))
        {
            if (confirmQuit)
            {
                confirmQuit = false;
                foreach (int id in InternalCall.m_InternalCallGetTagIDs("UIQuitConfirmation"))
                {
                    TextComponent textComponent = GetComponent.GetTextComponent((uint)id);
                    textComponent.m_fontLayer = -1000;
                    SetComponent.SetTextComponent((uint)id, textComponent);
                }
                return;
            }

            //Sets enemy speed to snail
            if (InternalCall.m_InternalCallIsKeyTriggered(keyCode.P))
            {
                if (!easyMode)
                {
                    Console.WriteLine("CHEAT - Easy Mode Activated");
                    easyMode = true;
                    EnemyController.enemySpeed = 1f;
                }

                else
                {
                    Console.WriteLine("CHEAT - Easy Mode Deactivated");
                    easyMode = false;
                    EnemyController.enemySpeed = 4f;
                }
            }

            //Kills all enemies
            if (InternalCall.m_InternalCallIsKeyTriggered(keyCode.L))
            {
                Console.WriteLine("CHEAT - All Enemies Killed");
                EnemyController.forceDeath = true;
            }
        }
        #endregion
    }
}