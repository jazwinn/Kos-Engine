using System;

public class SoundSlider : ScriptBase
{
    #region Entity ID
    private uint EntityID;

    public override void Awake(uint id)
    {
        EntityID = id;
    }
    #endregion

    public int sliderNo;

    private bool isHovering;


    public override void Start()
    {
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

    }

    private void CheckForClicks()
    {
        if (InternalCall.m_InternalCallIsKeyPressed(keyCode.LMB) && isHovering == true)
        {
            
            switch (sliderNo)
            {
                
                default:
                    break;
            }
        }
    }
}