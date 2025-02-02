using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;


public class EnemyScript : ScriptBase //Enemy Script, not state machine
{
    #region ID Variables
    private uint EntityID; //Entity ID of the object, do not touch!

    private uint playerID; //Store player ID
    #endregion

    #region Enemy State Machine Variables
    public enum EnemySelection //Enemy type enum, add here for more enemy types
    {
        Helpless,
        Melee,
        Ranged
    };

    public enum EnemyRoamType //Enemy roaming type enum, add here for more roaming behaviours
    {
        Patrolling,
        Static
    };

    public EnemySelection enemyType; //Current Enemy Type

    public EnemyRoamType enemyRoamBehaviour; //Current Enemy Roaming Behaviour

    public EnemyState currentState; //Current Enemy State

    #endregion

    #region Component Variables
    private SpriteComponent spriteComp;
    private AnimationComponent animComp;
    private EnemyComponent enemyComp;
    private PathfindingComponent pathFindComp;
    private ColliderComponent collComp;
    private TransformComponent transformComp;
    private TransformComponent playerTransformComp;
    //private GridComponent gridComp;
    #endregion

    #region Sprite Variables
    private string enemyScientistDeathTexture; //Enemy Scientist Death Texture
    private string enemyRobotDeathTexture; //Enemy Robot Death Texture
    #endregion

    //TO BE COMMENTED
    public bool isDead;
    private bool isChasing;
    private bool isPatrolling;
    private Vector2 movement;
    private float enemyDeathKnockbackMultiplier;
    private float enemyBloodPoolSpawnDelay = 0.5f;
    private float enemySpeed = 1.0f;

    #region Waypoint Variables
    private int initialPatrolWaypoint = 0;
    private int currentPatrolWaypoint = 0;
    private int[] childrenIDList;
    #endregion

    #region Pathfinding Variables
    private bool pFReachedEndOfPath;
    #endregion

    public override void Awake(uint id) //Called everytime instance is created
    {
        EntityID = id; //Sets ID for object, DO NOT TOUCH

        //UpdateRayCastToPlayerPosition(); //Updates Raycast Position when spawned, for objecting pooling, etc.
    }

    public override void Start() //Called once at the start of the game
    {
        
        UpdateComponentValues();
        playerID = (uint)InternalCall.m_InternalCallGetTagID("Player"); //Get Player ID

        enemyScientistDeathTexture = "img_scientistDeath.png";
        //enemyRobotDeathTexture = "img_robotDeath.png"; //Set to ranged enemy death texture

        enemyDeathKnockbackMultiplier = 0.4f;

        switch (enemyComp.m_enemyTypeInt) //Sets enemy type based on EnemyComponent enemy type int ID
        {
            //Helpless Melee
            case 0:
                enemyType = EnemySelection.Helpless; //Set enemy type to helpless
                break;

            //Normal Melee
            case 1:
                enemyType = EnemySelection.Melee; //Set enemy type to melee
                break;

            //Ranged
            case 2:
                enemyType = EnemySelection.Ranged; //Set enemy type to ranged
                break;

            default:
                break;
        }

        switch (enemyComp.m_enemyRoamBehaviourInt) //Sets enemy roaming behaviour based on EnemyComponent roam behaviour int ID
        {
            //Static, non moving
            case 0:
                enemyRoamBehaviour = EnemyRoamType.Static; //Set enemy type to static turrets
                break;

            //Roamers
            case 1:
                enemyRoamBehaviour = EnemyRoamType.Patrolling; //Set enemy type to roamers, with pathing
                break;

            default:
                break;
        }

        UpdateRayCastToPlayerPosition(); //Updates Raycast Position at the start to lock onto the Player

        currentState = CheckEnemyType(); //Checks enemy type to start off new state

        childrenIDList = InternalCall.m_InternalCallGetChildrenID(EntityID); //Gets waypoints for enemy, will be null/empty if there are no children waypoints
       // World2GridCoordinates(1.5f,2.5f,1);
    }

    public override void Update() //Runs every frame
    {
        if (isDead) return;
        CheckForCollisions(); //Checks for collisions in the event an enemy touches the player
        
        currentState.DoActionUpdate(InternalCall.m_InternalCallGetDeltaTime()); //Update the current state's DoActionUpdate function, such as patrolling, chasing etc, with delta time
    }

