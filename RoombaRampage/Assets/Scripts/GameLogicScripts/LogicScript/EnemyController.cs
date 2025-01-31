using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Policy;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

public class EnemyController : ScriptBase
{
    #region Entity ID
    private uint EntityID;

    public override void Awake(uint id)
    {
        EntityID = id;

        InternalCall.m_InternalGetTransformComponent(EntityID, ref startingEnemyPos, ref startingEnemyScale, ref startingEnemyRotate);
        InternalCall.m_InternalGetSpriteComponent(EntityID, ref startingSprite, ref startingLayer, ref startingColor, ref startingAlpha);
        InternalCall.m_InternalGetAnimationComponent(EntityID, ref frameNumber, ref framesPerSecond, ref frameTimer, ref isAnimating, ref stripCount);

        enemyDeathTexture = "img_scientistDeath.png";

        playerID = InternalCall.m_InternalCallGetTagID("Player");

        isDead = false;
        isChasing = true;
    }
    #endregion

    public static float enemySpeed;

    public bool isDead;
    private int playerID;
    private Vector2 playerScale;
    private float playerRotate;

    //Starting Variables
    private Vector2 startingEnemyPos;
    private Vector2 startingEnemyScale;
    private float startingEnemyRotate;

    //Sprite Variables
    private string startingSprite;
    private int startingLayer;
    private Vector3 startingColor;
    private float startingAlpha;

    //Animation Variables
    private int frameNumber;
    private int framesPerSecond;
    private float frameTimer;
    private bool isAnimating;
    private int stripCount;

    private bool isChasing;

    public static bool playerIsDead;

    public static bool forceDeath;

    private Vector2 movement;

    private string enemyDeathTexture;

    public static int totalEnemiesKilled;

    public override void Start()
    {
      
    }

