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
        public extern static bool m_InternalSetTransformComponent(uint entity, in Vector2 pos, in Vector2 scale, in float rotate);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static bool m_InternalGetTranslate(uint entity, out Vector2 pos);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static bool m_InternalSetTranslate(uint entity, in Vector2 pos);


        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static bool m_InternalCallIsKeyPressed(keyCode key);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static bool m_InternalGetVelocity(uint entity, out Vector2 Velocity);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static bool m_InternalSetVelocity(uint entity, in Vector2 Velocity);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static bool m_InternalCallGetDeltaTime(out float DeltaTime);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static int m_InternalCallGetPlayer();

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static bool m_InternalCallIsCollided(uint entity);
        

        public virtual void GetEntityID(uint id) { }
        public virtual void Start() { }
        public virtual void Update() { }
    }
}