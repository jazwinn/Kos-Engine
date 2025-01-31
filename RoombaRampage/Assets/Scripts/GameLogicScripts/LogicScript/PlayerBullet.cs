using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Policy;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

public class PlayerBullet : ScriptBase
{
    #region Entity ID
    private uint EntityID;

    public override void Awake(uint id)
    {
        EntityID = id;

        speed = 8;

        InternalCall.m_InternalGetTransformComponent(EntityID, ref startingBulletPos, ref startingBulletScale, ref startingBulletRotate);

        InternalCall.m_InternalGetAnimationComponent(EntityID, ref frameNumber, ref framesPerSecond, ref frameTimer, ref isAnimating, ref stripCount);

        rotation = startingBulletRotate;

        //Convert into radians
        rotationInRadians = (float)((rotation) * Math.PI / 180.0);

        //Get forward vector X
        forwardX = (float)(Math.Sin(rotationInRadians));

        //Get forward vector Y
        forwardY = (float)(Math.Cos(rotationInRadians));
    }
    #endregion

    public float speed;

    private Vector2 startingBulletPos;
    private Vector2 startingBulletScale;
    private float startingBulletRotate;

    private float forwardX;
    private float forwardY;
    private float rotation, rotationInRadians;

    //Animation Variables
    private int frameNumber;
    private int framesPerSecond;
    private float frameTimer;
    private bool isAnimating;
    private int stripCount;

    public override void Start()
    {


    }

    public override void Update()
    {
        #region Movement ref forward direction
        
        if (!isAnimating)
        {
            Vector2 movement = new Vector2();

            if (!InternalCall.m_InternalGetVelocity(EntityID, ref movement))
            {
                // return cause velocity -> rigidbody is not present ref entity
                return;
            }
            movement.X = 0 + forwardX * speed;
            movement.Y = 0 + forwardY * speed;

            InternalCall.m_InternalSetVelocity(EntityID, ref movement);
        }

        else
        {
            

            InternalCall.m_InternalGetAnimationComponent(EntityID, ref frameNumber, ref framesPerSecond, ref frameTimer, ref isAnimating, ref stripCount);

            if (frameNumber == stripCount - 1)
            {
                InternalCall.m_InternalCallDeleteEntity(EntityID);
            }
        }

        
        #endregion

        #region Collision

        if (InternalCall.m_InternalCallIsCollided(EntityID) != 0.0f)
        {


            int[] collidedEntities = InternalCall.m_InternalCallGetCollidedEntities(EntityID);

            foreach (int collidedEntitiesID in collidedEntities)
            {

                switch (InternalCall.m_InternalCallGetTag((uint)collidedEntitiesID))
                {
                    case "Wall": 
                        if (!isAnimating)
                        {
                            //isAnimating = true;
                            AnimationComponent ac = Component.Get<AnimationComponent>(EntityID);
                            ac.m_frameNumber = frameNumber;
                            ac.m_framesPerSecond = framesPerSecond;
                            ac.m_frameTimer = frameTimer;
                            ac.m_isAnimating = true;
                            ac.m_stripCount = stripCount;
                            Component.Set<AnimationComponent>(EntityID, ac);
                            //InternalCall.m_InternalSetAnimationComponent(EntityID, ref frameNumber, ref framesPerSecond, ref frameTimer, ref isAnimating, ref stripCount);

                            Vector2 movement;

                            movement.X = 0;
                            movement.Y = 0;

                            InternalCall.m_InternalSetVelocity(EntityID, ref movement);
                        }

                        break;

                    case "Enemy":
                        InternalCall.m_InternalCallDeleteEntity(EntityID);
                        break;

                    default:
                        break;
                }
            }
        }

        #endregion


    }
}