using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Policy;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

public class PlayerBulletType2 : ScriptBase
{
    #region Entity ID
    private uint EntityID;

    public override void Awake(uint id)
    {
        EntityID = id;

        speed = 12;

        InternalCall.m_InternalGetTransformComponent(EntityID, out startingBulletPos, out startingBulletScale, out startingBulletRotate);

        InternalCall.m_InternalGetAnimationComponent(EntityID, out frameNumber, out framesPerSecond, out frameTimer, out isAnimating, out stripCount);

        rotation = startingBulletRotate;

        //Convert into radians
        rotationInRadians = (float)((rotation) * Math.PI / 180.0);

        //Get forward vector X
        forwardX = (float)(Math.Sin(rotationInRadians));

        //Get forward vector Y
        forwardY = (float)(Math.Cos(rotationInRadians));

        BulletRichochet = 5;
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

    private int BulletRichochet; // delete bullet once it reaches 0

    private int prevWall; //ignore previous wall entity

    public override void Start()
    {
        prevWall = -1;
    }

    public override void Update()
    {

        #region Movement in forward direction
        
        if (!isAnimating)
        {
            Vector2 movement;

            if (!InternalCall.m_InternalGetVelocity(EntityID, out movement))
            {
                // return cause velocity -> rigidbody is not present in entity
                return;
            }

            movement.X = 0 + forwardX * speed;
            movement.Y = 0 + forwardY * speed;

            InternalCall.m_InternalSetVelocity(EntityID, movement);
        }

        else
        {
            

            InternalCall.m_InternalGetAnimationComponent(EntityID, out frameNumber, out framesPerSecond, out frameTimer, out isAnimating, out stripCount);

            if (frameNumber == stripCount - 1)
            {
                BulletRichochet = 0;
                InternalCall.m_InternalCallDeleteEntity(EntityID);
                return;
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
                    case "PropGlassWall":
                    case "Wall":
                        if (prevWall == collidedEntitiesID) break;
                        prevWall = collidedEntitiesID; //ignore 6 frames

                        BulletRichochet--;
                        //richoceht logic

                        if(BulletRichochet > 0 )
                        {
                            //float richochetdegree = 135f;



                            //TransformComponent tc = Component.Get<TransformComponent>(EntityID);
                            //if ((tc.m_rotation > 90f && tc.m_rotation < 180f) || (tc.m_rotation > 270f && tc.m_rotation < 360f)) richochetdegree = -richochetdegree;


                            //tc.m_rotation = tc.m_rotation + richochetdegree;
                            //if (tc.m_rotation > 360) tc.m_rotation -= 360f;

                            //Console.WriteLine(tc.m_rotation);

                            //rotationInRadians = (float)((tc.m_rotation) * Math.PI / 180.0);
                            //forwardX = (float)(Math.Sin(rotationInRadians));
                            ////Get forward vector Y
                            //forwardY = (float)(Math.Cos(rotationInRadians));

                            //Component.Set<TransformComponent>(EntityID, tc);

                            Vector2 bulletpos, wallcenter;
                            InternalCall.m_InternalGetTranslate(EntityID, out bulletpos);
                            InternalCall.m_InternalGetTranslate((uint)collidedEntitiesID, out wallcenter);

                            ColliderComponent wallCC = Component.Get<ColliderComponent>((uint)collidedEntitiesID);
                            float halfWallWidth = wallCC.m_Size.X / 2f;
                            float halfWallHeight = wallCC.m_Size.Y / 2f;

                            Vector2 normal = GetRectangleNormal(bulletpos, wallcenter, halfWallWidth, halfWallHeight);
                            //Console.WriteLine(normal.X);
                            //Console.WriteLine(normal.Y);
                            Vector2 bulletVelocity;
                            InternalCall.m_InternalGetVelocity(EntityID, out bulletVelocity);

                            float newRotation = 0;

                            // Calculate the wall normal angle in degrees
                            float wallNormalAngle = (float)Math.Atan2(normal.Y, normal.X) * (180f / (float)Math.PI);

                            // Reflect the rotation using the general reflection formula
                            newRotation = 2 * wallNormalAngle - rotation;

                            //Console.WriteLine(rotation);
                            //set rotation to transform component

                            TransformComponent tc = Component.Get<TransformComponent>(EntityID);
                            tc.m_rotation = newRotation;
                            Component.Set<TransformComponent>(EntityID, tc);    

                            //Convert into radians
                            rotationInRadians = (float)((newRotation) * Math.PI / 180.0);

                            //Get forward vector X
                            forwardX = (float)(Math.Sin(rotationInRadians));

                            //Get forward vector Y
                            forwardY = (float)(Math.Cos(rotationInRadians));

                            rotation = newRotation;

                            //Vector2 bulletVelocity;
                            //InternalCall.m_InternalGetVelocity(EntityID, out bulletVelocity);

                            ////calculated dotproduct
                            //float dot = bulletVelocity.X * normal.X + bulletVelocity.Y * normal.Y;

                            //Vector2 returnvelo;
                            //returnvelo.X = bulletVelocity.X - 2.0f * dot * normal.X;
                            //returnvelo.Y = bulletVelocity.Y - 2.0f * dot * normal.Y;

                            //InternalCall.m_InternalSetVelocity(EntityID, in returnvelo);

                            //TransformComponent tc = Component.Get<TransformComponent>(EntityID);
                            //tc.m_rotation = (float)(Math.Atan2((double)returnvelo.X, (double)returnvelo.Y) * (180f / (float)Math.PI)) + 90f;



                            //rotationInRadians = (float)((tc.m_rotation) * Math.PI / 180.0);
                            //float forwardX = returnvelo.X;
                            //float forwardY = returnvelo.Y;

                            //// Calculate the magnitude of the vector
                            //float magnitude = (float)Math.Sqrt(forwardX * forwardX + forwardY * forwardY);
                            //Console.WriteLine(magnitude);
                            //// Check if magnitude is non-zero to avoid division by zero
                            //if (magnitude != 0)
                            //{
                            //    // Normalize the vector by dividing by the magnitude
                            //    forwardX /= magnitude;
                            //    forwardY /= magnitude;
                            //}
                            //else
                            //{
                            //    // If the magnitude is zero (the vector has no direction), we leave it as is or set to default (e.g., (0, 0))
                            //    forwardX = 0;
                            //    forwardY = 0;
                            //}

                            //Component.Set<TransformComponent>(EntityID, tc);

                            //Console.WriteLine("Bullet Velocity: " + bulletVelocity.X + ", " + bulletVelocity.Y);
                            //Console.WriteLine("Normal: " + normal.X + ", " + normal.Y);
                            //Console.WriteLine("Reflected Velocity: " + returnvelo.X + ", " + returnvelo.Y);
                            //Vector2 translate;
                            //InternalCall.m_InternalGetTranslate(EntityID, out translate);

                            //translate.X = translate.X + velocity.X * 2f;
                            //translate.Y = translate.Y + velocity.Y * 2f;

                            //InternalCall.m_InternalSetTranslate(EntityID, in translate);

                            //Console.WriteLine("richocet");

                        }

   


                        break;

                    case "Enemy":
                        BulletRichochet--;

                        break;

                    default:
                        break;
                }

                if (!isAnimating && BulletRichochet <= 0)
                {
                    isAnimating = true;
                    InternalCall.m_InternalSetAnimationComponent(EntityID, in frameNumber, in framesPerSecond, in frameTimer, in isAnimating, in stripCount);

                    Vector2 movement;

                    movement.X = 0;
                    movement.Y = 0;

                    InternalCall.m_InternalSetVelocity(EntityID, movement);

                    return;
                }
            }
        }

        #endregion

    }

