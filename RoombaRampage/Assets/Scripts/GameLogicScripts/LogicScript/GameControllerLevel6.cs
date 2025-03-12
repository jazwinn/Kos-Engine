using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text.RegularExpressions;

public class GameControllerLevel6 : ScriptBase
{
    #region ID Variables
    private uint EntityID; //Entity ID of the object, do not touch!
    #endregion

    private string blockDoorPrefab;
    public static bool isActivated = false;
    public static bool isBossDead = false;
    private string bossBGM = "aud_bossLevelLoop.wav";


    public override void Awake(uint id)
    {
        EntityID = id; //Sets ID for object, DO NOT TOUCH
        blockDoorPrefab = "door_block"; 
    }
    public override void Start()
    {
        InternalCall.m_InternalGetTranslate(EntityID, out Vector2 doorPosition);

    }

    public override void Update()
    {

        #region Collision Handling
        if (InternalCall.m_InternalCallIsCollided(EntityID) != 0.0f)
        {
            int[] collidedEntities = InternalCall.m_InternalCallGetCollidedEntities(EntityID);

            foreach (int collidedEntitiesID in collidedEntities)
            {
                switch (InternalCall.m_InternalCallGetTag((uint)collidedEntitiesID))
                {
                    case "Player": // If it hits the player
                        if(isActivated == false)
                        {
                            isActivated = true;
                            SpawnDoor();
                            InternalCall.m_InternalCallPlayAudio(EntityID, bossBGM);

                        }
                        break;

                    default:
                        break;
                }
            }
        }
        #endregion
    }

    public void SpawnDoor()
    {
        InternalCall.m_InternalGetTranslate(EntityID, out Vector2 doorPosition);
        InternalCall.m_InternalCallAddPrefab(blockDoorPrefab, doorPosition.X, doorPosition.Y, 0);
    }

}