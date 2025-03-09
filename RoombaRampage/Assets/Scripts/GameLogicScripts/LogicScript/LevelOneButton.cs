using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Policy;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

public class LevelOneButton : ScriptBase
{
    private uint EntityID;
    //private string SceneName;
    private bool LevelSelected = false;
    private bool isSelected;

    // public bool animPlay = false;
    public override void Awake(uint id)
    {
        EntityID = id;
    }


    public override void Start()
    {
        InternalCall.m_DisableLayer(11);
    }

    public override void Update()
    {
        if (InternalCall.m_InternalCallIsKeyTriggered(keyCode.LMB))
        {
            InternalCall.m_InternalGetButtonComponent(EntityID, out Vector2 position, out Vector2 scale, out bool isClick);
            if (isClick == true && isSelected == false)
            {
                AnimationComponent temp = Component.Get<AnimationComponent>(EntityID);
                temp.m_frameNumber = 0;
                Component.Set<AnimationComponent>(EntityID, temp);

                isSelected = true;
                isClick = false;
                InternalCall.m_InternalCallPlayAudio(EntityID, "aud_buttonClick01");
                LevelSelection.LevelSelected = true;
                InternalCall.m_EnableLayer(11);
                LevelSelection.SceneName = "Cutscene01";
            }

            else if (isClick == true && isSelected == true)
            {
                AnimationComponent temp = Component.Get<AnimationComponent>(EntityID);
                temp.m_frameNumber = 1;
                Component.Set<AnimationComponent>(EntityID, temp);
                isSelected = false;
                isClick = false;
                InternalCall.m_InternalCallPlayAudio(EntityID, "aud_buttonClick01");
                LevelSelection.LevelSelected = false;
                InternalCall.m_DisableLayer(11);
                LevelSelection.SceneName = null;
            }
        }
    }
}
