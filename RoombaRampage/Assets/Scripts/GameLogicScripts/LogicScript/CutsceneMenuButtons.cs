using System;

public class CutsceneMenuButtons : ScriptBase
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
        CheckForCollisions();
        CheckForHover();
        CheckForClicks();

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
                    default:
                        if (isHovering)
                        {
                            break;
                        }
                        isHovering = true;
                        InternalCall.m_InternalCallPlayAudio(EntityID, "aud_buttonHover01");
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
        if (InternalCall.m_InternalCallIsKeyTriggered(keyCode.ESC))
        {
            InternalCall.m_InternalCallLoadScene(LevelSelection.SceneName);
        }

        if (InternalCall.m_InternalCallIsKeyTriggered(keyCode.LMB) || InternalCall.m_InternalCallIsKeyTriggered(keyCode.RMB) || InternalCall.m_InternalCallIsKeyTriggered(keyCode.SPACE) || InternalCall.m_InternalCallIsKeyTriggered(keyCode.ENTER))
        {
            InternalCall.m_InternalCallPlayAudio(EntityID, "aud_buttonClick01");
            switch (buttonFunctionNo)
            {
                case 10:
                    InternalCall.m_UnloadAllScene();
                    InternalCall.m_InternalCallLoadScene("Cutscene02");
                    break;
                case 11:
                    InternalCall.m_UnloadAllScene();
                    InternalCall.m_InternalCallLoadScene("Cutscene03");
                    break;
                case 12:
                    InternalCall.m_UnloadAllScene();
                    InternalCall.m_InternalCallLoadScene("Cutscene04");
                    break;
                case 13:
                    InternalCall.m_InternalCallPlayAudio(EntityID, "aud_startMenuAmbienceStop01");
                    InternalCall.m_InternalCallStopAllAudio();
                    InternalCall.m_UnloadAllScene();
                    InternalCall.m_InternalCallLoadScene("Level1");
                    break;
                case -1:
                    InternalCall.m_InternalCallPlayAudio(EntityID, "aud_startMenuAmbienceStop01");
                    InternalCall.m_InternalCallStopAllAudio();
                    InternalCall.m_UnloadAllScene();
                    InternalCall.m_InternalCallLoadScene("LevelSelect");
                    break;
                default:
                    break;
            }
        }
    }
}