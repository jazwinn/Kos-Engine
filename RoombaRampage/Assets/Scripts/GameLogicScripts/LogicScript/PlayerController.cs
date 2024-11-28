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

    public override void GetEntityID(uint id)
    {
        EntityID = id;
    }
    #endregion

    public float speed;

    private Vector2 startingRoombaPos;
    private Vector2 startingRoombaScale;
    private float startingRoombaRotate;

    private Vector2 movement;
    private int[] collidedEntities;

    //Sprite Variables
    private string startingSprite;
    private int startingLayer;
    private Vector3 startingColor;
    private float startingAlpha;

    private bool isDead;

    private string playerDeathTexture;

    public override void Start()
    {
        InternalCall.m_InternalCallSetTimeScale(1);
        playerDeathTexture = "img_roombertDeath.png";

        InternalCall.m_InternalGetTransformComponent(EntityID, out startingRoombaPos, out startingRoombaScale, out startingRoombaRotate);
        InternalCall.m_InternalGetSpriteComponent(EntityID, out startingSprite, out startingLayer, out startingColor, out startingAlpha);

        isDead = false;

        speed = 5;
    }

    public override void Update()
    {
        if (GameController.gameIsPaused) { return; }


        Console.WriteLine(InternalCall.m_InternalCallIsWindowMinimise());

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

        float rotationFloat = (float)(Math.Atan2(direction.X, direction.Y) * (180 / Math.PI));

        InternalCall.m_InternalSetTransformComponent(EntityID, roombaPos, startingRoombaScale, rotationFloat);

        #endregion

        #region Shooting
        if (InternalCall.m_InternalCallIsCollided(EntityID) != 0.0f)
        {
            collidedEntities = InternalCall.m_InternalCallGetCollidedEntities(EntityID);

            foreach (int collidedEntitiesID in collidedEntities)
            {
                if (InternalCall.m_InternalCallGetTag((uint) collidedEntitiesID) == "Enemy")
                {
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
}