using GameScript.ScriptCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

namespace Namespace
{
    public class CollisionResponse : ScriptBase
    {
        private uint EntityID;
        private bool iscollided;
        private float deltatime;

        public override void GetEntityID(uint id){
            EntityID = id;
        }

        public override void Start()
        {
            iscollided = false;
        }

        public override void Update()
        {

            //Console.WriteLine($"Entity:{EntityID}");

            Vector2 velocity;
            if (!m_InternalGetVelocity(EntityID, out velocity))
            {
                // return cause velocity -> rigidbody is not present in entity
                return;
            }


            m_InternalCallGetDeltaTime(out deltatime);
            iscollided = m_InternalCallIsCollided(EntityID);

            if (iscollided)
            {
                velocity.X = 0.01 * deltatime;
                velocity.Y = 0.01 * deltatime;
                m_InternalSetVelocity(EntityID, in velocity);
            }




            


        }
    }
}