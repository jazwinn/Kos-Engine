using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Policy;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

public class PlayerGun : ScriptBase
{
    #region Entity ID
    private uint EntityID;

    public override void GetEntityID(uint id)
    {
        EntityID = id;
    }
    #endregion

    public string limbTag;
    private Vector2 limbPos;

    private Vector2 roombaPos;
    private Vector2 roombaScale;
    private float roombaRotate;

    private int startFrameNumber, startFramesPerSecond, currentFrameNumber;
    private float startFrameTimer, currentFrameTimer;
    private bool isAnimating;
    private int stripCount;

    private int weaponEquipped;

    private string startImageFile;
    private int startLayer;
    private Vector3 startColor;
    private float startAlpha;

    public static int ammoCount;

    private bool meleeOnCooldown;

    private string gunshotSound;
    private string cleaverSound;

    private string leftCleaverTexture;
    private string rightCleaverTexture;
    private string leftGunTexture;
    private string rightGunTexture;
    private string bulletPrefab;


    public override void Start()
    {
        gunshotSound = "aud_gunshot01";
        cleaverSound = "aud_cleaver";
        leftCleaverTexture = "ani_cleaverLeftAnim_strip6.png";
        rightCleaverTexture = "ani_cleaverRightAnim_strip6.png";
        leftGunTexture = "ani_gunLeftAnim_strip8.png";
        rightGunTexture = "ani_gunRightAnim_strip8.png";
        bulletPrefab = "prefab_playerBullet";

        ammoCount = 10;

        limbTag = InternalCall.m_InternalCallGetTag(EntityID);
        InternalCall.m_InternalGetTranslate(EntityID, out limbPos);

        InternalCall.m_InternalGetTransformComponent((uint)InternalCall.m_InternalCallGetTagID("Player"), out roombaPos, out roombaScale, out roombaRotate);
        InternalCall.m_InternalGetAnimationComponent(EntityID, out startFrameNumber, out startFramesPerSecond, out startFrameTimer, out isAnimating, out stripCount);
        InternalCall.m_InternalGetSpriteComponent(EntityID, out startImageFile, out startLayer, out startColor, out startAlpha);
    }