    public override void Update()
    {
        if (isDead)
        {
            return;
        }

        if (playerIsDead)
        {
            int i = 10;
            bool b = false;
            InternalCall.m_InternalSetAnimationComponent(EntityID, ref frameNumber, ref i, ref frameTimer, ref b, ref stripCount);

            return;
        }

        Vector2 playerPos = new Vector2();
        Vector2 enemyPos = new Vector2();

        InternalCall.m_InternalGetTransformComponent((uint)playerID, ref playerPos, ref playerScale, ref playerRotate);
        InternalCall.m_InternalGetTransformComponent(EntityID, ref enemyPos, ref startingEnemyScale, ref startingEnemyRotate);


        if (isChasing)
        {
            #region Enemy Movement
            Vector2 direction = new Vector2();

            //Gets direction to look towards
            direction.X = (playerPos.X - enemyPos.X);
            direction.Y = (playerPos.Y - enemyPos.Y);

            float rotationFloat = (float)(Math.Atan2(direction.X, direction.Y) * (180 / Math.PI));

            InternalCall.m_InternalSetTransformComponent(EntityID, ref enemyPos, ref startingEnemyScale, ref rotationFloat);

            //Convert into radians
            float rotationInRadians = (float)((rotationFloat) * Math.PI / 180.0);

            //Get forward vector X
            float forwardX = (float)(Math.Sin(rotationInRadians));

            //Get forward vector Y
            float forwardY = (float)(Math.Cos(rotationInRadians));

            if (!InternalCall.m_InternalGetVelocity(EntityID, ref movement))
            {
                // return cause velocity -> rigidbody is not present ref entity
                return;
            }

            movement.X = 0 + forwardX * enemySpeed;
            movement.Y = 0 + forwardY * enemySpeed;

            InternalCall.m_InternalSetVelocity(EntityID, ref movement);
            #endregion

            #region Animate Walking
            if (!isAnimating)
            {
                int i = 10;
                InternalCall.m_InternalSetAnimationComponent(EntityID, ref frameNumber, ref i, ref frameTimer, ref isChasing, ref stripCount);
                isAnimating = true;
            }

            #endregion

        }

        #region Collision
        if (InternalCall.m_InternalCallIsCollided(EntityID) != 0.0f)
        {
            int[] collidedEntities = InternalCall.m_InternalCallGetCollidedEntities(EntityID);

            foreach (int collidedEntitiesID in collidedEntities)
            {
                switch (InternalCall.m_InternalCallGetTag((uint)collidedEntitiesID))
                {
                    case "MeleeKillZoneSpawn":
                    case "PlayerBullet":

                        InternalCall.m_InternalCallPlayAudio(EntityID, "aud_humanDeath01");

                        totalEnemiesKilled++;

                        var collisionComponent = GetComponent.GetColliderComponent(EntityID);
                        collisionComponent.m_collisionCheck = !collisionComponent.m_collisionCheck;
                        SetComponent.SetCollisionComponent(EntityID, collisionComponent);

                        AnimationComponent ac = new AnimationComponent();

                        ac.m_frameNumber = 0;
                        ac.m_frameTimer = 0;
                        ac.m_isAnimating = false;
                        ac.m_framesPerSecond = 0;
                        ac.m_stripCount = 1;
                        Component.Set<AnimationComponent>(EntityID, ac);
                        int i = 9;
                        InternalCall.m_InternalSetSpriteComponent(EntityID, enemyDeathTexture, ref i, ref startingColor, ref startingAlpha);

                        Vector2 direction;

                        direction.X = (enemyPos.X - playerPos.X);
                        direction.Y = (enemyPos.Y - playerPos.Y);

                        float rotationFloat = (float)(Math.Atan2(direction.X, direction.Y) * (180 / Math.PI));

                        InternalCall.m_InternalSetTransformComponent(EntityID, ref enemyPos, ref startingEnemyScale, ref rotationFloat);

                        //Convert into radians
                        float rotationInRadians = (float)((rotationFloat) * Math.PI / 180.0);

                        //Get forward vector X
                        float forwardX = (float)(Math.Sin(rotationInRadians));

                        //Get forward vector Y
                        float forwardY = (float)(Math.Cos(rotationInRadians));

                        movement.X = 0 + forwardX * 0.4f;
                        movement.Y = 0 + forwardY * 0.4f;

                        InternalCall.m_InternalSetVelocity(EntityID, ref movement);

                        isDead = true;

                        Vector2 pos = new Vector2();
                        Vector2 scale = new Vector2();
                        float rotation = 0.0f;

                        InternalCall.m_InternalGetTransformComponent(EntityID, ref pos, ref scale, ref rotation);

                        InternalCall.m_InternalCallAddPrefab("prefab_enemyBloodPool", ref pos.X, ref pos.Y, ref rotation);

                        break;

                    case "Player":

                        playerIsDead = true;
                        bool d = !playerIsDead;
                        int ix = 10;
                        InternalCall.m_InternalSetAnimationComponent(EntityID, ref frameNumber, ref ix, ref frameTimer, ref d, ref stripCount);

                        movement.X = 0;
                        movement.Y = 0;

                        InternalCall.m_InternalSetVelocity(EntityID, ref movement);

                        break;

                    default:
                        break;
                }
            }
        }
        #endregion

        #region Cheat Code

        if (forceDeath)
        {
            totalEnemiesKilled++;

            InternalCall.m_InternalCallPlayAudio(EntityID, "aud_humanDeath01");
            var collisionComponent = GetComponent.GetColliderComponent(EntityID);
            collisionComponent.m_collisionCheck = !collisionComponent.m_collisionCheck;
            SetComponent.SetCollisionComponent(EntityID, collisionComponent);

            AnimationComponent ac = new AnimationComponent();

            ac.m_frameNumber = 0;
            ac.m_frameTimer = 0;
            ac.m_isAnimating = false;
            ac.m_framesPerSecond = 0;
            ac.m_stripCount = 1;
            Component.Set<AnimationComponent>(EntityID, ac);
            int i = 10;
            InternalCall.m_InternalSetSpriteComponent(EntityID, enemyDeathTexture, ref i, ref startingColor, ref startingAlpha);

            Vector2 direction;

            direction.X = (enemyPos.X - playerPos.X);
            direction.Y = (enemyPos.Y - playerPos.Y);

            float rotationFloat = (float)(Math.Atan2(direction.X, direction.Y) * (180 / Math.PI));

            InternalCall.m_InternalSetTransformComponent(EntityID, ref enemyPos, ref startingEnemyScale, ref rotationFloat);

            //Convert into radians
            float rotationInRadians = (float)((rotationFloat) * Math.PI / 180.0);

            //Get forward vector X
            float forwardX = (float)(Math.Sin(rotationInRadians));

            //Get forward vector Y
            float forwardY = (float)(Math.Cos(rotationInRadians));

            movement.X = 0 + forwardX * 0.4f;
            movement.Y = 0 + forwardY * 0.4f;

            InternalCall.m_InternalSetVelocity(EntityID, ref movement);

            isDead = true;

            Vector2 pos = new Vector2();
            Vector2 scale = new Vector2();
            float rotation = 0.0f;

            InternalCall.m_InternalGetTransformComponent(EntityID, ref pos, ref scale, ref rotation);

            InternalCall.m_InternalCallAddPrefab("prefab_enemyBloodPool", ref pos.X, ref pos.Y, ref rotation);
        }

        #endregion

    }
}