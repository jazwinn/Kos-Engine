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

    public class Vector2
    {
        public float x;
        public float y;
    }

    public class ScriptBase
    {
        //[MethodImpl(MethodImplOptions.InternalCall)]
        //public extern int GetFirstEntityID();

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern TransformComponent GetTransformComponent();

        public virtual void Start() { }
        public virtual void Update() { }
    }
}
