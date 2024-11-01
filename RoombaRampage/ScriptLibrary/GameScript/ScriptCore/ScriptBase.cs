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
        public extern static bool m_InternalGetTransformComponent(uint entity, out Vector2 pos, out Vector2 scale, out float rotate);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static bool m_InternalCallIsKeyPressed(keyCode key);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static bool m_InternalGetVelocity(uint entity, out Vector2 Velocity);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static bool m_InternalSetVelocity(uint entity, in Vector2 Velocity);

        public virtual void Start() { }
        public virtual void Update() { }
    }
}