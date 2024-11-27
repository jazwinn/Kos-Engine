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

    public override void GetEntityID(uint id)
    {
        EntityID = id;
    }
    #endregion

    private int currentFrameNumber, startFrameNumber, startFramesPerSecond, stripCount;
    private float currentFrameTimer, startFrameTimer;
    private bool isAnimating;

    public override void Start()
    {
        isAnimating = true;
    }

    public override void Update()
    {
        if (isAnimating)
        {
            InternalCall.m_InternalGetAnimationComponent(EntityID, out currentFrameNumber, out startFramesPerSecond, out currentFrameTimer, out isAnimating, out stripCount);

            if (currentFrameNumber == stripCount - 1)
            {
                isAnimating = false;
                InternalCall.m_InternalSetAnimationComponent(EntityID, currentFrameNumber, startFramesPerSecond, startFrameTimer, isAnimating, stripCount);
            }
        }
    }
}