    #region Enemy State Manager Functions
    public void SetCurrentState(EnemyState nextState) //Sets state for the current enemy type
    {
        currentState = nextState; //Set current state
    }

    public EnemyState CheckEnemyType() //Check for which enemy type, and creates a new state to begin AI
    {
        switch (enemyType)
        {
            case EnemySelection.Helpless: //Return helpless beginning state
                return new EnemyStatePatrol(this);

            case EnemySelection.Melee: //Return melee beginning state
                return new EnemyStatePatrol(this);

            case EnemySelection.Ranged: //Return ranged beginning state
                break;

            default:
                return null;
        }
        return null;
    }

    #endregion

    #region Raycasting Functions
    public void UpdateRayCastToPlayerPosition() //Do before using Raycast
    {
        Raycast rC = InternalCall.m_GetRay(EntityID, "RaycastToPlayer"); //Gets current ray

        UpdateComponentValues(); //Updates components to get latest values

        rC.m_targetposition = playerTransformComp.m_position; //Set ray.Targetposition to player position through transform component, have to use Component.Get due to every frame position being updated

        InternalCall.m_SetRay(EntityID, "RaycastToPlayer", rC); //Sets ray
    }

    public bool CheckPlayerWithinSight() //Checks if able to Raycast to player, prevents enemies from seeing player through walls
    {
        //Draws raycast from enemy to player
        Raycast rC = InternalCall.m_GetRay(EntityID, "RaycastToPlayer"); //Gets current ray, ensure UpdateRaycastToPlayerPosition was called before this

        if (rC.m_targetReached == true) //Checks if able to reach the "target"/player
        {
            return true; //Returns true if able
        }

        else { return false; } //Returns false if unable
    }
    #endregion

    #region Collisions
    private void CheckForCollisions()
    {
        
        if (InternalCall.m_InternalCallIsCollided(EntityID) != 0.0f)
        {
            
            int[] collidedEntities = InternalCall.m_InternalCallGetCollidedEntities(EntityID);

            
            foreach (int collidedEntitiesID in collidedEntities)
            {
                switch (InternalCall.m_InternalCallGetTag((uint)collidedEntitiesID))
                {
                    case "MeleeKillZoneSpawn":
                    case "PlayerBullet":
                        CoroutineManager.Instance.StartCoroutine(EnemyDeath(), "EnemyDeath"); //Runs coroutine to spawn blood pool
                        break;

                    case "Player":

                        //playerIsDead = true;
                        //InternalCall.m_InternalSetAnimationComponent(EntityID, frameNumber, 10, frameTimer, !playerIsDead, stripCount);

                        movement.X = 0;
                        movement.Y = 0;

                        InternalCall.m_InternalSetVelocity(EntityID, movement);

                        break;

                    default:
                        break;
                }
            }
        }
    }
    #endregion

    #region Death Handlers
    private void SetDeadEnemySprite() //Function to set dead enemy Sprite
    {
        UpdateComponentValues(); //Get latest sprite component

        spriteComp.m_layer = 9; //Sets layer to prevent clipping

        switch (enemyType)
        {
            case EnemySelection.Helpless: //Set dead scientist image
                spriteComp.m_imageFile = enemyScientistDeathTexture;
                break;

            case EnemySelection.Melee: //Set dead scientist image
                spriteComp.m_imageFile = enemyScientistDeathTexture;
                break;

            case EnemySelection.Ranged: //Set dead robot image
                spriteComp.m_imageFile = enemyRobotDeathTexture;
                break;

            default:
                break;
        }

        Component.Set<SpriteComponent>(EntityID, spriteComp); //Sets sprite component
    }