    public Vector2 GetRectangleNormal(Vector2 bulletPos, Vector2 rectCenter, float halfWidth, float halfHeight)
    {
        float left = rectCenter.X - halfWidth;
        float right = rectCenter.X + halfWidth;
        float top = rectCenter.Y + halfHeight;
        float bottom = rectCenter.Y - halfHeight;

        //Console.WriteLine("Left: " + left);
        //Console.WriteLine("Right: " + right);
        //Console.WriteLine("Top: " + top);
        //Console.WriteLine("Bottom: " + bottom);

        //Console.WriteLine("bullet pos x: " + bulletPos.X + " bullet pos y: "+ bulletPos.Y);

        float dx = Math.Min(Math.Abs(bulletPos.X - left), Math.Abs(bulletPos.X - right));
        float dy = Math.Min(Math.Abs(bulletPos.Y - top), Math.Abs(bulletPos.Y - bottom));
        //Console.WriteLine(dx + " " + dy);
        if (dx < dy) // Closer to left/right edge
        {
            return (bulletPos.X < rectCenter.X) ? new Vector2(-1, 0) : new Vector2(1, 0); // Left or Right normal
        }
        else // Closer to top/bottom edge
        {
            return (bulletPos.Y < rectCenter.Y) ? new Vector2(0, -1) : new Vector2(0, 1); // Bottom or Top normal
        }
    }
}