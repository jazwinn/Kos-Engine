
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

public class MovingObjectScript : ScriptBase
{
    private uint EntityID;

    public float Speed; // Speed of movement
    public bool Static;

    private bool movingRight = true; // Direction flag (moving right or left)
    private float timeElapsed = 0f;  // Time elapsed since the last direction change
    public float changeDirectionInterval; // Interval (in seconds) for changing direction
    private Vector2 velocity;

    public override void GetEntityID(uint id)
    {
        EntityID = id;
    }

    public override void Start()
    {
        Speed = 2;
        changeDirectionInterval = 3;
        Static = false;
    }

    public override void Update()
    {

        if (Static)
        {
            velocity.X = 0;
            velocity.Y = 0;
            InternalCall.m_InternalSetVelocity(EntityID, in velocity);
            return;
        }

        //update time elapsed
        float deltatime;
        deltatime = InternalCall.m_InternalCallGetDeltaTime();
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



        if (!InternalCall.m_InternalGetVelocity(EntityID, out velocity))
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

        //Console.WriteLine(PlayerControl.yolo);



        InternalCall.m_InternalSetVelocity(EntityID, in velocity);


    }
}