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

    private string startText;
    private string startFileName;
    private int startFontLayer;
    private float startFontSize;
    private Vector3 startColor;

    public override void Start()
    {
        //InternalCall.m_InternalGetTextComponent(EntityID, out startText, out startFileName, out startFontLayer, out startFontSize, out startColor);

        //startText = PlayerGun.ammoCount.ToString();
        //InternalCall.m_InternalSetTextComponent(EntityID, in startText, in startFileName, in startFontLayer, in startFontSize, in startColor);
    }

    public override void Update()
    {
        //InternalCall.m_InternalSetTextComponent(EntityID, "FuckYourDad", startFileName, startFontLayer, startFontSize, startColor);
    }
}