using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text.RegularExpressions;

public class BossBullet : ScriptBase
{
    #region Entity ID
    private uint EntityID;

    public override void Awake(uint id)
    {
        EntityID = id;

        speed = 2;

        InternalCall.m_InternalGetTransformComponent(EntityID, out startingBulletPos, out startingBulletScale, out startingBulletRotate);

        rotation = startingBulletRotate;

        rotationInRadians = (float)(rotation * Math.PI / 180.0);

        InternalCall.m_InternalGetTransformComponent(EntityID, out startingBulletPos, out startingBulletScale, out startingBulletRotate);


        // Calculate forward movement
        forwardX = (float)Math.Cos(rotationInRadians);
        forwardY = (float)Math.Sin(rotationInRadians);

        Console.WriteLine($"[Bullet Spawned] Position: X={startingBulletPos.X}, Y={startingBulletPos.Y}, Rotation={rotation}°");
        Console.WriteLine($"[Bullet Direction] ForwardX={forwardX}, ForwardY={forwardY}");

        //if(EntityID !=)
        //CoroutineManager.Instance.StartCoroutine(DeathTimer());
    }
    #endregion

    public float speed;
    private Vector2 startingBulletPos;
    private Vector2 startingBulletScale;
    private float startingBulletRotate;

    private float forwardX;
    private float forwardY;
    private float rotation, rotationInRadians;

    private bool bulletHasHit;

    public override void Start()
    {
        bulletHasHit = false;

    }

    public override void Update()
    {
        if (bulletHasHit) return;

        #region Movement in Forward Direction
        Vector2 movement;

        if (!InternalCall.m_InternalGetVelocity(EntityID, out movement))
        {
            return;
        }

        movement.X = forwardX * speed;
        movement.Y = forwardY * speed;

        InternalCall.m_InternalSetVelocity(EntityID, movement);
        #endregion

        #region Collision Handling
        if (InternalCall.m_InternalCallIsCollided(EntityID) != 0.0f)
        {
            int[] collidedEntities = InternalCall.m_InternalCallGetCollidedEntities(EntityID);

            foreach (int collidedEntitiesID in collidedEntities)
            {
                switch (InternalCall.m_InternalCallGetTag((uint)collidedEntitiesID))
                {
                    case "PropGlassWall":
                    case "Wall":
                        bulletHasHit = true;

                        InternalCall.m_InternalCallDeleteEntity(EntityID);
                        return;

                    case "Player": // If it hits the player, delete it
                        bulletHasHit = true;
                        InternalCall.m_InternalCallDeleteEntity(EntityID);
                        return;

                    default:
                        break;
                }
            }
        }
        #endregion
    }

    private IEnumerator DeathTimer()
    {
        yield return new CoroutineManager.WaitForSeconds(3.0f);
        InternalCall.m_InternalCallDeleteEntity(EntityID);
    }
}

