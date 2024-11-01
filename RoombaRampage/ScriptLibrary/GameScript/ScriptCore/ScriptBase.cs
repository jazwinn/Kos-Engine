using System;
using System.Runtime.CompilerServices;

namespace Namespace
{
    public class TransformComponent
    {
        public Vector2 m_position; 
        public float m_rotation;   
        public Vector2 m_scale;   
    }

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

        public virtual void Start() { }
        public virtual void Update() { }
    }
}