using GameScript.ScriptCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

namespace Namespace
{
    public class CameraFollowPlayerScript : ScriptBase
    {
        private uint EntityID;
        private int playerID;
        
        private Vector2 pos;

        public override void GetEntityID(uint id){
            EntityID = id;
        }

        public override void Start()
        {
            playerID = InternalCall.m_InternalCallGetPlayer();

            if(playerID < 0){
               Console.WriteLine("Player Component not present in entity");
            }

        }

        public override void Update()
        {
             //Console.WriteLine($"Player:{playerID}");
            if(playerID < 0) return;

            InternalCall.m_InternalGetTranslate((uint)playerID, out pos);

            InternalCall.m_InternalSetTranslate(EntityID, in pos);



        }
    }
}