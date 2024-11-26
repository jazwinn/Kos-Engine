using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Policy;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

public class PlayerControl : ScriptBase
{
    public static string yolo;// see moving object script

    private uint EntityID;
    public float speed;

    private int framenumber;
    private int intframespersecond;
    private float frametimer;
    private bool isanimation;
    private int stripcount;

    private Queue<int> queue = new Queue<int>();

    public override void GetEntityID(uint id)
    {
        EntityID = id;
       
    }

    public override void Start()
    {
        //Console.WriteLine("Start");
        speed = 5;
        yolo = "sean very extremely gay"; //used in movingobjectscript
        InternalCall.m_InternalGetAnimationComponent(EntityID, out framenumber, out intframespersecond, out frametimer, out isanimation, out stripcount);
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

        velocity.X = 0;
        velocity.Y = 0;

        if (InternalCall.m_InternalCallIsKeyPressed(keyCode.W))
        {
            velocity.Y = speed;
        }
        if (InternalCall.m_InternalCallIsKeyPressed(keyCode.S))
        {
            velocity.Y = -speed;
        }
        if (InternalCall.m_InternalCallIsKeyPressed(keyCode.A))
        {
            velocity.X = -speed;
        }
        if (InternalCall.m_InternalCallIsKeyPressed(keyCode.D))
        {
            velocity.X = speed;
        }
        InternalCall.m_InternalSetVelocity(EntityID, velocity);

        if (InternalCall.m_InternalCallIsKeyTriggered(keyCode.F))
        {
            InternalCall.m_UnloadAllScene();
            InternalCall.m_InternalCallLoadScene("ScriptTest");
        }
        if (InternalCall.m_InternalCallIsKeyTriggered(keyCode.G))
        {
            int new_entity = InternalCall.m_InternalCallAddPrefab("blackTile", 0.0f, 0.0f, 45.0f); //do not call prefabs that share the same script as the current
            queue.Enqueue(new_entity);
        }
        if (InternalCall.m_InternalCallIsKeyTriggered(keyCode.H))
        {
            if (queue.Count > 0)
            {
                InternalCall.m_InternalCallDeleteEntity((uint)queue.Dequeue());//
            }

        }

        if (InternalCall.m_InternalCallIsKeyTriggered(keyCode.X))
        {
            var sprite = GetComponent.GetSpriteComponent(EntityID);

            Console.WriteLine($"Image is : {sprite.m_imageFile}"); // get sprite component

            sprite.m_imageFile = "Button.png";

            SetComponent.SetSpriteComponent(EntityID, sprite); // set sprite component

        }

        if (InternalCall.m_InternalCallIsKeyTriggered(keyCode.Z))
        {
            intframespersecond = 0;
            stripcount = 1;
            InternalCall.m_InternalSetAnimationComponent(EntityID, in framenumber, in intframespersecond, in frametimer, in isanimation, in stripcount);
        }
        if (InternalCall.m_InternalCallIsKeyTriggered(keyCode.V))
        {
            bool have_children;
            int[] childs = InternalCall.m_InternalCallGetChildrenID(EntityID, out have_children);

            if (have_children)
            {
                foreach (var id in childs)
                {
                    Console.WriteLine($"{id} is child");
                }
            }

        }
        
        if (InternalCall.m_InternalCallIsKeyTriggered(keyCode.C))
        {
            intframespersecond = 1;
            InternalCall.m_InternalCallCloseWindow();
        }

        if (InternalCall.m_InternalCallIsKeyTriggered(keyCode.J))
        {
            InternalCall.m_InternalCallPlayAudio(EntityID, "TestLoop.wav");
        }
        if (InternalCall.m_InternalCallIsKeyTriggered(keyCode.K))
        {
            InternalCall.m_InternalCallStopAudio(EntityID, "TestLoop.wav");
        }

        if (InternalCall.m_InternalCallIsKeyTriggered(keyCode.E))
        {
            
            var collisionComponent = GetComponent.GetColliderComponent(EntityID);
            collisionComponent.m_collisionCheck = collisionComponent.m_collisionCheck? false : true;
            SetComponent.SetCollisionComponent(EntityID, collisionComponent);
        }

        Vector2 worldpos;
       InternalCall.m_InternalGetWorldMousePosition(out worldpos);

       //Console.WriteLine($"x is : {worldpos.X} , y is : {worldpos.Y}");



    }
}