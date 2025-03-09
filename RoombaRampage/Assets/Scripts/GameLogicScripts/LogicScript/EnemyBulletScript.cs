using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

public class EnemyBulletScript : ScriptBase
{
    private uint EntityID;
    private float bulletSpeed = 8.0f;
    private float bulletLifetime = 5.0f;
    private float currentLifetime = 0.0f;
    private Vector2 movement = new Vector2();

    public override void Awake(uint id)
    {
        EntityID = id;
    }

    public override void Start()
    {
        TransformComponent transformComp = Component.Get<TransformComponent>(EntityID);
        float rotationInRadians = (float)(transformComp.m_rotation * Math.PI / 180.0);
        float directionX = (float)Math.Sin(rotationInRadians);
        float directionY = (float)Math.Cos(rotationInRadians);

        movement.X = directionX * bulletSpeed;
        movement.Y = directionY * bulletSpeed;

        InternalCall.m_InternalSetVelocity(EntityID, in movement);
    }

    public override void Update()
    {
        currentLifetime += InternalCall.m_InternalCallGetDeltaTime();
        if (currentLifetime >= bulletLifetime)
        {
            InternalCall.m_InternalCallDeleteEntity(EntityID);
            return;
        }

        CheckForCollisions();
    }

    private void CheckForCollisions()
    {
        if (InternalCall.m_InternalCallIsCollided(EntityID) != 0.0f)
        {
            int[] collidedEntities = InternalCall.m_InternalCallGetCollidedEntities(EntityID);

            foreach (int collidedEntityID in collidedEntities)
            {
                string tag = InternalCall.m_InternalCallGetTag((uint)collidedEntityID);

                if (tag == "Player")
                {
                    // Play hit effect //AUDIOHERE
                    TransformComponent transformComp = Component.Get<TransformComponent>(EntityID);
                    InternalCall.m_InternalCallAddPrefab(
                        "prefab_bulletImpact",
                        transformComp.m_position.X,
                        transformComp.m_position.Y,
                        transformComp.m_rotation
                    );

                    InternalCall.m_InternalCallDeleteEntity(EntityID);
                    return;
                }
                else if (tag == "Wall" || tag == "Obstacle")
                {
                    // Hit environment, play impact effect //AUDIOHERE
                    TransformComponent transformComp = Component.Get<TransformComponent>(EntityID);
                    InternalCall.m_InternalCallAddPrefab(
                        "prefab_bulletImpact",
                        transformComp.m_position.X,
                        transformComp.m_position.Y,
                        transformComp.m_rotation
                    );

                    //This is to destroy the bullet
                    InternalCall.m_InternalCallDeleteEntity(EntityID);
                    return;
                }
            }
        }
    }
}