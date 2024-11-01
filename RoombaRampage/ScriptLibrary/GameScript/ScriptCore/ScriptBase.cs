using System;
using System.Runtime.CompilerServices;
using GameScript.ScriptCore;
using Key;

namespace Namespace
{


    public struct Vector2
    {
        public float X, Y;
    }

    public class ScriptBase
    {
        // [MethodImpl(MethodImplOptions.InternalCall)]
        // public extern int GetFirstEntityID();

        //[MethodImplAttribute(MethodImplOptions.InternalCall)]
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void m_InternalGetTransformComponent(uint entity, out Vector2 pos, out Vector2 scale, out float rotate);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static bool m_InternalCallIsKeyPressed(keyCode key);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void m_InternalGetRigidBodyComponent(uint entity, out RigidBodyComponent rbcomp);

        public virtual void Start() { }
        public virtual void Update() { }
    }
}