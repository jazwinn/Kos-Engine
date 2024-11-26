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
    private bool iscolliding;

    //private int test;

    public override void Start()
    {
        InternalCall.m_InternalGetTransformComponent(EntityID, out startingRoombaPos, out startingRoombaScale, out startingRoombaRotate);

        speed = 5;
    }

    public override void Update()
    {
        #region Shooting

        int[] collidedEntities = InternalCall.m_InternalCallGetCollidedEntities(EntityID);

        if (InternalCall.m_InternalCallIsCollided(EntityID) != 0.0f)
        {
            foreach (int collidedEntitiesID in collidedEntities)
            {
                if (InternalCall.m_InternalCallGetTag((uint)collidedEntitiesID) == "Wall")
                {
                    //InternalCall.m_InternalCallDeleteEntity(EntityID);
                    movement.X = 0;
                    movement.Y = 0;
                    InternalCall.m_InternalSetVelocity(EntityID, movement);
                    iscolliding = true;

                }
            }
        }





        #endregion

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
            movement.Y = speed;
        }
        if (InternalCall.m_InternalCallIsKeyPressed(keyCode.S))
        {
            movement.Y = -speed;
        }
        if (InternalCall.m_InternalCallIsKeyPressed(keyCode.A))
        {
            movement.X = -speed;
        }
        if (InternalCall.m_InternalCallIsKeyPressed(keyCode.D))
        {
            movement.X = speed;
        }


        InternalCall.m_InternalSetVelocity(EntityID, movement);

        #endregion

        #region Mouse Rotation
        Vector2 mousePos;
        Vector2 roombaPos;

        //Get pos of mouse
        InternalCall.m_InternalGetWorldMousePosition(out mousePos);

        //Get pos of player
        InternalCall.m_InternalGetTranslate(EntityID, out roombaPos);

        Vector2 direction;

        //Gets direction to look towards
        direction.X = (mousePos.X - roombaPos.X);
        direction.Y = (mousePos.Y - roombaPos.Y);

        float rotationFloat = (float)(Math.Atan2(direction.X, direction.Y) * (180 / Math.PI));

        InternalCall.m_InternalSetTransformComponent(EntityID, roombaPos, startingRoombaScale, rotationFloat);

        #endregion
        if (InternalCall.m_InternalCallIsKeyTriggered(keyCode.LMB))
        {
            //InternalCall.m_InternalCallAddPrefab(string prefab, in float x, in float y, in float degree);
            InternalCall.m_InternalCallAddPrefab("PlayerBullet", roombaPos.X, roombaPos.Y, rotationFloat);
        }

    }
}