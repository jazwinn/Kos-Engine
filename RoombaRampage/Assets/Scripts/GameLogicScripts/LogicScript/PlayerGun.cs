using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Policy;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Xml;

public class PlayerGun : ScriptBase
{
    #region Entity ID
    private uint EntityID;

    public override void Awake(uint id)
    {
        EntityID = id;

        gunshotSound = "aud_gunshot01";
        cleaverSound = "aud_cleaver";
        leftCleaverTexture = "ani_cleaverLeftAnim_strip6.png";
        rightCleaverTexture = "ani_cleaverRightAnim_strip6.png";
        leftGunTexture = "ani_gunLeftAnim_strip8.png";
        rightGunTexture = "ani_gunRightAnim_strip8.png";
        bulletPrefab = "prefab_playerBullet";

        meleeCount = 2;

        ammoCount = 10;

        bulletCounterID = (uint)InternalCall.m_InternalCallGetTagID("UIBulletCounter");

        limbTag = InternalCall.m_InternalCallGetTag(EntityID);
        InternalCall.m_InternalGetTranslate(EntityID, out limbPos);

        InternalCall.m_InternalGetTransformComponent((uint)InternalCall.m_InternalCallGetTagID("Player"), out roombaPos, out roombaScale, out roombaRotate);
        InternalCall.m_InternalGetAnimationComponent(EntityID, out startFrameNumber, out startFramesPerSecond, out startFrameTimer, out isAnimating, out stripCount);
        InternalCall.m_InternalGetSpriteComponent(EntityID, out startImageFile, out startLayer, out startColor, out startAlpha);
        InternalCall.m_InternalGetTextComponent(bulletCounterID, out startText, out startFileName, out startFontLayer, out startFontSize, out startFontColor);
        UpdateBulletCounterUI();
        UpdateMeleeCounterUI();
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

    public static int meleeCount;

    private string gunshotSound;
    private string cleaverSound;

    private string leftCleaverTexture;
    private string rightCleaverTexture;
    private string leftGunTexture;
    private string rightGunTexture;
    private string bulletPrefab;

    //Bullet Counter UI Variables
    private string startText, startFileName;
    private int startFontLayer;
    private float startFontSize;
    private Vector3 startFontColor;
    private uint bulletCounterID;

    public override void Start()
    {
        

    }

    public override void Update()
    {
        if (GameController.gameIsPaused || EnemyController.playerIsDead)
        {
            if (isAnimating == true)
            {
                isAnimating = false;
                InternalCall.m_InternalSetAnimationComponent(EntityID, startFrameNumber, startFramesPerSecond, startFrameTimer, isAnimating, stripCount);
                return;
            }

            return;
        }

        #region Weapon Swap, 0 is gun 1 is melee

        if (InternalCall.m_InternalCallIsKeyTriggered(keyCode.NUM1) && limbTag == "LeftGunPos")
        {
            //Swap Weapon Left
            if (weaponEquipped == 0)
            {
                InternalCall.m_InternalCallPlayAudio(EntityID, "aud_weaponSwitch01");
                stripCount = 6;
                weaponEquipped = 1;
                InternalCall.m_InternalSetSpriteComponent(EntityID, leftCleaverTexture, startLayer, startColor, startAlpha);
                InternalCall.m_InternalSetAnimationComponent(EntityID, startFrameNumber, startFramesPerSecond, startFrameTimer, false, stripCount);

            }

            else if (weaponEquipped == 1)
            {
                InternalCall.m_InternalCallPlayAudio(EntityID, "aud_weaponSwitch01");
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
                InternalCall.m_InternalCallPlayAudio(EntityID, "aud_weaponSwitch01");
                stripCount = 6;
                weaponEquipped = 1;
                InternalCall.m_InternalSetSpriteComponent(EntityID, rightCleaverTexture, startLayer, startColor, startAlpha);
                InternalCall.m_InternalSetAnimationComponent(EntityID, startFrameNumber, startFramesPerSecond, startFrameTimer, false, stripCount);

            }

            else if (weaponEquipped == 1)
            {
                InternalCall.m_InternalCallPlayAudio(EntityID, "aud_weaponSwitch01");
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

                UpdateBulletCounterUI();

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

        if (InternalCall.m_InternalCallIsKeyTriggered(keyCode.RMB) && limbTag == "RightGunPos" && weaponEquipped == 0)
        {
            if (ammoCount != 0)
            {
                ammoCount--;

                UpdateBulletCounterUI();

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

        if (InternalCall.m_InternalCallIsKeyTriggered(keyCode.MMB) && limbTag == "BackGunPos")
        {
            if (ammoCount != 0)
            {
                ammoCount--;

                UpdateBulletCounterUI();

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
            if (meleeCount > 2)
            {
                meleeCount = 2;
            }
            AttemptMelee();
        }

        else if (InternalCall.m_InternalCallIsKeyTriggered(keyCode.RMB) && limbTag == "RightGunPos" && weaponEquipped == 1)
        {
            if (meleeCount > 2)
            {
                meleeCount = 2;
            }
            AttemptMelee();   
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

    private void AttemptMelee()
    {
        if (meleeCount > 0)
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

            meleeCount--;

            UpdateMeleeCounterUI();

            _ = ToggleMeleeCoolDownAfterDelay(1.25f);
        }
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
        meleeCount++;
        UpdateMeleeCounterUI();
    }

    #endregion

    #region UI Updating

    private void UpdateBulletCounterUI()
    {
        startText = ammoCount.ToString();

        if (ammoCount <= 0)
        {
            //Set Red AMMO
            Vector3 tempColor;
            tempColor.R = 255.0f/255.0f;
            tempColor.G = 0;
            tempColor.B = 0;

            string tempText;
            string tempFile;
            int tempLayer;
            float tempSize;
            Vector3 oldColor;

            InternalCall.m_InternalSetTextComponent(bulletCounterID, startText, startFileName, in startFontLayer, in startFontSize, in tempColor);
            InternalCall.m_InternalGetTextComponent((uint)InternalCall.m_InternalCallGetTagID("UIBulletHeader"), out tempText, out tempFile, out tempLayer, out tempSize, out oldColor);
            InternalCall.m_InternalSetTextComponent((uint)InternalCall.m_InternalCallGetTagID("UIBulletHeader"), tempText, tempFile, in tempLayer, in tempSize, in tempColor);
        }

        else
        {
            //Set Green AMMO
            Vector3 temp;
            temp.R = 0;
            temp.G = 255.0f/255.0f;
            temp.B = 0;
            InternalCall.m_InternalSetTextComponent(bulletCounterID, startText, startFileName, in startFontLayer, in startFontSize, in temp);
        }
        
    }

    private void UpdateMeleeCounterUI()
    {
        if (meleeCount <= 0)
        {
            //Set Red AMMO
            Vector3 tempColor;
            tempColor.R = 255.0f/255.0f;
            tempColor.G = 0;
            tempColor.B = 0;

            //SpriteComponent tempSpriteComponent = GetComponent.GetSpriteComponent((uint)InternalCall.m_InternalCallGetTagID("UIMeleeCounter"));
            SpriteComponent tempSpriteComponent = Component.Get<SpriteComponent>((uint)InternalCall.m_InternalCallGetTagID("UIMeleeCounter"));

            tempSpriteComponent.m_color = tempColor;
            SetComponent.SetSpriteComponent((uint)InternalCall.m_InternalCallGetTagID("UIMeleeCounter"), tempSpriteComponent);
        }

        else if (meleeCount == 1)
        {
            //Set ORANGE AMMO
            Vector3 tempColor;
            tempColor.R = 255.0f/255.0f;
            tempColor.G = 135.0f/255.0f;
            tempColor.B = 0;

            SpriteComponent tempSpriteComponent = GetComponent.GetSpriteComponent((uint)InternalCall.m_InternalCallGetTagID("UIMeleeCounter"));
            tempSpriteComponent.m_color = tempColor;
            SetComponent.SetSpriteComponent((uint)InternalCall.m_InternalCallGetTagID("UIMeleeCounter"), tempSpriteComponent);
            
        }

        else if (meleeCount >= 2)
        {
            //Set GREEN AMMO
            Vector3 tempColor;
            tempColor.R = 0;
            tempColor.G = 245.0f / 255.0f;
            tempColor.B = 0;

            SpriteComponent tempSpriteComponent = GetComponent.GetSpriteComponent((uint)InternalCall.m_InternalCallGetTagID("UIMeleeCounter"));
            tempSpriteComponent.m_color = tempColor;
            SetComponent.SetSpriteComponent((uint)InternalCall.m_InternalCallGetTagID("UIMeleeCounter"), tempSpriteComponent);

        }

    }

    #endregion
}