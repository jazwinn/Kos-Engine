using GameScript.ScriptCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

namespace Namespace
{
    public class MovingObjectScript : ScriptBase
    {
        public uint EntityID;

        
        public float Speed { get; private set; } // Speed of movement


        private bool movingRight = true; // Direction flag (moving right or left)
        private float timeElapsed = 0f;  // Time elapsed since the last direction change
        private float changeDirectionInterval; // Interval (in seconds) for changing direction

        public override void GetEntityID(uint id){
            EntityID = id;
        }

        public override void Start()
        {
            Speed = 2;
            changeDirectionInterval = 3;
        }

        public override void Update()
        {

            //update time elapsed
            float deltatime;
            m_InternalCallGetDeltaTime(out deltatime);
            timeElapsed += deltatime;

            //Console.WriteLine($"timeelapsed:{timeElapsed}");

            if (timeElapsed > changeDirectionInterval)
            {
                if (movingRight)
                {
                    movingRight = false; 
                }
                else
                {
                    movingRight = true;
                }
                timeElapsed = 0;
            }


            Vector2 velocity;
            if(!m_InternalGetVelocity(EntityID, out velocity))
            {
                // return cause velocity -> rigidbody is not present in entity
                return;
            }

            if (movingRight)
            {
                velocity.X = Speed;
            }
            else
            {
                velocity.X = -Speed;
            }





            m_InternalSetVelocity(EntityID, in velocity);


        }
    }
}