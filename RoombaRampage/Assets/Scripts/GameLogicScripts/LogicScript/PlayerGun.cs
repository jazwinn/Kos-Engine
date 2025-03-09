using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Security.Policy;
using System.Text;
using System.Text.RegularExpressions;
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

        gunshotSound = "aud_gunshot01.wav";
        cleaverSound = "aud_cleaver.wav";
        katanaSound = "aud_katana01.wav";
        emptyGunSound = "aud_emptyGun01.wav";
        cockingSound = "aud_tempcock.wav";

        leftCleaverTexture = "ani_cleaverLeftAnim_strip6.png";
        rightCleaverTexture = "ani_cleaverRightAnim_strip6.png";

        leftKatanaTexture = "ani_katanaLeftAnim_strip19.png";
        rightKatanaTexture = "ani_katanaRightAnim_strip19.png";

        leftBoosterTexture = "ani_boostersLeftAnim_strip8.png";
        rightBoosterTexture = "ani_boostersRightAnim_strip8.png";
        backBoosterTexture = "ani_boostersTailAnim_strip8.png";

        leftGunTexture = "ani_gunLeftAnim_strip8.png";
        rightGunTexture = "ani_gunRightAnim_strip8.png";
        backGunTexture = "ani_gunTailAnim_strip8.png";

        leftShotGunTexture = "ani_gunLeftAnim_strip8.png";
        rightShotGunTexture = "ani_gunRightAnim_strip8.png";

        bulletPrefab = "prefab_playerBullet";

        leftLimbGunAmmo = 6;
        rightLimbGunAmmo = 6;
        backLimbGunAmmo = 6;

        leftLimbShotGunAmmo = 6;
        rightLimbShotGunAmmo = 6;

        leftWeaponCount = 1;
        rightWeaponCount = 1;

        limbTag = InternalCall.m_InternalCallGetTag(EntityID);
        leftLimbPosID = (uint)InternalCall.m_InternalCallGetTagID("LeftLimbPos");
        rightLimbPosID = (uint)InternalCall.m_InternalCallGetTagID("RightLimbPos");
        backLimbPosID = (uint)InternalCall.m_InternalCallGetTagID("BackLimbPos");
        InternalCall.m_InternalGetTranslate(EntityID, out limbPos);

        GetComponentValues();

    }
    #endregion

    public string limbTag;
    private uint leftLimbPosID;
    private uint rightLimbPosID;
    private uint backLimbPosID;
    private Vector2 limbPos;

    private int weaponEquipped;

    private string gunshotSound;
    private string cleaverSound;
    private string katanaSound;
    private string emptyGunSound;
    private string cockingSound;

    private string leftCleaverTexture;
    private string rightCleaverTexture;
    private string leftKatanaTexture;
    private string rightKatanaTexture;
    private string leftBoosterTexture;
    private string rightBoosterTexture;
    private string backBoosterTexture;
    private string leftGunTexture;
    private string rightGunTexture;
    private string backGunTexture;
    private string bulletPrefab;
    private string leftShotGunTexture;
    private string rightShotGunTexture;

    private SpriteComponent limbSpriteComp;
    private AnimationComponent limbAnimComp;
    private TransformComponent playerTransformComp;

    private bool isAnimating;

    public static int leftLimbGunAmmo;
    public static int rightLimbGunAmmo;
    public static int backLimbGunAmmo;

    public static int leftLimbShotGunAmmo;
    public static int rightLimbShotGunAmmo;

    public static int leftWeaponCount;
    public static int rightWeaponCount;

    private uint uiLeftLimbCounterID;
    private uint uiRightLimbCounterID;
    private uint uiBackLimbCounterID;

    private SpriteComponent uiLeftLimbCounterSC;
    private SpriteComponent uiRightLimbCounterSC;
    private SpriteComponent uiBackLimbCounterSC;

    private AnimationComponent uiLeftLimbCounterAC;
    private AnimationComponent uiRightLimbCounterAC;
    private AnimationComponent uiBackLimbCounterAC;

    private bool uiLeftLimbCounterAnimating;
    private bool uiRightLimbCounterAnimating;
    private bool uiBackLimbCounterAnimating;

    public override void Start()
    {
        CheckWeapons();

        uiLeftLimbCounterID = (uint)InternalCall.m_InternalCallGetTagID("UIGameLeftLimbCounter");
        uiRightLimbCounterID = (uint)InternalCall.m_InternalCallGetTagID("UIGameRightLimbCounter");
        uiBackLimbCounterID = (uint)InternalCall.m_InternalCallGetTagID("UIGameBackLimbCounter");

        uiLeftLimbCounterSC = Component.Get<SpriteComponent>(uiLeftLimbCounterID);
        uiRightLimbCounterSC = Component.Get<SpriteComponent>(uiRightLimbCounterID);
        uiBackLimbCounterSC = Component.Get<SpriteComponent>(uiBackLimbCounterID);

        uiLeftLimbCounterAC = Component.Get<AnimationComponent>(uiLeftLimbCounterID);
        uiRightLimbCounterAC = Component.Get<AnimationComponent>(uiRightLimbCounterID);
        uiBackLimbCounterAC = Component.Get<AnimationComponent>(uiBackLimbCounterID);
    }

    public override void Update()
    {
        if (GameControllerLevel1.gameIsPaused || PlayerController.isDead)
        {
            if (PlayerController.isDead)
            {
                StopAnimation();
                return;
            }

            if (isAnimating == true)
            {
                CoroutineManager.Instance.StartCoroutine(PauseAnimation(), "PauseAnimation");
                return;
            }

            return;
        }

        if (PlayerLoadoutManager.isSortieing)
        {
            CheckWeapons();
        }

        switch (weaponEquipped)
        {
            case 0:
                CheckAmmo();
                break;
            case 1:
                CheckMeleeCounter();
                break;
            case 3:
                CheckAmmo();
                break;
            default:
                break;
        }

        if (!GameControllerLevel1.gameIsPaused)
        {
            CheckInputs();
        }

        #region Animation

        if (isAnimating)
        {
            GetComponentValues();
            if (limbAnimComp.m_frameNumber == limbAnimComp.m_stripCount - 1)
            {
                StopAnimation();
            }
        }

        if (uiLeftLimbCounterAnimating)
        {
            uiLeftLimbCounterAC = Component.Get<AnimationComponent>(uiLeftLimbCounterID);

            if (uiLeftLimbCounterAC.m_frameNumber == uiLeftLimbCounterAC.m_stripCount - 1)
            {
                uiLeftLimbCounterAnimating = false;
                uiLeftLimbCounterAC = Component.Get<AnimationComponent>(uiLeftLimbCounterID);
                uiLeftLimbCounterAC.m_frameNumber = 6;
                uiLeftLimbCounterAC.m_isAnimating = false;

                Component.Set<AnimationComponent>(uiLeftLimbCounterID, uiLeftLimbCounterAC);
            }
        }

        if (uiRightLimbCounterAnimating)
        {
            uiRightLimbCounterAC = Component.Get<AnimationComponent>(uiRightLimbCounterID);

            if (uiRightLimbCounterAC.m_frameNumber == uiRightLimbCounterAC.m_stripCount - 1)
            {
                uiRightLimbCounterAnimating = false;
                uiRightLimbCounterAC = Component.Get<AnimationComponent>(uiRightLimbCounterID);
                uiRightLimbCounterAC.m_frameNumber = 6;
                uiRightLimbCounterAC.m_isAnimating = false;

                Component.Set<AnimationComponent>(uiRightLimbCounterID, uiRightLimbCounterAC);
            }
        }

        #endregion

    }
    private void AttemptShootGun()
    {
        switch (limbTag)
        {
            case "LeftLimbSprite":
                if (leftLimbGunAmmo != 0) { leftLimbGunAmmo--; }
                else
                {
                    InternalCall.m_InternalCallPlayAudio(EntityID, emptyGunSound);
                    return;
                }
                break;

            case "RightLimbSprite":
                if (rightLimbGunAmmo != 0) { rightLimbGunAmmo--; }
                else
                {
                    InternalCall.m_InternalCallPlayAudio(EntityID, emptyGunSound);
                    return;
                }
                break;

            case "BackLimbSprite":
                if (backLimbGunAmmo != 0) { backLimbGunAmmo--; }
                else
                {
                    InternalCall.m_InternalCallPlayAudio(EntityID, emptyGunSound);
                    return;
                }
                break;

            default:
                return;
        }

        CoroutineManager.Instance.StartCoroutine(Shoot(), "Shooting");
    }

    private void AttemptShootShotGun()
    {
        switch (limbTag)
        {
            case "LeftLimbSprite":
                if (leftLimbShotGunAmmo != 0 && leftWeaponCount != 0) { 
                    leftLimbShotGunAmmo--;
                    leftWeaponCount--;
                }
                else
                {   
                    if(leftLimbShotGunAmmo <= 0)
                    {
                        InternalCall.m_InternalCallPlayAudio(EntityID, emptyGunSound);
                        
                    }
                    
                    return;
                }
                break;

            case "RightLimbSprite":
                if (rightLimbShotGunAmmo != 0 && rightWeaponCount != 0) { 
                    rightLimbShotGunAmmo--; 
                    rightWeaponCount--;
                }
                else
                {
                    if(rightLimbShotGunAmmo <= 0)
                    {
                        InternalCall.m_InternalCallPlayAudio(EntityID, emptyGunSound);
                    }
                    
                    return;
                }
                break;

            default:
                return;
        }

        
        CoroutineManager.Instance.StartCoroutine(Shoot(), "Shooting");

        


    }
    private void AttemptMelee()
    {
        switch (limbTag)
        {
            case "LeftLimbSprite":
                if (leftWeaponCount != 0)
                {
                    leftWeaponCount--;
                    CoroutineManager.Instance.StartCoroutine(Melee(), "Melee");
                }
                else { return; }
                break;

            case "RightLimbSprite":
                if (rightWeaponCount != 0)
                {
                    rightWeaponCount--;
                    CoroutineManager.Instance.StartCoroutine(Melee(), "Melee");
                }
                else { return; }
                break;

            default:
                return;
        }
    }
    
    #region Coroutines

    private IEnumerator Melee()
    {
        CoroutineManager.Instance.StartCoroutine(StartWeaponCooldown(1f), "MeleeCooldown");

        if (isAnimating)
        {
            StopAnimation();
            yield return new CoroutineManager.WaitForSeconds(0.001f);
            StartAnimation();
        }

        else 
        {
            StartAnimation();
        }

        switch (limbTag)
        {
            case "LeftLimbSprite":
                uiLeftLimbCounterAnimating = true;

                uiLeftLimbCounterAC = Component.Get<AnimationComponent>(uiLeftLimbCounterID);
                uiLeftLimbCounterAC.m_frameNumber = 0;
                uiLeftLimbCounterAC.m_isAnimating = true;

                Component.Set<AnimationComponent>(uiLeftLimbCounterID, uiLeftLimbCounterAC);
                break;

            case "RightLimbSprite":
                uiRightLimbCounterAnimating = true;

                uiRightLimbCounterAC = Component.Get<AnimationComponent>(uiRightLimbCounterID);
                uiRightLimbCounterAC.m_frameNumber = 0;
                uiRightLimbCounterAC.m_isAnimating = true;

                Component.Set<AnimationComponent>(uiRightLimbCounterID, uiRightLimbCounterAC);
                break;

            default:
                break;
        }
        InternalCall.m_InternalCallPlayAudio(EntityID, cleaverSound);
        yield return new CoroutineManager.WaitForSeconds(0.1f);
        InternalCall.m_InternalCallPlayAudio(EntityID, katanaSound);

        uint killZoneID = (uint)InternalCall.m_InternalCallGetTagID("MeleeKillZoneSpawn");
        ColliderComponent killZoneCollComp = Component.Get<ColliderComponent>(killZoneID);
        killZoneCollComp.m_collisionCheck = true;
        killZoneCollComp.m_drawDebug = true;
        Component.Set<ColliderComponent>(killZoneID, killZoneCollComp);

        yield return new CoroutineManager.WaitForSeconds(0.01f);

        killZoneCollComp.m_drawDebug = false;
        killZoneCollComp.m_collisionCheck = false;
        Component.Set<ColliderComponent>(killZoneID, killZoneCollComp);

    }

    private IEnumerator StartWeaponCooldown(float duration)
    {
        yield return new CoroutineManager.WaitForSeconds(duration);

        switch (limbTag)
        {
            case "LeftLimbSprite":
                leftWeaponCount++;
                break;

            case "RightLimbSprite":
                rightWeaponCount++;
                break;

            default:
                break;
        }
    }

    private IEnumerator Shoot()
    {
        //Plays Audio
        InternalCall.m_InternalCallPlayAudio(EntityID, gunshotSound);

        //Update Component Values, mainly for roomba rotation
        GetComponentValues();

        if (isAnimating)
        {
            StopAnimation();
            yield return new CoroutineManager.WaitForSeconds(0.001f);
            StartAnimation();
        }

        else
        {
            StartAnimation();
        }

        switch (limbTag)
        {
            case "LeftLimbSprite":
                //Get limbPos
                InternalCall.m_InternalGetTranslate(leftLimbPosID, out limbPos);
                break;

            case "RightLimbSprite":
                //Get limbPos
                InternalCall.m_InternalGetTranslate(rightLimbPosID, out limbPos);
                break;

            case "BackLimbSprite":
                //Get limbPos
                InternalCall.m_InternalGetTranslate(backLimbPosID, out limbPos);
                break;

            default:
                break;
        }


        if(weaponEquipped == 0)
        {
            //Spawn bullet at limb
            InternalCall.m_InternalCallAddPrefab(bulletPrefab, limbPos.X, limbPos.Y, playerTransformComp.m_rotation);

            //Shake Camera
            CameraFollowPlayerScript.Shake(0.5f, 1f);
        }
        if(weaponEquipped == 3)
        {
            CoroutineManager.Instance.StartCoroutine(StartWeaponCooldown(0.7f), "GunCooldown");

            int numberofpallets = 40;
            float spread = 360.0f; // in degree

            double interval = Math.Floor(spread / (float)numberofpallets);

            float startangle = playerTransformComp.m_rotation - spread / 2.0f;

            for (int n = 0; n < numberofpallets; n++)
            {
                InternalCall.m_InternalCallAddPrefab(bulletPrefab, limbPos.X, limbPos.Y, startangle);
                startangle += (float)interval;

                CameraFollowPlayerScript.Shake(0.7f, 1f);

            }

            yield return new CoroutineManager.WaitForSeconds(0.5f);

            InternalCall.m_InternalCallPlayAudio(EntityID, cockingSound);

        }



    }

    private IEnumerator PauseAnimation()
    {
        isAnimating = false;
        GetComponentValues();
        limbAnimComp.m_isAnimating = false;
        int tempCurrentFrame = limbAnimComp.m_frameNumber;
        Component.Set<AnimationComponent>(EntityID, limbAnimComp);

        yield return new CoroutineManager.WaitForCondition(() =>
        {
            return !GameControllerLevel1.gameIsPaused;
        });

        Console.WriteLine("Condition Met " + !GameControllerLevel1.gameIsPaused);

        isAnimating = true;
        limbAnimComp.m_isAnimating = true;
        limbAnimComp.m_frameNumber = tempCurrentFrame;
        Component.Set<AnimationComponent>(EntityID, limbAnimComp);
    }

    #endregion

    private void CheckAmmo()
    {
        int leftWeaponAmmo = 0;
        int rightWeaponAmmo = 0;
        int backWeaponAmmo = 0;

        switch (weaponEquipped)
        {
            case 0:
                leftWeaponAmmo = leftLimbGunAmmo;
                rightWeaponAmmo = rightLimbGunAmmo;
                backWeaponAmmo = backLimbGunAmmo;
                break;
            case 3:
                leftWeaponAmmo = leftLimbShotGunAmmo;
                rightWeaponAmmo = rightLimbShotGunAmmo;
                break;
            default:
                break;

        }


        switch (limbTag)
        {
            case "LeftLimbSprite":
                //Update UI
                uiLeftLimbCounterAC = Component.Get<AnimationComponent>(uiLeftLimbCounterID);
                uiLeftLimbCounterAC.m_frameNumber = leftWeaponAmmo;
                uiLeftLimbCounterAC.m_isAnimating = false;

                Component.Set<AnimationComponent>(uiLeftLimbCounterID, uiLeftLimbCounterAC);

                break;

            case "RightLimbSprite":
                //Update UI
                uiRightLimbCounterAC = Component.Get<AnimationComponent>(uiRightLimbCounterID);
                uiRightLimbCounterAC.m_frameNumber = rightWeaponAmmo;
                uiRightLimbCounterAC.m_isAnimating = false;

                Component.Set<AnimationComponent>(uiRightLimbCounterID, uiRightLimbCounterAC);
                break;

            case "BackLimbSprite":
                //Update UI
                uiBackLimbCounterAC = Component.Get<AnimationComponent>(uiBackLimbCounterID);
                uiBackLimbCounterAC.m_frameNumber = backWeaponAmmo;
                uiBackLimbCounterAC.m_isAnimating = false;

                Component.Set<AnimationComponent>(uiBackLimbCounterID, uiBackLimbCounterAC);
                break;

            default:
                break;
        }
    }

    private void CheckMeleeCounter()
    {
        switch (limbTag)
        {
            case "LeftLimbSprite":
                //Update UI
                if (leftWeaponCount == 1)
                {
                    uiLeftLimbCounterAC = Component.Get<AnimationComponent>(uiLeftLimbCounterID);
                    uiLeftLimbCounterAC.m_frameNumber = 6;
                    uiLeftLimbCounterAC.m_isAnimating = false;

                    Component.Set<AnimationComponent>(uiLeftLimbCounterID, uiLeftLimbCounterAC);
                }

                break;

            case "RightLimbSprite":
                //Update UI
                if (rightWeaponCount == 1)
                {
                    uiRightLimbCounterAC = Component.Get<AnimationComponent>(uiRightLimbCounterID);
                    uiRightLimbCounterAC.m_frameNumber = 6;
                    uiRightLimbCounterAC.m_isAnimating = false;

                    Component.Set<AnimationComponent>(uiRightLimbCounterID, uiRightLimbCounterAC);
                }

                break;

            default:
                break;
        }
    }

    private string GetWeaponSprite(string weaponName)
    {
        string limbTag = InternalCall.m_InternalCallGetTag(EntityID);

        switch (limbTag)
        {
            case "LeftLimbSprite":
                return this.GetType().GetField("left" + weaponName + "Texture",
                                               System.Reflection.BindingFlags.NonPublic | System.Reflection.BindingFlags.Instance)
                                 ?.GetValue(this) as string;

            case "RightLimbSprite":
                return this.GetType().GetField("right" + weaponName + "Texture",
                                               System.Reflection.BindingFlags.NonPublic | System.Reflection.BindingFlags.Instance)
                                 ?.GetValue(this) as string;

            case "BackLimbSprite":
                return this.GetType().GetField("back" + weaponName + "Texture",
                                               System.Reflection.BindingFlags.NonPublic | System.Reflection.BindingFlags.Instance)
                                 ?.GetValue(this) as string;

            default:
                return null;
        }
    }

    private int GetStripCount(string fileName)
    {
        // Find the last non-digit position before the number
        string numberStr = "";

        // Iterate through the string to find the last sequence of digits
        for (int i = fileName.Length - 1; i >= 0; i--)
        {
            if (char.IsDigit(fileName[i]))
            {
                numberStr = fileName[i] + numberStr; // Prepend to keep the correct order
            }
            else if (numberStr.Length > 0)
            {
                break; // Stop when we encounter a non-digit after collecting digits
            }
        }

        // Convert to integer
        return int.TryParse(numberStr, out int result) ? result : -1;
    }

    private void GetComponentValues()
    {
        limbSpriteComp = GetComponent.GetSpriteComponent(EntityID);
        limbAnimComp = Component.Get<AnimationComponent>(EntityID);
        playerTransformComp = Component.Get<TransformComponent>((uint)InternalCall.m_InternalCallGetTagID("Player"));
    }

    private void SwitchWeapon(string weaponName)
    {
        string fileName = GetWeaponSprite(weaponName);

        limbSpriteComp.m_imageFile = fileName;
        SetComponent.SetSpriteComponent(EntityID, limbSpriteComp);

        limbAnimComp.m_stripCount = GetStripCount(fileName);
        Component.Set<AnimationComponent>(EntityID, limbAnimComp);
    }

    private void StartAnimation()
    {
        isAnimating = true;

        GetComponentValues();
        limbAnimComp.m_frameNumber = 0;
        limbAnimComp.m_isAnimating = isAnimating;

        Component.Set<AnimationComponent>(EntityID, limbAnimComp);
    }

    private void StopAnimation()
    {
        isAnimating = false;

        GetComponentValues();
        limbAnimComp.m_frameNumber = 0;
        limbAnimComp.m_isAnimating = isAnimating;

        Component.Set<AnimationComponent>(EntityID, limbAnimComp);
    }

    private void CheckWeapons()
    {
        switch (limbTag)
        {
            case "LeftLimbSprite":
                weaponEquipped = PlayerLoadoutManager.leftLimbEquippedNo;
                break;

            case "RightLimbSprite":
                weaponEquipped = PlayerLoadoutManager.rightLimbEquippedNo;
                break;

            case "BackLimbSprite":
                weaponEquipped = PlayerLoadoutManager.backLimbEquippedNo;
                break;

            default:
                break;
        }

        ChangeWeaponSprite();
    }

    private void ChangeWeaponSprite()
    {
        if (limbTag == "LeftLimbSprite" || limbTag == "RightLimbSprite")
        {
            switch (weaponEquipped)
            {
                case 0:
                    SwitchWeapon("Gun");
                    break;

                case 1:
                    SwitchWeapon("Katana");
                    break;

                case 2:
                    SwitchWeapon("Booster");
                    break;

                case 3:
                    SwitchWeapon("ShotGun");
                    break;

                default:
                    break;
            }
        }
        
        else if (limbTag == "BackLimbSprite")
        {
            switch (weaponEquipped)
            {
                case 0:
                    SwitchWeapon("Gun");
                    break;

                case 1:
                    SwitchWeapon("Booster");
                    break;

                default:
                    break;
            }
        }
    }

    private void CheckInputs()
    {
        #region Mouse Checks

        if (InternalCall.m_InternalCallIsKeyTriggered(keyCode.LMB) && limbTag == "LeftLimbSprite" && PlayerLoadoutManager.isSortieing == false)
        {
            switch (weaponEquipped)
            {
                case 0:
                    AttemptShootGun();
                    break;

                case 1:
                    AttemptMelee();
                    break;
                case 3:
                    AttemptShootShotGun();
                    break;

                default:
                    break;
            }
        }

        if (InternalCall.m_InternalCallIsKeyTriggered(keyCode.RMB) && limbTag == "RightLimbSprite" && PlayerLoadoutManager.isSortieing == false)
        {
            switch (weaponEquipped)
            {
                case 0:
                    AttemptShootGun();
                    break;

                case 1:
                    AttemptMelee();
                    break;
                case 3:
                    AttemptShootShotGun();
                    break;

                default:
                    break;
            }
        }

        #endregion

        #region Keyboard Checks
        if (InternalCall.m_InternalCallIsKeyTriggered(keyCode.SPACE) && limbTag == "BackLimbSprite" && PlayerLoadoutManager.isSortieing == false)
        {
            switch (weaponEquipped)
            {
                case 0:
                    AttemptShootGun();
                    break;

                default:
                    break;
            }
        }
        #endregion
    }
}