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
            //Console.WriteLine("Start");
        }

        public override void Update()
        {
            EntityID = 0;

            //Console.WriteLine("Update");

            Vector2 velocity;
            if(!m_InternalGetVelocity(EntityID, out velocity))
            {
                // return cause velocity -> rigidbody is not present in entity
                return;
            }

            velocity.X = 0;
            velocity.Y = 0;

            if (m_InternalCallIsKeyPressed(Key.keyCode.W))
            {
                velocity.Y = 1;
            }
            if (m_InternalCallIsKeyPressed(Key.keyCode.S))
            {
                velocity.Y = -1;
            }
            if (m_InternalCallIsKeyPressed(Key.keyCode.A))
            {
                velocity.X = -1;
            }
            if (m_InternalCallIsKeyPressed(Key.keyCode.D))
            {
                velocity.X = 1;
            }

            
            m_InternalSetVelocity(EntityID, in velocity);


        }
    }
}