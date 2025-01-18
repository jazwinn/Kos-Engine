using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Policy;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

public class PlayerController : ScriptBase
{
    #region Entity ID
    private uint EntityID;

    public override void Awake(uint id)
    {
        EntityID = id;

        //Dead Player Texture Allocation
        playerDeathTexture = "img_roombertDeath.png";

        //Get starting component values
        InternalCall.m_InternalGetTransformComponent(EntityID, out startingRoombaPos, out startingRoombaScale, out startingRoombaRotate);
        InternalCall.m_InternalGetSpriteComponent(EntityID, out startingSprite, out startingLayer, out startingColor, out startingAlpha);

        //Ensures player isnt dead at the start of the game
        isDead = false;

        //Set speed of player
        speed = 5;

        //Set tolerance to prevent jittering, higher values = more rigid rotation, but no more jittering due to micro changes
        angleTolerance = 3.5f;
    }
    #endregion

    //Player Speed
    public float speed;

    //Starting Values Holder
    private Vector2 startingRoombaPos;
    private Vector2 startingRoombaScale;
    private float startingRoombaRotate;

    //For rotation, angleTolerance to ensure no jittering
    private float previousRotationFloat;
    private float rotationFloat;
    private float angleTolerance;

    //Movement Vector
    private Vector2 movement;

    //Collision
    private int[] collidedEntities;

    //Sprite Variables
    private string startingSprite;
    private int startingLayer;
    private Vector3 startingColor;
    private float startingAlpha;

    //Stop playing from doing anything if dead
    private bool isDead;

    //Dead player texture, swap once dead
    private string playerDeathTexture;

    public override void Start()
    {

    }

    public override void Update()
    {
        //Checks if game is paused and prevents player from doing anything
        if (GameController.gameIsPaused) { return; }

        //Dead player, return to prevent actions
        if (isDead) { return; }

        #region Movement WASD

        if (!InternalCall.m_InternalGetVelocity(EntityID, out movement))
        {
            // return cause velocity -> rigidbody is not present in entity
            return;
        }

        movement.X = 0;
        movement.Y = 0;

        if (InternalCall.m_InternalCallIsKeyPressed(keyCode.W))
        {
            movement.Y = speed; // Move up if not blocked
        }

        if (InternalCall.m_InternalCallIsKeyPressed(keyCode.S))
        {
            movement.Y = -speed; // Move down if not blocked
        }

        if (InternalCall.m_InternalCallIsKeyPressed(keyCode.A))
        {
            movement.X = -speed; // Move left if not blocked
        }

        if (InternalCall.m_InternalCallIsKeyPressed(keyCode.D))
        {
            movement.X = speed; // Move right if not blocked
        }

        //Normalize to prevent diagonal movement from adding speed
        movement = NormalizeAndScale(movement.X, movement.Y, speed);

        InternalCall.m_InternalSetVelocity(EntityID, movement);

        #endregion

        #region Mouse Rotation

        Vector2 mousePos;
        Vector2 roombaPos;

        //Get pos of mouse
        InternalCall.m_InternalGetWorldMousePosition(out mousePos);

        //Get pos of player
        InternalCall.m_InternalGetTransformComponent(EntityID, out roombaPos, out startingRoombaScale, out startingRoombaRotate);

        Vector2 direction;

        //Gets direction to look towards
        direction.X = (mousePos.X - roombaPos.X);
        direction.Y = (mousePos.Y - roombaPos.Y);

        rotationFloat = (float)(Math.Atan2(direction.X, direction.Y) * (180 / Math.PI));

        if ((rotationFloat > previousRotationFloat + angleTolerance) || (rotationFloat < previousRotationFloat - angleTolerance))
        {
            InternalCall.m_InternalSetTransformComponent(EntityID, roombaPos, startingRoombaScale, rotationFloat);
            previousRotationFloat = rotationFloat;
        }

        #endregion

        #region Collision
        if (InternalCall.m_InternalCallIsCollided(EntityID) != 0.0f)
        {
            collidedEntities = InternalCall.m_InternalCallGetCollidedEntities(EntityID);

            foreach (int collidedEntitiesID in collidedEntities)
            {
                if (InternalCall.m_InternalCallGetTag((uint) collidedEntitiesID) == "Enemy")
                {
                    InternalCall.m_InternalCallPlayAudio(EntityID, "aud_playerDeath01");

                    var collisionComponent = GetComponent.GetColliderComponent(EntityID);
                    collisionComponent.m_collisionCheck = !collisionComponent.m_collisionCheck;
                    SetComponent.SetCollisionComponent(EntityID, collisionComponent);

                    InternalCall.m_InternalSetAnimationComponent(EntityID, 0, 0, 0, false, 1);
                    InternalCall.m_InternalSetSpriteComponent(EntityID, playerDeathTexture, startingLayer, startingColor, startingAlpha);

                    movement.X = 0;
                    movement.Y = 0;

                    InternalCall.m_InternalSetVelocity(EntityID, movement);

                    isDead = true;
                }
            }
        }

        #endregion
    }

    #region Normalize
    public Vector2 NormalizeAndScale(float x, float y, float speed)
    {
        // Calculate the magnitude of the vector
        double magnitude = Math.Sqrt(x * x + y * y);

        // Handle the zero vector case
        if (magnitude == 0)
        {
            Vector2 empty;

            empty.X = 0;
            empty.Y = 0;

            return empty;
        }

        // Normalize the vector
        double normalizedX = x / magnitude;
        double normalizedY = y / magnitude;

        // Scale the normalized vector by the desired speed
        double scaledX = normalizedX * speed;
        double scaledY = normalizedY * speed;

        Vector2 temp;

        temp.X = (float)scaledX;
        temp.Y = (float)scaledY;

        return temp;
    }
    #endregion
}