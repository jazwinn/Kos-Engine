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

    private uint playerID; //Store player ID
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

    #endregion

    #region Boss Attack Pattern
    public enum BossAttackPattern // Attack pattern
    {
        AlternatingBullet,
        BulletSpread,
        BulletDisperse
    };
    #endregion



    public override void Awake(uint id)
    {
        EntityID = id; //Sets ID for object, DO NOT TOUCH

    }
    public override void Start()
    {
        playerID = (uint)InternalCall.m_InternalCallGetTagID("Player"); //Get Player ID
        Console.WriteLine($"Selected Pattern: {currentPattern}");



    }

    public override void Update()
    {
        AttackRandomizer();
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

        Console.WriteLine($"Selected Pattern: {currentPattern}");

    }

    private void ExecutePattern(BossAttackPattern pattern)
    {
        switch (pattern)
        {
            case BossAttackPattern.AlternatingBullet:
                Console.WriteLine("Firing Alternating Bullets!");
                break;
            case BossAttackPattern.BulletSpread:
                Console.WriteLine("Firing Bullet Spread!");
                break;
            case BossAttackPattern.BulletDisperse:
                Console.WriteLine("Firing Bullet Disperse!");
                break;
        }
    }

    private int GenerateRandom(int min, int max)
    {
        seed = (seed * 1664525 + 1013904223) & 0x7FFFFFFF; // Update seed
        return (int)(min + (seed % (max - min))); // Map to range
    }
}