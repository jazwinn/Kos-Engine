using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text.RegularExpressions;

public class BulletTest : ScriptBase
{
    private uint EntityID;
    public override void Awake(uint id)
    {
        EntityID = id; //Sets ID for object, DO NOT TOUCH
    }

    public override void Start()
    {
        CoroutineManager.Instance.StartCoroutine(Death());
    }

    private IEnumerator Death()
    {
        Console.WriteLine("Starting death coroutine");
        yield return new CoroutineManager.WaitForSeconds(1.0f);
        InternalCall.m_InternalCallDeleteEntity(EntityID);
    }
}

