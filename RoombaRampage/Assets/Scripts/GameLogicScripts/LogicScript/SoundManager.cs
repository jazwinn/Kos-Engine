using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Policy;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Xml;

public class SoundManager : ScriptBase
{

    private uint EntityID;

    static public float sfx;
    static public float bgm;

    public override void Awake(uint id)
    {
        EntityID = id;

        sfx = InternalCall.m_InternalCallGetGlobalSFXVolume();
        bgm = InternalCall.m_InternalCallGetGlobalBGMVolume();

    }


    public override void Start()
    {

    }

    public override void Update()
    {
       InternalCall.m_InternalCallSetGlobalBGMVolume(bgm);
        InternalCall.m_InternalCallSetGlobalSFXVolume(sfx);
    }

    public void LateUpdate()
    {

    }

}