using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text.RegularExpressions;


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
    private Vector2 movement = new Vector2();
    private float enemyDeathKnockbackMultiplier;
    private float enemyBloodPoolSpawnDelay = 0.5f;
    private float enemySpeed = 1.0f;
    private float patrolSpeed = 1.8f;

    #region Waypoint Variables
    private int initialPatrolWaypoint = 0;
    private int currentPatrolWaypoint = 0;
    private int[] childrenIDList;
    private List<Vector2> waypoints;
    private int currentPatrolPath = 0;
    private List<Vector2> Paths;
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
        
        
        playerID = (uint)InternalCall.m_InternalCallGetTagID("Player"); //Get Player ID
        UpdateComponentValues();
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

        if (enemyRoamBehaviour == EnemyRoamType.Patrolling)
        {
            childrenIDList = InternalCall.m_InternalCallGetChildrenID(EntityID); //Gets waypoints for enemy, will be null/empty if there are no children waypoints
            StoreWaypoints();
            if (waypoints.Count != 0)
            {
                Vector2 targetWaypoint = waypoints[currentPatrolWaypoint];

                Vector2 gridTargetPos = World2GridCoordinates(targetWaypoint.X, targetWaypoint.Y, pathFindComp.m_gridkey);

                pathFindComp.m_targetPosition = gridTargetPos;

            }

            Paths = GetPath(
               pathFindComp.m_gridkey,
               (int)World2GridCoordinates(transformComp.m_position.X,transformComp.m_position.Y,pathFindComp.m_gridkey).X, (int)World2GridCoordinates(transformComp.m_position.X, transformComp.m_position.Y, pathFindComp.m_gridkey).Y,
               (int)pathFindComp.m_targetPosition.X, (int)pathFindComp.m_targetPosition.Y
            );
            Console.WriteLine(Paths.Count);
        }
        //List<Vector2> path = GetPath(0, 0, 0, 2, 2);
        //foreach (Vector2 point in path)
        //{
        //    //Console.WriteLine($"Path Point: ({point.X}, {point.Y})");
        //    Vector2 points = Grid2WorldCoordinates((int)point.X, (int)point.Y, 0);
        //    Console.WriteLine($"Path Point: ({points.X}, {points.Y})");
        //}

        //StartPatrol();

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

                        InternalCall.m_InternalSetVelocity(EntityID, in movement);

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

        //RigidBodyComponent rc = Component.Get<RigidBodyComponent>(EntityID);

        //+ forwardX * 0.4f
         movement.X = 0 ; //Pushes enemy back for "knockback effect"
         movement.Y = 0 ; ; //Pushes enemy back for "knockback effect"

         InternalCall.m_InternalSetVelocity(EntityID, in movement); //Sets velocity for rigidbody to move

        // isDead = true; //Sets enemy to dead status
        transformComp = Component.Get<TransformComponent>(EntityID);

        yield return null;
        //yield return new CoroutineManager.WaitForSeconds(enemyBloodPoolSpawnDelay); //Waits for time before moving to next line;
        
        InternalCall.m_InternalCallAddPrefab("prefab_enemyBloodPool", transformComp.m_position.X, transformComp.m_position.Y, transformComp.m_rotation); //Spawns blood pool
    }
    #endregion

    #region Patrolling Behaviour
    public void StartPatrol()
    {
        Console.WriteLine(currentPatrolPath);
        if (Vector2DistanceChecker(transformComp.m_position, Grid2WorldCoordinates((int)Paths[currentPatrolPath].X, (int)Paths[currentPatrolPath].Y, pathFindComp.m_gridkey), 0.3f))
        {
            
            SetToNextPath();
        }
        if (childrenIDList.Length > 0)
        {

            if (!isPatrolling)
            {
                isPatrolling = true;

                CoroutineManager.Instance.StartCoroutine(PatrolRoutine(), "Patrol");
                
            }
        }
        else
        {
            Console.WriteLine("Enemy Has No Waypoints!");
        }
    }

    private IEnumerator PatrolRoutine()
    {
        if (waypoints == null || waypoints.Count == 0)
        {
            Console.WriteLine("Waypoints list is empty!");
            yield break;
        }

        while (true)
        {
            Vector2 targetWaypoint = waypoints[currentPatrolWaypoint];
            Vector2 gridTargetPos = World2GridCoordinates(targetWaypoint.X, targetWaypoint.Y, pathFindComp.m_gridkey);
            pathFindComp.m_targetPosition = gridTargetPos;


            //if (Paths != null)
            //{
            //    Console.WriteLine("here");

            //    Paths = GetPath(
            //        pathFindComp.m_gridkey,
            //        (int)pathFindComp.m_startPosition.X, (int)pathFindComp.m_startPosition.Y,
            //        (int)pathFindComp.m_targetPosition.X, (int)pathFindComp.m_targetPosition.Y
            //    );
            //}
            if (Paths == null || Paths.Count == 0)
            {
                Console.WriteLine("No valid path found!");
                yield return new CoroutineManager.WaitForSeconds(1.0f);
                continue;
            }
            
            //Console.WriteLine(Paths[currentPatrolPath].X.ToString());
            //Console.WriteLine(Paths[currentPatrolPath].Y.ToString());
            //Console.WriteLine(Grid2WorldCoordinates((int)Paths[currentPatrolPath].X, (int)Paths[currentPatrolPath].Y, pathFindComp.m_gridkey).X);
            //Console.WriteLine(Grid2WorldCoordinates((int)Paths[currentPatrolPath].X, (int)Paths[currentPatrolPath].Y, pathFindComp.m_gridkey).Y);

            if (!Vector2DistanceChecker(transformComp.m_position, Grid2WorldCoordinates((int)Paths[currentPatrolPath].X, (int)Paths[currentPatrolPath].Y, pathFindComp.m_gridkey), 0.1f))
            {
                Console.WriteLine($"moving {Paths[currentPatrolPath].X},{Paths[currentPatrolPath].Y}");
                MoveToTarget(Paths[currentPatrolPath], patrolSpeed);
            }
            //else if(Vector2DistanceChecker(transformComp.m_position, Grid2WorldCoordinates((int)Paths[currentPatrolPath].X, (int)Paths[currentPatrolPath].Y, pathFindComp.m_gridkey), 0.1f))
            //{   
            //    Vector2 stop = new Vector2(0,0);
            //    InternalCall.m_InternalSetVelocity(EntityID, stop);
            //    SetToNextPath();
            //}

            //foreach (Vector2 pathPoint in paths)
            //{
            //    if (!Vector2DistanceChecker(transformComp.m_position, pathPoint, 0.1f))
            //    {
            //        MoveToTarget(pathPoint, patrolSpeed);
            //    }


            //    yield return new CoroutineManager.WaitForCondition(() =>
            //        Vector2DistanceChecker(transformComp.m_position, pathPoint, 0.1f)
            //    );
            //}

            //for (int i = 0; i < paths.Count();) { 
            //    Vector2 pathPoint = paths[i];
            //    if (!Vector2DistanceChecker(transformComp.m_position, pathPoint, 0.1f))
            //    {
            //        MoveToTarget(pathPoint, patrolSpeed);

            //    }
            //    else 
            //    { 
            //        i++;
            //        yield return new CoroutineManager.WaitForCondition(() =>
            //        Vector2DistanceChecker(transformComp.m_position, pathPoint, 0.1f));
            //    }
            //}

            // After reaching the last path point, check if we've reached the waypoint
            yield return new CoroutineManager.WaitForCondition(() =>
                Vector2DistanceChecker(transformComp.m_position, targetWaypoint, 0.1f)
            );

            //if (Paths.Count == 0)
            //{
            //    SetToNextWaypoint();
            //}

            // Wait at the waypoint for 3 seconds before moving to the next waypoint
            yield return new CoroutineManager.WaitForSeconds(0.1f);
        }
    }



    public void SetToNextWaypoint()
    {
        currentPatrolWaypoint += 1;
        if (currentPatrolWaypoint >= waypoints.Count) // Ensure it wraps correctly
            currentPatrolWaypoint = 0;
    }

    public void SetToNextPath()
    {
        currentPatrolPath += 1;
        if (currentPatrolPath >= Paths.Count) // Ensure it wraps correctly
        {
            Paths.Clear();
            currentPatrolPath = 0;
            SetToNextWaypoint();
            Vector2 targetWaypoint = waypoints[currentPatrolWaypoint];
            Vector2 gridTargetPos = World2GridCoordinates(targetWaypoint.X, targetWaypoint.Y, pathFindComp.m_gridkey);
            pathFindComp.m_targetPosition = gridTargetPos;
            Paths = GetPath(
               pathFindComp.m_gridkey,
               (int)World2GridCoordinates(transformComp.m_position.X, transformComp.m_position.Y, pathFindComp.m_gridkey).X, (int)World2GridCoordinates(transformComp.m_position.X, transformComp.m_position.Y, pathFindComp.m_gridkey).Y,
               (int)pathFindComp.m_targetPosition.X, (int)pathFindComp.m_targetPosition.Y
            );
        }
    }
    #endregion

    private void StoreWaypoints()
    {
        if (childrenIDList == null)
        {
            Console.WriteLine("No child waypoints found!");
            return;
        }

        waypoints = new List<Vector2>();

        foreach (var waypointID in childrenIDList)
        {
            TransformComponent waypointTransform = GetComponent.GetTransformComponent((uint)waypointID);
            if (waypointTransform == null)
            {
                Console.WriteLine($"Waypoint ID {waypointID} has no TransformComponent!");
                continue;
            }

            Vector2 waypointPos = new Vector2(
                transformComp.m_position.X + waypointTransform.m_position.X* transformComp.m_scale.X,
                transformComp.m_position.Y + waypointTransform.m_position.Y* transformComp.m_scale.Y
            );
            //Console.WriteLine(waypointID);
            //Console.WriteLine(waypointPos.X);
            //Console.WriteLine(waypointPos.Y);
            waypoints.Add(waypointPos);

            //Console.WriteLine(temp.Y);
        }
    }

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
            //gridComp = Component.Get<GridComponent>(EntityID);
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
        float worldX = gridComponent.m_Anchor.X + gridx + 0.5f;
        float worldY = gridComponent.m_Anchor.Y - gridy - 0.5f;
        return new Vector2(worldX, worldY); //This is the World Coordinates from the Grid Coordinates
    }

    public Vector2 World2GridCoordinates(float worldX, float worldY, int gridkey)
    {
        int gridid = InternalCall.m_InternalGetEntityIdFromGridKey(gridkey);
        GridComponent gridComponent = Component.Get<GridComponent>((uint)gridid);
        int gridX = (int)Math.Floor(worldX) - (int)gridComponent.m_Anchor.X;
        int gridY = (int)gridComponent.m_Anchor.Y - (int)Math.Floor(worldY) - 1;

        return new Vector2(gridX, gridY); //This is the Grid Coordinates from the World Coordinates
    }
    public List<Vector2> GetPath(int gridKey, int startX, int startY, int endX, int endY)
    {
        bool success = InternalCall.m_InternalCallGetPath(gridKey, startX, startY, endX, endY, out int[] pathx, out int[] pathy);

        List<Vector2> path = new List<Vector2>();

        if (success && pathx.Length == pathy.Length) // Ensure arrays are valid
        {
            for (int i = 0; i < pathx.Length; i++)
            {
                path.Add(new Vector2(pathx[i], pathy[i]));
            }
        }

        return path;
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



        InternalCall.m_InternalSetVelocity(EntityID, in movement); //BANE OF MY EXISTENCE
    }

    public void MoveToTarget(Vector2 targetPosition, float maxSpeed)
    {
        //Console.WriteLine($"Moving to: {targetPosition.X}, {targetPosition.Y}");

        // Convert target grid position to world coordinates
        Vector2 targetPos = Grid2WorldCoordinates((int)targetPosition.X, (int)targetPosition.Y, pathFindComp.m_gridkey);

        // Calculate direction vector
        Vector2 direction = new Vector2(
            targetPos.X - transformComp.m_position.X,
            targetPos.Y - transformComp.m_position.Y
        );

        // Calculate squared magnitude (to avoid sqrt for comparison)
        float squaredMagnitude = (direction.X * direction.X) + (direction.Y * direction.Y);

        // Define a threshold squared distance
        float thresholdSquared = 0.1f;  // (0.1f * 0.1f)

        // If target is reached (magnitude squared is below the threshold), stop
        if (squaredMagnitude < thresholdSquared)
        {
            transformComp.m_position = targetPos;
            movement.X = 0;
            movement.Y = 0;
        }
        else
        {
            // Normalize direction vector
            float magnitude = (float)Math.Sqrt(squaredMagnitude);  // magnitude is used just for normalization
            direction.X /= magnitude;
            direction.Y /= magnitude;

            transformComp.m_rotation = (float)Math.Atan2(direction.Y, direction.X) * (360 / (float)Math.PI);
            //Console.WriteLine(transformComp.m_rotation);
            SetComponent.SetTransformComponent(EntityID, transformComp);

            float dynamicSpeed = maxSpeed * (1 - Math.Min(squaredMagnitude / 100.0f, 1.0f)); // Easing in deceleration

            // Set the movement velocity based on dynamic speed
            movement.X = direction.X * dynamicSpeed;
            movement.Y = direction.Y * dynamicSpeed;
        }

        // Set velocity only once
        InternalCall.m_InternalSetVelocity(EntityID, in movement);
    }





    public void RunAtPlayer()
    {
        // Call MoveToTarget method with player's position
        MoveToTarget(playerTransformComp.m_position, enemySpeed);
    }

    public void PatrolToWaypoint()
    {
        // Call MoveToTarget method with the next waypoint's position
        Vector2 waypointPosition = new Vector2(
            transformComp.m_position.X + GetComponent.GetTransformComponent((uint)childrenIDList[currentPatrolWaypoint]).m_position.X,
            transformComp.m_position.Y + GetComponent.GetTransformComponent((uint)childrenIDList[currentPatrolWaypoint]).m_position.Y
        );

        MoveToTarget(waypointPosition, patrolSpeed);
    }


    #endregion
}