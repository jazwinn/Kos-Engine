using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Policy;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

public class UIBulletCounter : ScriptBase
{
    #region Entity ID
    private uint EntityID;

    public override void GetEntityID(uint id)
    {
        EntityID = id;
    }
    #endregion

    TextComponent tc;

    public override void Start()
    {
        tc = GetComponent.GetTextComponent(EntityID);

        tc.m_text = "bitch";
        SetComponent.SetTextComponent(EntityID, tc);
    }

    public override void Update()
    {
        //InternalCall.m_InternalSetTextComponent(EntityID, "FuckYourDad", startFileName, startFontLayer, startFontSize, startColor);
    }
}