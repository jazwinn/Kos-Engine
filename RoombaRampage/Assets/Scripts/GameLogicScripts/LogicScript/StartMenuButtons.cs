using System;

public class StartMenuButtons : ScriptBase
{
    #region Entity ID
    private uint EntityID;

    public override void Awake(uint id)
    {
        EntityID = id;
        InternalCall.m_DisableLayer(9);
    }
    #endregion

    public int buttonFunctionNo;

    private bool isHovering;

    private AnimationComponent animComp;
    public static bool isReading;

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
                        isHovering = true;
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
                    if (isReading)
                    {
                        isReading = false;
                        InternalCall.m_DisableLayer(9);
                    }

                    else
                    {
                        isReading = true;
                        InternalCall.m_EnableLayer(9);
                    }
                    
                    break;
                case 11:
                    isReading = false;
                    InternalCall.m_DisableLayer(9);
                    break;
                case -1:
                    break;
                default:
                    break;
            }
        }
    }
}