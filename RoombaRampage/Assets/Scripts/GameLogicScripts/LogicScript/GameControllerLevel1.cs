using System;

public class GameControllerLevel1 : ScriptBase
{
    #region Entity ID
    private uint EntityID;

    public override void Awake(uint id)
    {
        EntityID = id;
    }
    #endregion

    public static bool gameIsPaused; //For all scripts to check if game is paused

    public static bool runOnce; //Prevents PauseGame from running more than once


    public override void Start()
    {
        runOnce = false; 
        gameIsPaused = false;

        InternalCall.m_DisableLayer(8); //Disables Loadout Menu UI
        InternalCall.m_DisableLayer(7); //Disables Pause Menu UI
    }

    public override void Update()
    {
        InputChecker();
    }

    private void PauseGame()
    {
        if (!gameIsPaused && !runOnce)
        {
            gameIsPaused = true;
            runOnce = true;

            //Pauses all coroutines
            CoroutineManager.Instance.PauseAllCoroutines();

            //Sets delta timescale to 0
            InternalCall.m_InternalCallSetTimeScale(0f);

            //Shows Pause Menu Layer
            InternalCall.m_EnableLayer(7);
        }

        else if (gameIsPaused)
        {
            //Resumes all coroutines
            CoroutineManager.Instance.ResumeAllCoroutines();

            //Sets delta timescale to 1
            InternalCall.m_InternalCallSetTimeScale(1f);

            //Hide Pause Menu Layer
            InternalCall.m_DisableLayer(7);

            gameIsPaused = false;
            runOnce = false;
        }

    }

    private void Sortie()
    {
        if (!PlayerLoadoutManager.isSortieing)
        {
            PlayerLoadoutManager.isSortieing = true;
            InternalCall.m_InternalCallSetTimeScale(0.2f);
            InternalCall.m_EnableLayer(8);
        }

        else
        {
            PlayerLoadoutManager.isSortieing = false;
            InternalCall.m_InternalCallSetTimeScale(1f);
            InternalCall.m_DisableLayer(8);
        }
    }

    #region Check Inputs

    private void InputChecker()
    {
        if (InternalCall.m_InternalCallIsKeyTriggered(keyCode.ESC))
        {
            PauseGame();
        }


        if (gameIsPaused) { return; } //Ensures nothing but pause menu can be activated when game is paused

        if (InternalCall.m_InternalCallIsKeyTriggered(keyCode.TAB))
        {
            Sortie();
        }
    }
    #endregion
}