    private IEnumerator EnemyDeath() //Coroutine for enemy death
    {
        isDead = true;
        currentState.EnemyDead();

        InternalCall.m_InternalCallPlayAudio(EntityID, "aud_humanDeath01"); //Plays enemy death sound

        collComp.m_collisionCheck = !collComp.m_collisionCheck; //Disables collision check
        collComp.m_collisionResponse = false;
        //InternalCall.m_ChangeLayer(EntityID, 8);


        SetComponent.SetCollisionComponent(EntityID, collComp); //Sets collider of enemy

        InternalCall.m_InternalSetAnimationComponent(EntityID, 0, 0, 0, false, 1); //Stops animation of enemy
        SetDeadEnemySprite(); //Sets dead sprite

        transformComp = Component.Get<TransformComponent>(EntityID);
        //playerTransformComp = Component.Get<TransformComponent>(playerID);

        Vector2 direction;

        //Gets direction to look towards
        direction.X = (transformComp.m_position.X - playerTransformComp.m_position.X); //Gets Vector.X towards player
        direction.Y = (transformComp.m_position.Y - playerTransformComp.m_position.Y); //Gets Vector.Y towards player

        float rotationFloat = (float)(Math.Atan2(direction.X, direction.Y) * (180 / Math.PI)); //Gets rotation towards player

        transformComp.m_rotation = rotationFloat; //Sets rotation values

        SetComponent.SetTransformComponent(EntityID, transformComp); //Sets transform component

        direction.X = (transformComp.m_position.X - playerTransformComp.m_position.X); //Gets Vector.X away from player
        direction.Y = (transformComp.m_position.Y - playerTransformComp.m_position.Y); //Gets Vector.Y away from player

        rotationFloat = (float)(Math.Atan2(direction.X, direction.Y) * (180 / Math.PI)); //Gets rotation away player

        //Convert into radians
        float rotationInRadians = (float)((rotationFloat) * Math.PI / 180.0);

        //Get forward vector X
        float forwardX = (float)(Math.Sin(rotationInRadians));

        //Get forward vector Y
        float forwardY = (float)(Math.Cos(rotationInRadians));
        //+ forwardX * 0.4f
         movement.X = 0 ; //Pushes enemy back for "knockback effect"
         movement.Y = 0 ; ; //Pushes enemy back for "knockback effect"

         InternalCall.m_InternalSetVelocity(EntityID, movement); //Sets velocity for rigidbody to move

        // isDead = true; //Sets enemy to dead status
        transformComp = Component.Get<TransformComponent>(EntityID);

        
        yield return new CoroutineManager.WaitForSeconds(enemyBloodPoolSpawnDelay); //Waits for time before moving to next line;

        InternalCall.m_InternalCallAddPrefab("prefab_enemyBloodPool", transformComp.m_position.X, transformComp.m_position.Y, transformComp.m_rotation); //Spawns blood pool
    }
    #endregion

    #region Patrolling Behaviour
    public void StartPatrol()
    {
        if (childrenIDList.Length > 0) //Checks if there are waypoints
        {
            if (isPatrolling)
            {
                SetToNextWaypoint();

                pathFindComp.m_targetPosition = GetComponent.GetTransformComponent((uint)childrenIDList[currentPatrolWaypoint]).m_position; //Change to grid position

                isPatrolling = true;
            }
        }

        else
        {
            Console.WriteLine("Enemy Has No Waypoints!");
            return;
        }
    }

    public void SetToNextWaypoint()
    {
        currentPatrolWaypoint += 1; //Go to next waypoint

        if (currentPatrolWaypoint >= childrenIDList.Length) currentPatrolWaypoint = 0; //Cycle back to first waypoint
    }



    #endregion

    #region Component Handlers  
    private void UpdateComponentValues()
    {
            spriteComp = Component.Get<SpriteComponent>(EntityID);
            animComp = Component.Get<AnimationComponent>(EntityID);
            enemyComp = Component.Get<EnemyComponent>(EntityID);
            collComp = Component.Get<ColliderComponent>(EntityID);
            transformComp = Component.Get<TransformComponent>(EntityID);
            playerTransformComp = Component.Get<TransformComponent>(playerID);
            pathFindComp = Component.Get<PathfindingComponent>(EntityID);
           // gridComp = Component.Get<GridComponent>(EntityID);
    }
    #endregion

    #region Pathfinding

    public bool ReachedEndOfPathChecker()
    {
        return false;
    }

    public bool Vector2DistanceChecker(Vector2 v1, Vector2 v2, float tolerance)
    {
        float distance = (float) Math.Sqrt(Math.Pow(v2.X - v1.X, 2) + Math.Pow(v2.Y - v1.Y, 2));
        return distance <= tolerance;

    }

