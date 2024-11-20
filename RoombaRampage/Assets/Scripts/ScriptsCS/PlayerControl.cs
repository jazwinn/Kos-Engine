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
            if (m_InternalCallIsKeyTriggered(Key.keyCode.F))
            {
                m_UnloadAllScene();
                m_InternalCallLoadScene("Script Test");
            }
            if (m_InternalCallIsKeyTriggered(Key.keyCode.G))
            {
                int new_entity = m_InternalCallAddPrefab("blackTile_test_1", 0.0f, 0.0f); //do not call prefabs that share the same script as the current
                queue.Enqueue(new_entity);
            }
            if (m_InternalCallIsKeyTriggered(Key.keyCode.H))
            {
                if(queue.Count > 0){
                    m_InternalCallDeleteEntity((uint)queue.Dequeue());
                }
                
            }

            m_InternalSetVelocity(EntityID, velocity);
        }
    }
}