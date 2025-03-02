using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text.RegularExpressions;

public class BossController : ScriptBase
{
    #region ID Variables
    private uint EntityID; //Entity ID of the object, do not touch!
    private uint forceFieldID; // ID for the force field prefab
    //private uint playerID; //Store player ID
    #endregion

    #region Component Variable
    private SpriteComponent spriteComp;
    private ColliderComponent collComp;

    //private AnimationComponent animComp;
    //private TransformComponent transformComp;
    //private TransformComponent playerTransformComp;
    #endregion

    #region Boss Variable
    public int playerHit = 0;
    //bool noForceField = false
    private BossAttackPattern currentPattern;
    private long seed = DateTime.Now.Ticks; // seed for randomizer

    private Vector2 bossPosition;
    private bool canAttack = true;
    private float attackCooldown = 3.0f;

    public int forceFieldHealth = 5; 
    public int bossHealth = 3;
    private bool isForceFieldActive = true;
    private string forceFieldPrefab = "Boss_Forcefield"; 

    #endregion

    #region Boss Attack Pattern
    public enum BossAttackPattern // Attack pattern
    {
        AlternatingBullet,
        BulletSpread,
        BulletDisperse,
        BulletSpiral
    };
    #endregion


    private string alternatePrefab;
    private string spreadPrefab;
    private string dispersePrefab;


    public override void Awake(uint id)
    {
        EntityID = id; //Sets ID for object, DO NOT TOUCH
        alternatePrefab = "prefab_bulletAlternate";
        spreadPrefab = "prefab_bulletSpread";
        dispersePrefab = "prefab_bulletDisperse";

        SpawnForceField();
    }
    public override void Start()
    {
        //EntityID = (uint)InternalCall.m_InternalCallGetTagID("Player"); //Get Player ID
        Console.WriteLine($"Selected Pattern: {currentPattern}");
    }

    public override void Update()
    {
        //if (InternalCall.m_InternalCallIsKeyTriggered(keyCode.SPACE) && canAttack)
        //{
        //    AttackRandomizer();
        //    canAttack = false;
        //    CoroutineManager.Instance.StartCoroutine(ResetAttackCooldown());
        //}

        CheckCollision();


    }

    public void AttackRandomizer()
    {
        BossAttackPattern newPattern;

        do
        {
            Console.WriteLine("Randomizing");
            int randomIndex = GenerateRandom(0, Enum.GetValues(typeof(BossAttackPattern)).Length);
            newPattern = (BossAttackPattern)randomIndex;

        }
        while (newPattern == currentPattern);

        currentPattern = newPattern;
        ExecutePattern(currentPattern);

        Console.WriteLine($"Selected Pattern: {currentPattern}");
    }

    private void ExecutePattern(BossAttackPattern pattern)
    {
        InternalCall.m_InternalGetTranslate(EntityID, out bossPosition);
        switch (pattern)
        {
            case BossAttackPattern.AlternatingBullet:
                Console.WriteLine("Firing Alternating Bullets!");
                CoroutineManager.Instance.StartCoroutine(FireAlternatingWaves());

                break;
            case BossAttackPattern.BulletSpread:
                Console.WriteLine("Firing Bullet Spread!");
                SpawnBullets(bossPosition, 8, 0.2f, 180f, 360f);   

                break;
            case BossAttackPattern.BulletDisperse:
                Console.WriteLine("Firing Bullet Disperse!");
                SpawnBulletDisperse(bossPosition);

                break;
            case BossAttackPattern.BulletSpiral:
                Console.WriteLine("Firing Bullet Spiral!");
                CoroutineManager.Instance.StartCoroutine(FireSweepingCurvingBulletVolley(bossPosition,10,0.2f,0.1f));

                break;
        }
    }

    private int GenerateRandom(int min, int max)
    {
        seed = (seed * 1664525 + 1013904223) & 0x7FFFFFFF; // Update seed
        return (int)(min + (seed % (max - min))); // Map to range
    }

    private IEnumerator ResetAttackCooldown()
    {
        yield return new CoroutineManager.WaitForSeconds(attackCooldown);
        canAttack = true;
    }


    private IEnumerator FireAlternatingWaves()
    {
        int waveCount = 3; // Number of waves 
        float waveDelay = 0.3f; // Time between waves
        float positionOffset = 10f; // Angle offset to shift bullets 

        for (int i = 0; i < waveCount; i++)
        {
            float waveAngleOffset = (i % 2 == 0) ? positionOffset : -positionOffset;
            SpawnBullets(bossPosition, 8, 0.2f, 180f + waveAngleOffset, 360f + waveAngleOffset);
            yield return new CoroutineManager.WaitForSeconds(waveDelay);
        }
    }

    private void SpawnBullets(Vector2 position, int bulletCount, float radius, float startAngle, float endAngle)
    {
        float angleStep = (endAngle - startAngle) / (bulletCount - 1);

        for (int i = 0; i < bulletCount; i++)
        {
            float angle = startAngle + (angleStep * i);
            float radian = angle * (float)(Math.PI / 180f);
            float spawnX = position.X + (float)Math.Cos(radian) * radius;
            float spawnY = position.Y + (float)Math.Sin(radian) * radius;

            InternalCall.m_InternalCallAddPrefab(alternatePrefab, spawnX, spawnY, angle);
        }
    }

