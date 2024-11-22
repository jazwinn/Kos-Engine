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
        private uint EntityID;
        public float speed;

        private Queue<int> queue = new Queue<int>();

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
            
            if(!InternalCall.m_InternalGetVelocity(EntityID, out velocity))
            {
                // return cause velocity -> rigidbody is not present in entity
                return;
            }

            velocity.X = 0;
            velocity.Y = 0;

            if (InternalCall.m_InternalCallIsKeyPressed(Key.keyCode.W))
            {
                velocity.Y = speed;
            }
            if (InternalCall.m_InternalCallIsKeyPressed(Key.keyCode.S))
            {
                velocity.Y = -speed;
            }
            if (InternalCall.m_InternalCallIsKeyPressed(Key.keyCode.A))
            {
                velocity.X = -speed;
            }
            if (InternalCall.m_InternalCallIsKeyPressed(Key.keyCode.D))
            {
                velocity.X = speed;
            }
            if (InternalCall.m_InternalCallIsKeyTriggered(Key.keyCode.F))
            {
                InternalCall.m_UnloadAllScene();
                InternalCall.m_InternalCallLoadScene("Script Test");
            }
            if (InternalCall.m_InternalCallIsKeyTriggered(Key.keyCode.G))
            {
                int new_entity = InternalCall.m_InternalCallAddPrefab("blackTile_test_1", 0.0f, 0.0f, 45.0f); //do not call prefabs that share the same script as the current
                queue.Enqueue(new_entity);
            }
            if (InternalCall.m_InternalCallIsKeyTriggered(Key.keyCode.H))
            {
                if(queue.Count > 0){
                    InternalCall.m_InternalCallDeleteEntity((uint)queue.Dequeue());//
                }
                
            }

            if (InternalCall.m_InternalCallIsKeyTriggered(Key.keyCode.X))
            {
                var sprite = GetComponent.GetSpriteComponent(EntityID);

                Console.WriteLine($"Image is : {sprite.m_imageFile}"); // get sprite component

                sprite.m_imageFile = "Button.png";

                SetComponent.SetSpriteComponent(EntityID, sprite); // set sprite component

            }

            InternalCall.m_InternalSetVelocity(EntityID, velocity);
        }
    }
}