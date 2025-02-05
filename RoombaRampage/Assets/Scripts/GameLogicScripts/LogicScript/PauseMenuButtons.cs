using System;

public class PauseMenuButtons : ScriptBase
{
    #region Entity ID
    private uint EntityID;

    public override void Awake(uint id)
    {
        EntityID = id;
    }
    #endregion

    public int buttonFunctionNo;

    private bool isHovering;

    private AnimationComponent animComp;

    public override void Start()
    {
        animComp = Component.Get<AnimationComponent>(EntityID);
    }

    public override void Update()
    {
        if (GameControllerLevel1.gameIsPaused)
        {
            CheckForCollisions();
            CheckForHover();
            CheckForClicks();
        }
        
    }

    private void CheckForCollisions()
    {
        if (InternalCall.m_InternalCallIsCollided(EntityID) != 0.0f)
        {
            int[] collidedEntities = InternalCall.m_InternalCallGetCollidedEntities(EntityID);

            foreach (int collidedEntitiesID in collidedEntities)
            {
                switch (InternalCall.m_InternalCallGetTag((uint)collidedEntitiesID))
                {
                    case "PauseMenuCursor":
                        isHovering = true;
                        break;

                    default:
                        break;
                }
            }
        }

        else
        {
            isHovering = false;
        }
    }

    private void CheckForHover()
    {
        if (isHovering)
        {
            animComp.m_frameNumber = 1;
            Component.Set<AnimationComponent>(EntityID, animComp);
        }

        else
        {
            animComp.m_frameNumber = 0;
            Component.Set<AnimationComponent>(EntityID, animComp);
        }
    }

    private void CheckForClicks()
    {
        if (InternalCall.m_InternalCallIsKeyTriggered(keyCode.LMB) && isHovering == true)
        {
            switch (buttonFunctionNo)
            {
                case 10:
                    //Resume
                    ResumeGame();
                    break;
                case 20:
                    //Restart
                    RestartGame();
                    break;
                case 30:
                    //HowToplay
                    HowToPlay();
                    break;
                case 31:
                    //HowToPlayBack
                    HowToPlayBack();
                    break;
                case -1:
                    //Quit Game
                    QuitToMainMenu();
                    break;

                default:
                    break;
            }
        }
    }

    private void ResumeGame()
    {
        //Hide Pause Menu Layer
        InternalCall.m_DisableLayer(7);

        //Resumes all coroutines
        CoroutineManager.Instance.ResumeAllCoroutines();

        //Sets delta timescale to 1
        InternalCall.m_InternalCallSetTimeScale(1f);

        GameControllerLevel1.gameIsPaused = false;
        GameControllerLevel1.runOnce = false;
    }

    private void RestartGame()
    {
        if (LevelSelection.SceneName != null)
        {
            InternalCall.m_InternalCallStopAllAudio();
            CoroutineManager.Instance.StopAllCoroutines();
            InternalCall.m_InternalCallSetTimeScale(1);
            InternalCall.m_UnloadAllScene();
            InternalCall.m_InternalCallLoadScene(LevelSelection.SceneName);
        }

        else
        {
            Console.WriteLine("Level Selection Script Has No SceneName!");

            InternalCall.m_InternalCallStopAllAudio();

            CoroutineManager.Instance.StopAllCoroutines();
            InternalCall.m_InternalCallSetTimeScale(1);
            InternalCall.m_UnloadAllScene();
            InternalCall.m_InternalCallLoadScene("Level1");
        }
    }

    private void HowToPlay()
    {
        InternalCall.m_DisableLayer(7);
        InternalCall.m_EnableLayer(6);
    }

    private void HowToPlayBack()
    {
        InternalCall.m_DisableLayer(6);
        InternalCall.m_EnableLayer(7);
    }

    private void QuitToMainMenu()
    {
        InternalCall.m_InternalCallStopAllAudio();
        CoroutineManager.Instance.StopAllCoroutines();
        InternalCall.m_InternalCallSetTimeScale(1);
        InternalCall.m_UnloadAllScene();
        InternalCall.m_InternalCallLoadScene("MainMenu");
    }
}