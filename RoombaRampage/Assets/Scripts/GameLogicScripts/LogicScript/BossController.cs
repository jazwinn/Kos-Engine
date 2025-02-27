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

    //private uint playerID; //Store player ID
    #endregion

    #region Component Variable
    private SpriteComponent spriteComp;
    private AnimationComponent animComp;
    private ColliderComponent collComp;
    private TransformComponent transformComp;
    private TransformComponent playerTransformComp;
    #endregion

    #region Boss Variable
    public int forceFieldHealth = 3;
    public int playerHit = 0;
    bool noForceField = false;

    private BossAttackPattern currentPattern;
    private long seed = DateTime.Now.Ticks; // seed for randomizer

    private Vector2 bossPosition;
    private bool canAttack = true;
    private float attackCooldown = 3.0f;


    #endregion

    #region Boss Attack Pattern
    public enum BossAttackPattern // Attack pattern
    {
        AlternatingBullet,
        BulletSpread,
        BulletDisperse
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
    }
    public override void Start()
    {
        //EntityID = (uint)InternalCall.m_InternalCallGetTagID("Player"); //Get Player ID
        Console.WriteLine($"Selected Pattern: {currentPattern}");
    }

    public override void Update()
    {
        if (InternalCall.m_InternalCallIsKeyTriggered(keyCode.W) && canAttack)
        {
            AttackRandomizer();
            canAttack = false;
            CoroutineManager.Instance.StartCoroutine(ResetAttackCooldown());
        }
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
                //InternalCall.m_InternalCallAddPrefab(alternatePrefab, bossPosition.X, bossPosition.Y, 0);
                CoroutineManager.Instance.StartCoroutine(FireAlternatingWaves());

                //SpawnBulletAlternate(bossPosition, 8, 0.2f);

                break;
            case BossAttackPattern.BulletSpread:
                Console.WriteLine("Firing Bullet Spread!");
                //InternalCall.m_InternalCallAddPrefab(spreadPrefab, bossPosition.X, bossPosition.Y, 0);
                SpawnBulletAlternate(bossPosition, 8, 0.2f);

                break;
            case BossAttackPattern.BulletDisperse:
                Console.WriteLine("Firing Bullet Disperse!");
                //InternalCall.m_InternalCallAddPrefab(dispersePrefab, bossPosition.X, bossPosition.Y, 0);
                //SpawnBulletAlternate(bossPosition, 8, 0.2f);
                SpawnBulletDisperse(bossPosition);

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

    private void SpawnBulletAlternate(Vector2 position, int bulletCount, float radius)
    {
        for (int i = 0; i < bulletCount; i++)
        {
            float angle = 180f + ((180f / (bulletCount - 1)) * i); 
            float radian = angle * (float)(Math.PI / 180f); 

            float spawnX = position.X + (float)Math.Cos(radian) * radius;
            float spawnY = position.Y + (float)Math.Sin(radian) * radius;


            InternalCall.m_InternalCallAddPrefab(alternatePrefab, spawnX, spawnY, angle);
        }
    }

    //  Attack 1: Alternating Bullet
    private bool reverseWave = false;

    private IEnumerator FireAlternatingWaves()
    {
        int waveCount = 3; // Number of waves 
        float waveDelay = 0.3f; // Time between waves
        float positionOffset = 10f; // Angle offset to shift bullets 

        for (int i = 0; i < waveCount; i++)
        {
            float waveAngleOffset = reverseWave ? positionOffset : -positionOffset; // Alternate offset direction

            SpawnBulletSemiCircle(bossPosition, 8, 0.2f, reverseWave, waveAngleOffset);
            reverseWave = !reverseWave; // Alternate direction
            yield return new CoroutineManager.WaitForSeconds(waveDelay);
        }
    }

    private void SpawnBulletSemiCircle(Vector2 position, int bulletCount, float radius, bool reverse, float waveAngleOffset)
    {
        for (int i = 0; i < bulletCount; i++)
        {
            float baseAngle = reverse ?
                       360f - ((180f / (bulletCount - 1)) * i)  // Reverse wave (360° to 180°)
                       : 180f + ((180f / (bulletCount - 1)) * i); // Normal wave (180° to 360°)

            float shiftedAngle = baseAngle + waveAngleOffset; // Apply the wave offset
            float radian = shiftedAngle * (float)(Math.PI / 180f); 

            float spawnX = position.X + (float)Math.Cos(radian) * radius;
            float spawnY = position.Y + (float)Math.Sin(radian) * radius;


            InternalCall.m_InternalCallAddPrefab(alternatePrefab, spawnX, spawnY, shiftedAngle);
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
        yield return new CoroutineManager.WaitForSeconds(1.5f); // Delay before first explosion


        InternalCall.m_InternalGetTranslate(largeBullet, out Vector2 explosionPosition);

        for (int i = 0; i < 6; i++)
        {
            float angle = i * (360f / 6);
            InternalCall.m_InternalCallAddPrefab(alternatePrefab, explosionPosition.X, explosionPosition.Y, angle);
        }

        // Delay for second explosion
        yield return new CoroutineManager.WaitForSeconds(0.2f);

        // Second explosion
        for (int i = 0; i < 6; i++)
        {
            float angle = i * (360f / 6);
            InternalCall.m_InternalCallAddPrefab(spreadPrefab, explosionPosition.X, explosionPosition.Y, angle);
        }
    }
}