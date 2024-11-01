using Namespace;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameScript.ScriptCore
{
    public class TransformComponent
    {
        public Vector2 m_position;
        public float m_rotation;
        public Vector2 m_scale;
    }

    public class RigidBodyComponent
    {
        public Vector2 m_Velocity;
        public Vector2 m_Acceleration;
        public float m_Rotation;
        public float m_AngularVelocity;
        public float m_AngularAcceleration;
        public float m_Mass;
        public float m_InverseMass;
        public float m_LinearDamping;
        public float m_AngularDamping;
        public Vector2 m_Force;
        public float m_Torque;
        public bool m_IsKinematic;
        public bool m_IsStatic;


    }
}
