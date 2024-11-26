using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

public class CollisionResponse : ScriptBase
{
    private uint EntityID;
    private float iscollided;
    private float deltatime;

    private Vector2 position;
    private Vector2 scale;
    private bool isclick;

    public override void GetEntityID(uint id)
    {
        EntityID = id;
    }

    public override void Start()
    {
        iscollided = 0.0f;
    }

    public override void Update()
    {

        //Console.WriteLine($"Entity:{EntityID}");
        Vector2 velocity;
        if (!InternalCall.m_InternalGetVelocity(EntityID, out velocity))
        {
            // return cause velocity -> rigidbody is not present in entity
            return;
        }


        deltatime = InternalCall.m_InternalCallGetDeltaTime();


        iscollided = InternalCall.m_InternalCallIsCollided(EntityID);
        if (iscollided != 0.0f)
        {
            int[] collidedEntities = InternalCall.m_InternalCallGetCollidedEntities(EntityID);
            Console.WriteLine(collidedEntities.Length);

            foreach (var collidedid in collidedEntities)
            {
                Console.WriteLine($"{collidedid} is Collided");

                string tag = InternalCall.m_InternalCallGetTag((uint)collidedid);

                Console.WriteLine($"Collided tag is {tag}");
            }
            //Console.WriteLine($"{EntityID} is Script Colliding");
            //velocity.X = 0.0f;
            //velocity.Y = 0.0f;
            //m_InternalSetVelocity(EntityID, in velocity);
            ///*
            // * TED Script
            // */
        }

        if (InternalCall.m_InternalCallIsKeyPressed(keyCode.LMB))
        {
            InternalCall.m_InternalGetButtonComponent(EntityID, out position, out scale, out isclick);

            if (isclick == true)
            {
                Console.WriteLine($"Button is clicked : {isclick}");
            }
            else
            {
                Console.WriteLine($"Button not clicked");
            }
        }





    }
}