    public override void Update()
    {
        if (GameController.gameIsPaused) { return; }

        #region Weapon Swap, 0 is gun 1 is melee

        if (InternalCall.m_InternalCallIsKeyTriggered(keyCode.NUM1) && limbTag == "LeftGunPos")
        {
            //Swap Weapon Left
            if (weaponEquipped == 0)
            {
                stripCount = 6;
                weaponEquipped = 1;
                InternalCall.m_InternalSetSpriteComponent(EntityID, leftCleaverTexture, startLayer, startColor, startAlpha);
                InternalCall.m_InternalSetAnimationComponent(EntityID, startFrameNumber, startFramesPerSecond, startFrameTimer, false, stripCount);

            }

            else if (weaponEquipped == 1)
            {
                stripCount = 8;
                weaponEquipped = 0;
                InternalCall.m_InternalSetSpriteComponent(EntityID, leftGunTexture, startLayer, startColor, startAlpha);
                InternalCall.m_InternalSetAnimationComponent(EntityID, startFrameNumber, startFramesPerSecond, startFrameTimer, false, stripCount);
            }
        }

        if (InternalCall.m_InternalCallIsKeyTriggered(keyCode.NUM2) && limbTag == "RightGunPos")
        {
            //Swap Weapon Right
            if (weaponEquipped == 0)
            {
                stripCount = 6;
                weaponEquipped = 1;
                InternalCall.m_InternalSetSpriteComponent(EntityID, rightCleaverTexture, startLayer, startColor, startAlpha);
                InternalCall.m_InternalSetAnimationComponent(EntityID, startFrameNumber, startFramesPerSecond, startFrameTimer, false, stripCount);

            }

            else if (weaponEquipped == 1)
            {
                stripCount = 8;
                weaponEquipped = 0;
                InternalCall.m_InternalSetSpriteComponent(EntityID, rightGunTexture, startLayer, startColor, startAlpha);
                InternalCall.m_InternalSetAnimationComponent(EntityID, startFrameNumber, startFramesPerSecond, startFrameTimer, false, stripCount);
            }
        }

        #endregion

        #region Shooting

        if (InternalCall.m_InternalCallIsKeyTriggered(keyCode.LMB) && limbTag == "LeftGunPos" && weaponEquipped == 0)
        {
            if (ammoCount != 0)
            {
                ammoCount--;

                InternalCall.m_InternalCallPlayAudio(EntityID, gunshotSound);

                //Get roomba rotation
                InternalCall.m_InternalGetTransformComponent((uint)InternalCall.m_InternalCallGetTagID("Player"), out roombaPos, out roombaScale, out roombaRotate);

                Vector2 spawnPos;

                //Get spawnPos
                InternalCall.m_InternalGetTranslate((uint)InternalCall.m_InternalCallGetTagID("MeleeKillZoneSpawn"), out spawnPos);

                //Set isAnimating true
                isAnimating = true;

                //Play animation by feeding isAnimating into component
                InternalCall.m_InternalSetAnimationComponent(EntityID, startFrameNumber, startFramesPerSecond, startFrameTimer, isAnimating, stripCount);

                //Spawn bullet at limb
                InternalCall.m_InternalCallAddPrefab(bulletPrefab, spawnPos.X, spawnPos.Y, roombaRotate);
            }

            
        }

        else if (InternalCall.m_InternalCallIsKeyTriggered(keyCode.RMB) && limbTag == "RightGunPos" && weaponEquipped == 0)
        {
            if (ammoCount != 0)
            {
                ammoCount--;

                InternalCall.m_InternalCallPlayAudio(EntityID, gunshotSound);

                //Get roomba rotation
                InternalCall.m_InternalGetTransformComponent((uint)InternalCall.m_InternalCallGetTagID("Player"), out roombaPos, out roombaScale, out roombaRotate);

                Vector2 spawnPos;

                //Get spawnPos
                InternalCall.m_InternalGetTranslate((uint)InternalCall.m_InternalCallGetTagID("MeleeKillZoneSpawn"), out spawnPos);

                //Set isAnimating true
                isAnimating = true;

                //Play animation by feeding isAnimating into component
                InternalCall.m_InternalSetAnimationComponent(EntityID, startFrameNumber, startFramesPerSecond, startFrameTimer, isAnimating, stripCount);

                //Spawn bullet at limb
                InternalCall.m_InternalCallAddPrefab(bulletPrefab, spawnPos.X, spawnPos.Y, roombaRotate);
            }

            
        }

        else if (InternalCall.m_InternalCallIsKeyTriggered(keyCode.MMB) && limbTag == "BackGunPos")
        {
            if (ammoCount != 0)
            {
                ammoCount--;

                InternalCall.m_InternalCallPlayAudio(EntityID, gunshotSound);

                //Get roomba rotation
                InternalCall.m_InternalGetTransformComponent((uint)InternalCall.m_InternalCallGetTagID("Player"), out roombaPos, out roombaScale, out roombaRotate);

                //Get limbPos
                InternalCall.m_InternalGetTranslate(EntityID, out limbPos);

                //Set isAnimating true
                isAnimating = true;

                //Play animation by feeding isAnimating into component
                InternalCall.m_InternalSetAnimationComponent(EntityID, startFrameNumber, startFramesPerSecond, startFrameTimer, isAnimating, stripCount);

                //Spawn bullet at limb
                InternalCall.m_InternalCallAddPrefab(bulletPrefab, limbPos.X, limbPos.Y, roombaRotate);
            }
          
        }

        #endregion

        #region Meleeing
        if (InternalCall.m_InternalCallIsKeyTriggered(keyCode.LMB) && limbTag == "LeftGunPos" && weaponEquipped == 1)
        {
            if (!meleeOnCooldown)
            {
                InternalCall.m_InternalCallPlayAudio(EntityID, cleaverSound);

                //Get collider component of MeleeKillZoneSpawn & set to true for checking collision
                var collisionComponent = GetComponent.GetColliderComponent((uint)InternalCall.m_InternalCallGetTagID("MeleeKillZoneSpawn"));
                collisionComponent.m_collisionCheck = true;
                collisionComponent.m_drawDebug = true;
                SetComponent.SetCollisionComponent((uint)InternalCall.m_InternalCallGetTagID("MeleeKillZoneSpawn"), collisionComponent);

                //Set isAnimating true
                isAnimating = true;

                //Animate
                InternalCall.m_InternalSetAnimationComponent(EntityID, startFrameNumber, startFramesPerSecond, startFrameTimer, isAnimating, stripCount);

                //Start Coroutine to set to false for checking collision
                _ = ToggleCollisionAfterDelay(collisionComponent, (uint)InternalCall.m_InternalCallGetTagID("MeleeKillZoneSpawn"), 0.1f);

                meleeOnCooldown = true;

                _ = ToggleMeleeCoolDownAfterDelay(1);
            }

        }

        else if (InternalCall.m_InternalCallIsKeyTriggered(keyCode.RMB) && limbTag == "RightGunPos" && weaponEquipped == 1)
        {
            if (!meleeOnCooldown)
            {
                InternalCall.m_InternalCallPlayAudio(EntityID, cleaverSound);

                //Get collider component of MeleeKillZoneSpawn & set to true for checking collision
                var collisionComponent = GetComponent.GetColliderComponent((uint)InternalCall.m_InternalCallGetTagID("MeleeKillZoneSpawn"));
                collisionComponent.m_collisionCheck = true;
                collisionComponent.m_drawDebug = true;
                SetComponent.SetCollisionComponent((uint)InternalCall.m_InternalCallGetTagID("MeleeKillZoneSpawn"), collisionComponent);

                //Set isAnimating true
                isAnimating = true;

                //Animate
                InternalCall.m_InternalSetAnimationComponent(EntityID, startFrameNumber, startFramesPerSecond, startFrameTimer, isAnimating, stripCount);

                //Start Coroutine to set to false for checking collision
                _ = ToggleCollisionAfterDelay(collisionComponent, (uint)InternalCall.m_InternalCallGetTagID("MeleeKillZoneSpawn"), 0.1f);

                meleeOnCooldown = true;

                _ = ToggleMeleeCoolDownAfterDelay(1);
            }
            
        }
        #endregion

        #region Animation

        if (isAnimating)
        {
            InternalCall.m_InternalGetAnimationComponent(EntityID, out currentFrameNumber, out startFramesPerSecond, out currentFrameTimer, out isAnimating, out stripCount);

            if (currentFrameNumber == stripCount - 1)
            {
                isAnimating = false;
                InternalCall.m_InternalSetAnimationComponent(EntityID, startFrameNumber, startFramesPerSecond, startFrameTimer, isAnimating, stripCount);
            }
        }

        #endregion

        
    }

    #region Coroutines

    private async Task ToggleCollisionAfterDelay(ColliderComponent collisionComponent, uint entityID, float delaySeconds)
    {
        await Task.Delay(TimeSpan.FromSeconds(delaySeconds));

        collisionComponent.m_drawDebug = false;
        collisionComponent.m_collisionCheck = false;
        SetComponent.SetCollisionComponent(entityID, collisionComponent);
    }

    private async Task ToggleMeleeCoolDownAfterDelay(float delaySeconds)
    {
        await Task.Delay(TimeSpan.FromSeconds(delaySeconds));
        meleeOnCooldown = false;
    }

    #endregion
}