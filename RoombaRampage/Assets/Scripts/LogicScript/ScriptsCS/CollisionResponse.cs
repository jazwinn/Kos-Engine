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
        private float iscollided;
        private float deltatime;
        private string collidedEntities;

        public override void GetEntityID(uint id){
            EntityID = id;
        }

        public override void Start()
        {
            collidedEntities = "";
            iscollided = 0.0f;
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
            iscollided = m_InternalCallIsCollided(EntityID, out collidedEntities);
            string[] parts = collidedEntities.Split(',');
            if (iscollided != 0.0f)
            {

                foreach (var part in parts)
                {
                    Console.WriteLine($"{part} is Script Colliding");
                }
                //Console.WriteLine($"{EntityID} is Script Colliding");
                //velocity.X = 0.0f;
                //velocity.Y = 0.0f;
                //m_InternalSetVelocity(EntityID, in velocity);
                ///*
                // * TED Script
                // */
            }







        }
    }
}