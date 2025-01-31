using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Policy;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

public class EnemyBloodPool : ScriptBase
{
    #region Entity ID
    private uint EntityID;

    public override void Awake(uint id)
    {
        EntityID = id;

        isAnimating = true;
        InternalCall.m_InternalGetAnimationComponent(EntityID, ref startFrameNumber, ref startFramesPerSecond, ref startFrameTimer, ref isAnimating, ref stripCount);
    }
    #endregion

    private int currentFrameNumber, startFrameNumber, startFramesPerSecond, stripCount;
    private float currentFrameTimer, startFrameTimer;
    private bool isAnimating = true;

    public override void Start()
    {
       
    }

    public override void Update()
    {
        if (isAnimating)
        {
            InternalCall.m_InternalGetAnimationComponent(EntityID, ref currentFrameNumber, ref startFramesPerSecond, ref currentFrameTimer, ref isAnimating, ref stripCount);

            if (currentFrameNumber == stripCount - 1)
            {
                isAnimating = false;
                InternalCall.m_InternalSetAnimationComponent(EntityID, ref currentFrameNumber, ref startFramesPerSecond, ref startFrameTimer, ref isAnimating, ref stripCount);
            }
        }
    }
}