    public Vector2 Grid2WorldCoordinates(int gridx, int gridy, int gridkey)
    {
        int gridid = InternalCall.m_InternalGetEntityIdFromGridKey(gridkey);
        GridComponent gridComponent = Component.Get<GridComponent>((uint)gridid);
        float worldX = gridComponent.m_Anchor.X + (gridx * gridComponent.m_GridColumnLength) + 0.5f;
        float worldY = gridComponent.m_Anchor.Y - (gridy * gridComponent.m_GridRowLength) - 0.5f;

        return new Vector2(worldX, worldY); //This is the World Coordinates from the Grid Coordinates
    }

    public Vector2 World2GridCoordinates(float worldX, float worldY, int gridkey)
    {
        int gridid = InternalCall.m_InternalGetEntityIdFromGridKey(gridkey);
        GridComponent gridComponent = Component.Get<GridComponent>((uint)gridid);
        int gridX = (int)Math.Floor(worldX) - (int)gridComponent.m_Anchor.X;
        int gridY = (int)gridComponent.m_Anchor.Y - (int)Math.Floor(worldY) - 1;
        Console.WriteLine(gridX);
        Console.WriteLine(gridY);

        return new Vector2(gridX, gridY); //This is the Grid Coordinates from the World Coordinates
    }


    #endregion

    #region Enemy Behaviour

    public void RunFromPlayer()
    {
        //UpdateComponentValues();
        transformComp = Component.Get<TransformComponent>(EntityID);
        //playerTransformComp = Component.Get<TransformComponent>(playerID);

        Vector2 direction;

        //Gets direction to look towards
        direction.X = (playerTransformComp.m_position.X - transformComp.m_position.X); //Gets Vector.X towards player
        direction.Y = (playerTransformComp.m_position.Y - transformComp.m_position.Y); //Gets Vector.Y towards player

        float rotationFloat = (float)(Math.Atan2(direction.X, direction.Y) * (180 / Math.PI)); //Gets rotation towards player

        transformComp.m_rotation = rotationFloat; //Sets rotation values

        SetComponent.SetTransformComponent(EntityID, transformComp); //Sets transform component

        direction.X = (transformComp.m_position.X - playerTransformComp.m_position.X); //Gets Vector.X away from player
        direction.Y = (transformComp.m_position.Y - playerTransformComp.m_position.Y); //Gets Vector.Y away from player

        rotationFloat = (float)(Math.Atan2(direction.X, direction.Y) * (180 / Math.PI)); //Gets rotation away player

        //Convert into radians
        float rotationInRadians = (float)((rotationFloat) * Math.PI / 180.0);

        //Get forward vector X
        float forwardX = (float)(Math.Sin(rotationInRadians));

        //Get forward vector Y
        float forwardY = (float)(Math.Cos(rotationInRadians));

        if (!InternalCall.m_InternalGetVelocity(EntityID, out movement))
        {
            // return cause velocity -> rigidbody is not present in entity
            return;
        }

        movement.X = 0 + forwardX * enemySpeed;
        movement.Y = 0 + forwardY * enemySpeed;

        InternalCall.m_InternalSetVelocity(EntityID, movement);
    }

    public void RunAtPlayer()
    {
        UpdateComponentValues();

        Vector2 direction;

        //Gets direction to look towards
        direction.X = (playerTransformComp.m_position.X - transformComp.m_position.X); //Gets Vector.X towards player
        direction.Y = (playerTransformComp.m_position.Y - transformComp.m_position.Y); //Gets Vector.Y towards player

        float rotationFloat = (float)(Math.Atan2(direction.X, direction.Y) * (180 / Math.PI)); //Gets rotation towards player

        transformComp.m_rotation = rotationFloat; //Sets rotation values

        SetComponent.SetTransformComponent(EntityID, transformComp); //Sets transform component

        //Convert into radians
        float rotationInRadians = (float)((rotationFloat) * Math.PI / 180.0);

        //Get forward vector X
        float forwardX = (float)(Math.Sin(rotationInRadians));

        //Get forward vector Y
        float forwardY = (float)(Math.Cos(rotationInRadians));

        if (!InternalCall.m_InternalGetVelocity(EntityID, out movement))
        {
            // return cause velocity -> rigidbody is not present in entity
            return;
        }

        movement.X = 0 + forwardX * enemySpeed;
        movement.Y = 0 + forwardY * enemySpeed;

        InternalCall.m_InternalSetVelocity(EntityID, movement);
    }

    #endregion
}