    private void SpawnBulletDisperse(Vector2 position)
    {
        float bigBulletAngle = 270f;

        uint largeBullet = (uint)InternalCall.m_InternalCallAddPrefab(dispersePrefab, position.X, position.Y, bigBulletAngle);

        CoroutineManager.Instance.StartCoroutine(ExplodeBigBullet(largeBullet));
    }

    private IEnumerator ExplodeBigBullet(uint largeBullet)
    {
        yield return new CoroutineManager.WaitForSeconds(1.5f);

        InternalCall.m_InternalGetTranslate(largeBullet, out Vector2 explosionPosition);

        List<uint> mediumBullets = new List<uint>();

        // First explosion (i = number of bullet)
        for (int i = 0; i < 6; i++)
        {
            float angle = i * (360f / 6); // Spread evenly
            uint mediumBullet = (uint)InternalCall.m_InternalCallAddPrefab(alternatePrefab, explosionPosition.X, explosionPosition.Y, angle);
            mediumBullets.Add(mediumBullet);
        }

       InternalCall.m_InternalCallDeleteEntity(largeBullet);

        // Delay 
        yield return new CoroutineManager.WaitForSeconds(1f);

        // Second explosion
        foreach (uint mediumBullet in mediumBullets)
        {
            InternalCall.m_InternalGetTranslate(mediumBullet, out Vector2 mediumBulletPosition);

            for (int j = 0; j < 8; j++)
            {
                float spreadAngle = (j * 60f) - 30f; 
                float radian = (float)(spreadAngle * (Math.PI / 180f));
                float offsetX = (float)(mediumBulletPosition.X + Math.Cos(radian) * 0.3f);
                float offsetY = (float)(mediumBulletPosition.Y + Math.Sin(radian) * 0.3f);

                InternalCall.m_InternalCallAddPrefab(spreadPrefab, offsetX, offsetY, spreadAngle);
            }

            InternalCall.m_InternalCallDeleteEntity(mediumBullet);
        }

    }

    private IEnumerator FireSweepingCurvingBulletVolley(Vector2 position, int bulletCount, float radius, float bulletInterval)
    {

        for (int direction = 0; direction < 2; direction++) // 0 = Left to Right, 1 = Right to Left
        {
            bool reverse = (direction == 1);

            for (int i = 0; i < bulletCount; i++)
            {
                int index = reverse ? (bulletCount - 1 - i) : i;
                float baseAngle = 180f + ((180f / (bulletCount - 1)) * index);
                float radian = (float)(baseAngle * (Math.PI / 180f));

                float spawnX = position.X + (float)Math.Cos(radian) * radius;
                float spawnY = position.Y + (float)Math.Sin(radian) * radius;

                uint bullet = (uint)InternalCall.m_InternalCallAddPrefab(alternatePrefab, spawnX, spawnY, baseAngle);
                CoroutineManager.Instance.StartCoroutine(CurveBullet(bullet, baseAngle));

                yield return new CoroutineManager.WaitForSeconds(bulletInterval);
            }
            yield return new CoroutineManager.WaitForSeconds(0.2f);
        }
    }

    private IEnumerator CurveBullet(uint bulletID, float initialAngle)
    {
        float speed = 2.0f, acceleration = 0.05f, maxSpeed = 10.0f, curveStrength = 0.5f;

        while (InternalCall.m_InternalGetTranslate(bulletID, out Vector2 bulletPos))
        {
            speed = Math.Min(speed + acceleration, maxSpeed);
            initialAngle += curveStrength;
            float radian = (float)(initialAngle * (Math.PI / 180f));

            InternalCall.m_InternalSetVelocity(bulletID, new Vector2((float)Math.Cos(radian) * speed, (float)Math.Sin(radian) * speed));

            yield return null;
        }
    }

    private void SpawnForceField()
    {
        InternalCall.m_InternalGetTranslate(EntityID, out Vector2 bossPosition);
        forceFieldID = (uint)InternalCall.m_InternalCallAddPrefab(forceFieldPrefab, bossPosition.X, bossPosition.Y, 0);
        isForceFieldActive = true;
        forceFieldHealth = 5;
    }

    private void HandleDamage(int bulletID)
    {
        InternalCall.m_InternalCallDeleteEntity((uint)bulletID); // Destroy the bullet
        InternalCall.m_InternalGetTranslate(EntityID, out Vector2 bossPosition);

        if (isForceFieldActive)
        {
            forceFieldHealth--;

            Console.WriteLine($"[Boss] Force Field Hit! Remaining: {forceFieldHealth}");

            if (forceFieldHealth <= 0)
            {
                Console.WriteLine("[Boss] Force Field Down!");
                isForceFieldActive = false;
            }
        }
        else
        {
            bossHealth--;

            Console.WriteLine($"[Boss] Health Remaining: {bossHealth}");

            if (bossHealth <= 0)
            {
                Console.WriteLine("[Boss] Respawning Force Field!");
                SpawnForceField();
            }
        }
    }

    private void CheckCollision()
    {
        if (InternalCall.m_InternalCallIsCollided(EntityID) != 0.0f)
        {
            int[] collidedEntities = InternalCall.m_InternalCallGetCollidedEntities(EntityID);

            foreach (int collidedEntitiesID in collidedEntities)
            {
                if (InternalCall.m_InternalCallGetTag((uint)collidedEntitiesID) == "PlayerBullet")
                {
                    Console.WriteLine("STOP");
                }
            }
        }
    }

}