using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

public class CollisionResponse : ScriptBase
{
    private uint EntityID;

    public override void Awake(uint id)
    {
        EntityID = id;

        //InternalCall.m_InternalGetTransformComponent(EntityID, out startingPlayerPos, out startingPlayerScale, out startingPlayerRotate);
        //InternalCall.m_InternalGetRigidBodyComponent(EntityID, out playerVelocity, out playerAcceleration, out playerRotationalForce);
        //InternalCall.m_InternalGetColliderComponent(EntityID, out colSize, out colOffset, out colDraw, out colRadius, out colFlag, out isCollided, out colCheck);
    }

    private Vector2 startingPlayerPos;
    private Vector2 startingPlayerScale;
    private float startingPlayerRotate;

    private Vector2 playerVelocity;
    private Vector2 playerAcceleration;
    private float playerRotationalForce;

    private Vector2 colSize;
    private Vector2 colOffset;
    private bool colDraw;
    private float colRadius;
    private int colFlag;
    private float isCollided;
    private bool colCheck;



    public override void Start()
    {


    }

    public override void Update()
    {


        /*
         *  EntityID -> the game object
         *  WIth collision flag we know where it cant move 
         *  Rigidbody velocity to get the other direction 
         *  to find the distance to move
         *  time * velocity + position (snap back)
         *  set velocity to wtv 
         * 
         */
        Vector2 entity_Position, entity_Velocity, entity_Acceleration;
        uint entity_CollisionFlag;

        int steps = InternalCall.m_InternalCallGetSteps();
        float delta = InternalCall.m_InternalCallGetDeltaTime();
        if (steps > 1)
        {
            delta *= steps;
        }

        Vector2 entity_ColliderSize, entity_ColliderOffset, entity_DirectionVector;
        float entity_isCollided, entity_Radius, entity_Rotation;
        bool entity_drawDebug, entity_collisionCheck;

        InternalCall.m_InternalGetColliderComponent((uint)EntityID, out entity_ColliderSize, out entity_ColliderOffset, out entity_drawDebug, out entity_Radius,
        out entity_CollisionFlag, out entity_isCollided, out entity_collisionCheck , out entity_collisionCheck);

        InternalCall.m_InternalGetTransformComponent(EntityID, out startingPlayerPos, out startingPlayerScale, out startingPlayerRotate);

        InternalCall.m_InternalGetRigidBodyComponent(EntityID, out entity_Velocity, out entity_Acceleration, out entity_Rotation, out entity_Position, out entity_DirectionVector);
        
        RigidBodyComponent entity_Rigid = Component.Get<RigidBodyComponent>(EntityID);
        TransformComponent entity_Transform = Component.Get<TransformComponent>(EntityID);
        ColliderComponent entity_Collider = Component.Get<ColliderComponent>(EntityID);

        entity_Position = entity_Transform.m_position;
        entity_Velocity = entity_Rigid.m_Velocity;
        entity_Acceleration = entity_Rigid.m_Acceleration;
        entity_CollisionFlag = entity_Collider.m_blockedFlag;
        entity_DirectionVector = entity_Rigid.m_direction;


        Vector2 new_Position;
        Vector2 entity_Snapback;
        Vector2 new_Velocity;

        new_Velocity.X = entity_Velocity.X + (delta * entity_Acceleration.X);
        new_Velocity.Y = entity_Velocity.Y + (delta * entity_Acceleration.Y);

        //entity_Snapback.X = (delta * new_Velocity.X) + 0.001f;
        //entity_Snapback.Y = (delta * new_Velocity.Y) + 0.001f;
        //Console.WriteLine($"Position X  {entity_Position.X} T POS Y {entity_Position.Y}");
        //Console.WriteLine($"Snapback X  {entity_Snapback.X} T POS Y {entity_Snapback.Y}");
        //new_Position.X = entity_Position.X - entity_Snapback.X;
        //new_Position.Y = entity_Position.Y - entity_Snapback.Y;
        //Console.WriteLine($"New X  {new_Position.X} T POS Y {new_Position.Y}");

        Vector2 newVelo = entity_Velocity;

        ////I think need contact point..... sigh
        ///*
        // *
        // * 
        // * 
        // * 
        // * 
        // */




        //Console.WriteLine($"Velocity Pos: ({entity_Velocity.X}, {entity_Velocity.Y}))");
        //Console.WriteLine($"Flag: ({entity_CollisionFlag})");
        int direction = 0;
        if (entity_DirectionVector.Y > 0)
        {
            direction += 1;
        }
        if (entity_DirectionVector.X > 0)
        {
            direction += 2;
        }
        if (entity_DirectionVector.X < 0)
        {
            direction += 8;
        }
        if (entity_DirectionVector.Y < 0)
        {
            direction += 4;
        }
        switch (entity_CollisionFlag)
        {
            case 0:
                break;
            case 1: //TOP
                if (direction == 1)
                {
                    if (entity_Velocity.Y > 0)
                    {

                        //Vector2 testing;
                        newVelo.X = entity_Velocity.X;
                        newVelo.Y = 0;
                    }
                }
                else if (direction == 4)
                {
                    if (entity_Velocity.Y < 0)
                    {
                        newVelo.X = entity_Velocity.X;
                        newVelo.Y = 0;                 
                    }
                }
                else if (direction == 2)
                {
                    if (entity_Velocity.X > 0)
                    {
                        newVelo.X = 0;
                        newVelo.Y = entity_Velocity.Y;
                    }
                }
                else if (direction == 8)
                {
                    if (entity_Velocity.X < 0)
                    {
                        newVelo.X = 0;
                        newVelo.Y = entity_Velocity.Y;
                    }
                }
                else if (direction == 9)
                {
                    if (entity_Velocity.X < 0)
                    {
                        newVelo.X = 0;
             
                    }
                    if (entity_Velocity.Y > 0)
                    {
                        newVelo.Y = 0;
                    }
               
                }
                else if (direction == 3)
                {
                    if (entity_Velocity.X > 0)
                    {
                        newVelo.X = 0;
                    }
                    if (entity_Velocity.Y > 0)
                    {
                        newVelo.Y = 0;
                    }
                }
                else if (direction == 6)
                {
                    if (entity_Velocity.X > 0)
                    {
                        newVelo.X = 0;
                    }
                    if (entity_Velocity.Y < 0)
                    {
                        newVelo.Y = 0;
                    }

                }
                else if (direction == 12)
                {
                    if (entity_Velocity.X < 0)
                    {
                        newVelo.X = 0;
                    }
                    if (entity_Velocity.Y < 0)
                    {
                        newVelo.Y = 0;
                    }

                }

                entity_Snapback.X = (delta * new_Velocity.X) + 0.001f;
                entity_Snapback.Y = (delta * new_Velocity.Y) + 0.001f;
                new_Position.X = entity_Position.X;
                new_Position.Y = entity_Position.Y - entity_Snapback.Y;
                entity_Transform.m_position = new_Position;
                entity_Rigid.m_Velocity = newVelo;

                Component.Set<RigidBodyComponent>(EntityID, entity_Rigid);
                Component.Set<TransformComponent>(EntityID, entity_Transform);
                break;
            case 2: //RIGHT
                if (direction == 1)
                {
                    if (entity_Velocity.X > 0)
                    {
                        newVelo.X = 0;
                        newVelo.Y = entity_Velocity.Y;

                    }
                }
                else if (direction == 4)
                {
                    if (entity_Velocity.X < 0)
                    {
                        newVelo.X = 0;
                        newVelo.Y = entity_Velocity.Y;
                    }
                }
                else if (direction == 2)
                {
                    if (entity_Velocity.Y < 0)
                    {
                        newVelo.X = entity_Velocity.X;
                        newVelo.Y = 0;
 
                    }
                }
                else if (direction == 8)
                {
                    if (entity_Velocity.Y > 0)
                    {
                        newVelo.X = entity_Velocity.X;
                        newVelo.Y = 0;

                    }
                }
                else if (direction == 9)
                {
                    if (entity_Velocity.X > 0)
                    {
                        newVelo.X = 0;
                    }
                    if (entity_Velocity.Y > 0)
                    {
                        newVelo.Y = 0;
                    }

                }
                else if (direction == 3)
                {
                    if (entity_Velocity.X > 0)
                    {
                        newVelo.X = 0;
                    }
                    if (entity_Velocity.Y < 0)
                    {
                        newVelo.Y = 0;
                    }

                }
                else if (direction == 6)
                {
                    if (entity_Velocity.X < 0)
                    {
                        newVelo.X = 0;
                    }
                    if (entity_Velocity.Y < 0)
                    {
                        newVelo.Y = 0;
                    }

                }
                else if (direction == 12)
                {
                    if (entity_Velocity.X < 0)
                    {
                        newVelo.X = 0;
                    }
                    if (entity_Velocity.Y > 0)
                    {
                        newVelo.Y = 0;
                    }

                }
                entity_Snapback.X = (delta * new_Velocity.X) + 0.001f;
                entity_Snapback.Y = (delta * new_Velocity.Y) + 0.001f;
                new_Position.X = entity_Position.X - entity_Snapback.X;
                new_Position.Y = entity_Position.Y ;
                entity_Transform.m_position = new_Position;
                entity_Rigid.m_Velocity = newVelo;

                Component.Set<RigidBodyComponent>(EntityID, entity_Rigid);
                Component.Set<TransformComponent>(EntityID, entity_Transform);
                break;
            case 4: // BOTTOM
                if (direction == 1)
                {
                    if (entity_Velocity.Y < 0)
                    {
                        newVelo.X = entity_Velocity.X;
                        newVelo.Y = 0;

                    }
                }
                else if (direction == 4)
                {
                    if (entity_Velocity.Y > 0)
                    {
                        newVelo.X = entity_Velocity.X;
                        newVelo.Y = 0;
                    }
                }
                else if (direction == 2)
                {
                    if (entity_Velocity.X < 0)
                    {
                        newVelo.X = 0;
                        newVelo.Y = entity_Velocity.Y;

                    }
                }
                else if (direction == 8)
                {
                    if (entity_Velocity.X > 0)
                    {
                        newVelo.X = 0;
                        newVelo.Y = entity_Velocity.Y;
                    }
                }
                else if (direction == 9)
                {
                    if (entity_Velocity.X > 0)
                    {
                        newVelo.X = 0;
                    }
                    if (entity_Velocity.Y < 0)
                    {
                        newVelo.Y = 0;
                    }

                }
                else if (direction == 3)
                {

                    if (entity_Velocity.X < 0)
                    {
                        newVelo.X = 0;
                    }
                    if (entity_Velocity.Y < 0)
                    {
                        newVelo.Y = 0;
                    }

                }
                else if (direction == 6)
                {
                    if (entity_Velocity.X < 0)
                    {
                        newVelo.X = 0;
                    }
                    if (entity_Velocity.Y > 0)
                    {
                        newVelo.Y = 0;
                    }

                }
                else if (direction == 12)
                {
                    if (entity_Velocity.X > 0)
                    {
                        newVelo.X = 0;
                    }
                    if (entity_Velocity.Y > 0)
                    {
                        newVelo.Y = 0;
                    }

                }
                entity_Snapback.X = (delta * new_Velocity.X) + 0.001f;
                entity_Snapback.Y = (delta * new_Velocity.Y) + 0.001f;
                new_Position.X = entity_Position.X ;
                new_Position.Y = entity_Position.Y + entity_Snapback.Y;
                entity_Transform.m_position = new_Position;
                entity_Rigid.m_Velocity = newVelo;

                Component.Set<RigidBodyComponent>(EntityID, entity_Rigid);
                Component.Set<TransformComponent>(EntityID, entity_Transform);
                break;
            case 8: //LEFT
                if (direction == 1)
                {
                    if (entity_Velocity.X < 0)
                    {
                        newVelo.X = 0;
                        newVelo.Y = entity_Velocity.Y;
 
                    }
                }
                else if (direction == 4)
                {
                    if (entity_Velocity.X > 0)
                    {
                        newVelo.X = 0;
                        newVelo.Y = entity_Velocity.Y;

                    }
                }
                else if (direction == 2)
                {
                    if (entity_Velocity.Y > 0)
                    {
                        newVelo.X = entity_Velocity.X;
                        newVelo.Y = 0;

                    }
                }
                else if (direction == 8)
                {
                    if (entity_Velocity.Y < 0)
                    {
                        newVelo.X = entity_Velocity.X;
                        newVelo.Y = 0;

                    }
                }
                else if (direction == 9)
                {
                    if (entity_Velocity.X < 0)
                    {
                        newVelo.X = 0;
                    }
                    if (entity_Velocity.Y < 0)
                    {
                        newVelo.Y = 0;
                    }

                }
                else if (direction == 3)
                {
                    if (entity_Velocity.X < 0)
                    {
                        newVelo.X = 0;
                    }
                    if (entity_Velocity.Y > 0)
                    {
                        newVelo.Y = 0;
                    }

                }
                else if (direction == 6)
                {

                    if (entity_Velocity.X > 0)
                    {
                        newVelo.X = 0;
                    }
                    if (entity_Velocity.Y > 0)
                    {
                        newVelo.Y = 0;
                    }
  
                }
                else if (direction == 12)
                {
                    if (entity_Velocity.X > 0)
                    {
                        newVelo.X = 0;
                    }
                    if (entity_Velocity.Y < 0)
                    {
                        newVelo.Y = 0;
                    }

                }
                entity_Snapback.X = (delta * new_Velocity.X) + 0.001f;
                entity_Snapback.Y = (delta * new_Velocity.Y) + 0.001f;
                new_Position.X = entity_Position.X + entity_Snapback.X;
                new_Position.Y = entity_Position.Y;
                entity_Transform.m_position = new_Position;
                entity_Rigid.m_Velocity = newVelo;

                Component.Set<RigidBodyComponent>(EntityID, entity_Rigid);
                Component.Set<TransformComponent>(EntityID, entity_Transform);
                break;
            case 3: //top right
                if (direction == 1)
                {
                    if (entity_Velocity.X > 0)
                    {
                        newVelo.X = 0;
                    }
                    if (entity_Velocity.Y > 0)
                    {
                        newVelo.Y = 0;
                    }

                }
                else if (direction == 4)
                {
                    if (entity_Velocity.X < 0)
                    {
                        newVelo.X = 0;
                    }
                    if (entity_Velocity.Y < 0)
                    {
                        newVelo.Y = 0;
                    }

                }
                else if (direction == 2)
                {
                    if (entity_Velocity.X > 0)
                    {
                        newVelo.X = 0;
                    }
                    if (entity_Velocity.Y < 0)
                    {
                        newVelo.Y = 0;
                    }

                }
                else if (direction == 8)
                {
                    if (entity_Velocity.X < 0)
                    {
                        newVelo.X = 0;
                    }
                    if (entity_Velocity.Y > 0)
                    {
                        newVelo.Y = 0;
                    }

                }
                else if (direction == 9)
                {
                    if (entity_Velocity.Y > 0)
                    {
                        newVelo.Y = 0;
                    }

                }
                else if (direction == 3)
                {
                    if (entity_Velocity.X > 0)
                    {
                        newVelo.X = 0;
                    }

                }
                else if (direction == 6)
                {
                    if (entity_Velocity.Y < 0)
                    {
                        newVelo.Y = 0;
                    }
 
                }
                else if (direction == 12)
                {
                    if (entity_Velocity.X < 0)
                    {
                        newVelo.X = 0;
                    }

                }
                entity_Snapback.X = (delta * new_Velocity.X) + 0.001f;
                entity_Snapback.Y = (delta * new_Velocity.Y) + 0.001f;
                new_Position.X = entity_Position.X - entity_Snapback.X;
                new_Position.Y = entity_Position.Y - entity_Snapback.Y;
                entity_Transform.m_position = new_Position;
                entity_Rigid.m_Velocity = newVelo;
                Component.Set<RigidBodyComponent>(EntityID, entity_Rigid);
                Component.Set<TransformComponent>(EntityID, entity_Transform);
                break;
            case 6: //bottom right
                if (direction == 1)
                {
                    if (entity_Velocity.X > 0)
                    {
                        newVelo.X = 0;
                    }
                    if (entity_Velocity.Y < 0)
                    {
                        newVelo.Y = 0;
                    }

                }
                else if (direction == 4)
                {
                    if (entity_Velocity.X < 0)
                    {
                        newVelo.X = 0;
                    }
                    if (entity_Velocity.Y > 0)
                    {
                        newVelo.Y = 0;
                    }

                }
                else if (direction == 2)
                {
                    if (entity_Velocity.X < 0)
                    {
                        newVelo.X = 0;
                    }
                    if (entity_Velocity.Y < 0)
                    {
                        newVelo.Y = 0;
                    }

                }
                else if (direction == 8)
                {
                    if (entity_Velocity.X > 0)
                    {
                        newVelo.X = 0;
                    }
                    if (entity_Velocity.Y > 0)
                    {
                        newVelo.Y = 0;
                    }
  
                }
                else if (direction == 9)
                {
                    if (entity_Velocity.X > 0)
                    {
                        newVelo.X = 0;
                    }
                   
                }
                else if (direction == 3)
                {
                    if (entity_Velocity.Y < 0)
                    {
                        newVelo.Y = 0;
                    }
                   
                }
                else if (direction == 6)
                {
                    if (entity_Velocity.X < 0)
                    {
                        newVelo.X = 0;
                    }

                }
                else if (direction == 12)
                {
                    if (entity_Velocity.Y > 0)
                    {
                        newVelo.Y = 0;
                    }

                }
                entity_Snapback.X = (delta * new_Velocity.X) + 0.001f;
                entity_Snapback.Y = (delta * new_Velocity.Y) + 0.001f;
                new_Position.X = entity_Position.X - entity_Snapback.X;
                new_Position.Y = entity_Position.Y + entity_Snapback.Y;
                entity_Transform.m_position = new_Position;
                entity_Rigid.m_Velocity = newVelo;

                Component.Set<RigidBodyComponent>(EntityID, entity_Rigid);
                Component.Set<TransformComponent>(EntityID, entity_Transform);
                break;
            case 12: // bottom left
                if (direction == 1)
                {
                    if (entity_Velocity.X < 0)
                    {
                        newVelo.X = 0;
                    }
                    if (entity_Velocity.Y < 0)
                    {
                        newVelo.Y = 0;
                    }

                }
                else if (direction == 4)
                {
                    if (entity_Velocity.X > 0)
                    {
                        newVelo.X = 0;
                    }
                    if (entity_Velocity.Y > 0)
                    {
                        newVelo.Y = 0;
                    }

                }
                else if (direction == 2)
                {
                    if (entity_Velocity.X < 0)
                    {
                        newVelo.X = 0;
                    }
                    if (entity_Velocity.Y > 0)
                    {
                        newVelo.Y = 0;
                    }

                }
                else if (direction == 8)
                {
                    if (entity_Velocity.X > 0)
                    {
                        newVelo.X = 0;
                    }
                    if (entity_Velocity.Y < 0)
                    {
                        newVelo.Y = 0;
                    }

                }
                else if (direction == 9)
                {
                    if (entity_Velocity.Y < 0)
                    {
                        newVelo.Y = 0;
                    }

                }
                else if (direction == 3)
                {
                    if (entity_Velocity.X < 0)
                    {
                        newVelo.X = 0;
                    }

                }
                else if (direction == 6)
                {
                    if (entity_Velocity.Y > 0)
                    {
                        newVelo.Y = 0;
                    }

                }
                else if (direction == 12)
                {
                    if (entity_Velocity.X > 0)
                    {
                        newVelo.X = 0;
                    }

                }
                entity_Snapback.X = (delta * new_Velocity.X) + 0.001f;
                entity_Snapback.Y = (delta * new_Velocity.Y) + 0.001f;
                new_Position.X = entity_Position.X + entity_Snapback.X;
                new_Position.Y = entity_Position.Y + entity_Snapback.Y;
                entity_Transform.m_position = new_Position;
                entity_Rigid.m_Velocity = newVelo;

                Component.Set<RigidBodyComponent>(EntityID, entity_Rigid);
                Component.Set<TransformComponent>(EntityID, entity_Transform);
                break;
            case 9: // top left
                if (direction == 1)
                {
                    if (entity_Velocity.X < 0)
                    {
                        newVelo.X = 0;
                    }
                    if (entity_Velocity.Y > 0)
                    {
                        newVelo.Y = 0;
                    }

                }
                else if (direction == 4)
                {
                    if (entity_Velocity.X > 0)
                    {
                        newVelo.X = 0;
                    }
                    if (entity_Velocity.Y < 0)
                    {
                        newVelo.Y = 0;
                    }

                }
                else if (direction == 2)
                {
                    if (entity_Velocity.X > 0)
                    {
                        newVelo.X = 0;
                    }
                    if (entity_Velocity.Y > 0)
                    {
                        newVelo.Y = 0;
                    }

                }
                else if (direction == 8)
                {
                    if (entity_Velocity.X < 0)
                    {
                        newVelo.X = 0;
                    }
                    if (entity_Velocity.Y < 0)
                    {
                        newVelo.Y = 0;
                    }
         
                }
                else if (direction == 9)
                {
                    if (entity_Velocity.X < 0)
                    {
                        newVelo.X = 0;
                    }

                }
                else if (direction == 3)
                {
                    if (entity_Velocity.Y > 0)
                    {
                        newVelo.Y = 0;
                    }

                }
                else if (direction == 6)
                {
                    if (entity_Velocity.X > 0)
                    {
                        newVelo.X = 0;
                    }

                }
                else if (direction == 12)
                {
                    if (entity_Velocity.Y < 0)
                    {
                        newVelo.Y = 0;
                    }

                }
                entity_Snapback.X = (delta * new_Velocity.X) + 0.001f;
                entity_Snapback.Y = (delta * new_Velocity.Y) + 0.001f;
                new_Position.X = entity_Position.X + entity_Snapback.X;
                new_Position.Y = entity_Position.Y - entity_Snapback.Y;
                entity_Transform.m_position = new_Position;
                entity_Rigid.m_Velocity = newVelo;

                Component.Set<RigidBodyComponent>(EntityID, entity_Rigid);
                Component.Set<TransformComponent>(EntityID, entity_Transform);
                break;
            default:
                entity_Snapback.X = (delta * new_Velocity.X) + 0.001f;
                entity_Snapback.Y = (delta * new_Velocity.Y) + 0.001f;
                new_Position.X = entity_Position.X - entity_Snapback.X;
                new_Position.Y = entity_Position.Y - entity_Snapback.Y;
                entity_Transform.m_position = new_Position;
                entity_Rigid.m_Velocity = newVelo;

                entity_Transform.m_position = new_Position;
                entity_Rigid.m_Velocity = newVelo;

                Component.Set<RigidBodyComponent>(EntityID, entity_Rigid);
                Component.Set<TransformComponent>(EntityID, entity_Transform);
                break;
        }

        //Setting the position + velocity
        //Console.WriteLine($"T POS X  {entity_Transform.m_position.X} T POS Y {entity_Transform.m_position.Y}");
        //Console.WriteLine($"N pOS X  {new_Position.X} T POS Y {new_Position.Y}");





        //Console.WriteLine("K");
        #region saved code previous collision response
        //if (InternalCall.m_InternalCallGetTagIDs("Wall") != null)
        //{
        //    int[] wallEntities = InternalCall.m_InternalCallGetTagIDs("Wall");
        //    foreach (int wallEntitiesID in wallEntities)
        //    {
        //        Vector2 wallSize, wallOffset, wallPos, wallScale;
        //        float wallRadius, wallRotate, wallCollided;
        //        bool wallDraw, wallCheck;
        //        uint wallFlag;

        //        InternalCall.m_InternalGetColliderComponent((uint)wallEntitiesID, out wallSize, out wallOffset, out wallDraw, out wallRadius, out wallFlag, out wallCollided, out wallCheck);
        //        InternalCall.m_InternalGetTransformComponent((uint)wallEntitiesID, out wallPos, out wallScale, out wallRotate);

        //        float xMin = wallPos.X + wallOffset.X - (wallSize.X * wallScale.X) / 2;
        //        float xMax = wallPos.X + wallOffset.X + (wallSize.X * wallScale.X) / 2;
        //        float yMin = wallPos.Y + wallOffset.Y - (wallSize.Y * wallScale.Y) / 2;
        //        float yMax = wallPos.Y + wallOffset.Y + (wallSize.Y * wallScale.Y) / 2;

        //        float exMin = xMin - colRadius;
        //        float exMax = xMax + colRadius;
        //        float eyMin = yMin - colRadius;
        //        float eyMax = yMax + colRadius;

        //        // Console.WriteLine($"Wall Pos: ({wallPos.X}, {wallPos.Y}), Wall Size: ({wallSize.X}, {wallSize.Y})");
        //        //Console.WriteLine($"Expanded AABB: ({exMin}, {exMax}, {eyMin}, {eyMax})");



        //        Vector2 playerVelocity;
        //        InternalCall.m_InternalGetVelocity(EntityID, out playerVelocity);
        //        Vector2 startingPlayerPos, startingPlayerScale;
        //        float startingPlayerRotate;
        //        InternalCall.m_InternalGetTransformComponent(EntityID, out startingPlayerPos, out startingPlayerScale, out startingPlayerRotate);


        //        int steps = InternalCall.m_InternalCallGetSteps();
        //        float delta = InternalCall.m_InternalCallGetDeltaTime();
        //        if (steps > 1)
        //        {
        //            delta *= steps;
        //            //Console.WriteLine(steps);
        //        }
        //        float cxStart = startingPlayerPos.X;
        //        float cyStart = startingPlayerPos.Y;
        //        float cxEnd = cxStart + playerVelocity.X * delta;
        //        float cyEnd = cyStart + playerVelocity.Y * delta;

        //        // Console.WriteLine(delta);

        //        if (!(cxEnd > exMin && cxEnd < exMax && cyEnd < eyMax && cyEnd > eyMin))
        //        {
        //            continue;
        //        }
        //        else if ((cxStart > xMin + colRadius && cxStart < xMax - colRadius && cyStart < eyMax - colRadius && cyStart > eyMin + colRadius)) //Completely Inside
        //        {
        //            // Circle's position (cxStart, cyStart) and radius
        //            float circleRadius = colRadius;

        //            // Calculate distances from the circle to each edge of the AABB
        //            float distLeft = cxStart - exMin;
        //            float distRight = exMax - cxStart;
        //            float distTop = eyMax - cyStart;
        //            float distBottom = cyStart - eyMin;

        //            // Determine the closest edge
        //            float minDist = Math.Min(Math.Min(distLeft, distRight), Math.Min(distTop, distBottom));

        //            // Push the circle outside the closest edge
        //            if (minDist == distLeft)
        //            {
        //                cxStart = exMin - circleRadius; // Push left
        //            }
        //            else if (minDist == distRight)
        //            {
        //                cxStart = exMax + circleRadius; // Push right
        //            }
        //            else if (minDist == distTop)
        //            {
        //                cyStart = eyMax + circleRadius; // Push up
        //            }
        //            else if (minDist == distBottom)
        //            {
        //                cyStart = eyMin - circleRadius; // Push down
        //            }

        //            // Update the entity's position
        //            Vector2 newPos = new Vector2(cxStart, cyStart);
        //            InternalCall.m_InternalSetTranslate(EntityID, in newPos);
        //            continue;
        //        }
        //        else if ((cxStart > exMin && cxStart < exMax && cyStart < eyMax && cyStart > eyMin)) // Slightly Outside
        //        {
        //            // Circle's position (cxStart, cyStart) and radius
        //            float circleRadius = colRadius;

        //            float closestX = cxStart;
        //            if (cxStart < xMin) closestX = xMin;
        //            if (cxStart > xMax) closestX = xMax;

        //            float closestY = cyStart;
        //            if (cyStart < yMin) closestY = yMin;
        //            if (cyStart > yMax) closestY = yMax;

        //            //Console.WriteLine(eyMax);

        //            // Calculate the vector from the circle's center to the closest point on the AABB
        //            float offsetX = cxStart - closestX;
        //            float offsetY = cyStart - closestY;

        //            // Calculate the distance from the circle's center to the closest point on the AABB
        //            float distance = (float)Math.Sqrt(offsetX * offsetX + offsetY * offsetY);

        //            // If the circle is overlapping the AABB, move it outside
        //            if (distance < circleRadius)
        //            {
        //                // Calculate the amount to push the circle outside of the AABB
        //                float pushDistance = circleRadius - distance;

        //                // Normalize the offset to get the direction
        //                if (distance == 0)
        //                {
        //                    continue;
        //                }
        //                float normalizedX = offsetX / distance;
        //                float normalizedY = offsetY / distance;

        //                // Apply the push to the circle’s position (move it outside the AABB)
        //                cxStart += normalizedX * pushDistance * 1.01f;
        //                cyStart += normalizedY * pushDistance * 1.01f;



        //                // Update the entity's position
        //                Vector2 newPos;
        //                newPos.X = cxStart;
        //                newPos.Y = cyStart;
        //                InternalCall.m_InternalSetTranslate(EntityID, in newPos);
        //            }
        //            continue;
        //        }



        //        //Console.WriteLine($"Start Pos: ({cxStart}, {cyStart}), End Pos: ({cxEnd}, {cyEnd})");

        //        //Console.WriteLine($"Velocity: ({playerVelocity.X}, {playerVelocity.Y}), Delta: {delta}");


        //        float dx = cxEnd - cxStart;
        //        float dy = cyEnd - cyStart;
        //        float tMin = 0.0f, tMax = 1.0f;

        //        if (dx != 0.0f)
        //        {
        //            float t1 = (exMin - cxStart) / dx;
        //            float t2 = (exMax - cxStart) / dx;
        //            if (t1 > t2) (t1, t2) = (t2, t1);
        //            tMin = Math.Max(tMin, t1);
        //            tMax = Math.Min(tMax, t2);
        //            // Console.WriteLine($"Axis: X, t1: {t1}, t2: {t2}, tMin: {tMin}, tMax: {tMax}");
        //        }

        //        if (dy != 0.0f)
        //        {
        //            float t1 = (eyMin - cyStart) / dy;
        //            float t2 = (eyMax - cyStart) / dy;
        //            if (t1 > t2) (t1, t2) = (t2, t1);
        //            tMin = Math.Max(tMin, t1);
        //            tMax = Math.Min(tMax, t2);
        //            // Console.WriteLine($"Axis: Y, t1: {t1}, t2: {t2}, tMin: {tMin}, tMax: {tMax}");
        //        }



        //        if (tMin <= tMax + 0.001f)
        //        {
        //            float tImpact = tMin;
        //            float newPosX = cxStart + tImpact * dx;
        //            float newPosY = cyStart + tImpact * dy;

        //            // Console.WriteLine($"Start Pos: ({cxStart}, {cyStart}), End Pos: ({newPosX}, {newPosY})");

        //            Vector2 newPos;
        //            newPos.X = newPosX;
        //            newPos.Y = newPosY;

        //            InternalCall.m_InternalSetTranslate(EntityID, in newPos);

        //            Vector2 movement;

        //            movement.X = 0;
        //            movement.Y = 0;

        //            if (tMax != 0)
        //            {
        //                InternalCall.m_InternalSetVelocity(EntityID, movement);
        //            }

        //        }
        //    }
        //}
        #endregion
    }
}

