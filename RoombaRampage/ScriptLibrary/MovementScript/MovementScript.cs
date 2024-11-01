using GameScript.ScriptCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

namespace Namespace
{
    public class MovementScript : ScriptBase
    {
        public uint EntityID { get; set; }

        public override void Start()
        {

        }

        public override void Update()
        {
            EntityID = 0;
            Console.WriteLine("test");
            if (m_InternalCallIsKeyPressed(Key.keyCode.W)){
                Console.WriteLine("W is pressed");
            }

            RigidBodyComponent rbComp;
            m_InternalGetRigidBodyComponent(EntityID, out rbComp);

            rbComp.m_Velocity.X = 0;
            rbComp.m_Velocity.Y = 0;

            if (m_InternalCallIsKeyPressed(Key.keyCode.W))
            {
                rbComp.m_Velocity.Y = 1;
            }
            if (m_InternalCallIsKeyPressed(Key.keyCode.S))
            {
                rbComp.m_Velocity.Y = -1;
            }
            if (m_InternalCallIsKeyPressed(Key.keyCode.A))
            {
                rbComp.m_Velocity.X = -1;
            }
            if (m_InternalCallIsKeyPressed(Key.keyCode.D))
            {
                rbComp.m_Velocity.Y = 1;
            }



            //Vector2 translate, scale;
            //float rotate;

            //m_InternalGetTransformComponent(EntityID, out translate, out scale, out rotate);
            //Console.WriteLine($"Position: {translate.X}");


            //Console.WriteLine($"velocity: {rbComp.m_Velocity.X}" );


        }
    }
}