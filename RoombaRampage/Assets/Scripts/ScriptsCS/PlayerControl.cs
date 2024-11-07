using GameScript.ScriptCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

namespace Namespace
{
    public class PlayerControl : ScriptBase
    {
        public uint EntityID;
        public float speed;

        public override void GetEntityID(uint id){
            EntityID = id;
        }

        public override void Start()
        {
            //Console.WriteLine("Start");
            speed = 5;
        }

        public override void Update()
        {

            //Console.WriteLine($"Entity:{EntityID}");

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
                velocity.Y = speed;
            }
            if (m_InternalCallIsKeyPressed(Key.keyCode.S))
            {
                velocity.Y = -speed;
            }
            if (m_InternalCallIsKeyPressed(Key.keyCode.A))
            {
                velocity.X = -speed;
            }
            if (m_InternalCallIsKeyPressed(Key.keyCode.D))
            {
                velocity.X = speed;
            }

            
            m_InternalSetVelocity(EntityID, in velocity);


        }
    }
}