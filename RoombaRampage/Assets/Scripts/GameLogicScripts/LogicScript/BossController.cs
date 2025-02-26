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
        if (InternalCall.m_InternalCallIsKeyTriggered(keyCode.W))
        {
            AttackRandomizer();
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
                InternalCall.m_InternalCallAddPrefab(alternatePrefab, bossPosition.X, bossPosition.Y, 0);

                break;
            case BossAttackPattern.BulletSpread:
                Console.WriteLine("Firing Bullet Spread!");
                InternalCall.m_InternalCallAddPrefab(spreadPrefab, bossPosition.X, bossPosition.Y, 0);

                break;
            case BossAttackPattern.BulletDisperse:
                Console.WriteLine("Firing Bullet Disperse!");
                InternalCall.m_InternalCallAddPrefab(dispersePrefab, bossPosition.X, bossPosition.Y, 0);

                break;
        }
    }

    private int GenerateRandom(int min, int max)
    {
        seed = (seed * 1664525 + 1013904223) & 0x7FFFFFFF; // Update seed
        return (int)(min + (seed % (max - min))); // Map to range
    }
}