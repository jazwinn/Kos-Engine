using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using GameScript.ScriptCore;
using Key;


namespace Namespace
{


    public class ScriptBase 
    {

        public virtual void GetEntityID(uint id) { }
        public virtual void Start() { }
        public virtual void Update() { }
    }
}