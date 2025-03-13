using System;

public class KillCounter : ScriptBase
{
    #region ID Variables
    private uint EntityID;
    private uint playerID;
    #endregion

    public static int killCount = 0;
    private string exitPrefab = "prefab_enemy";
    public override void Awake(uint id) //Called everytime instance is created
    {
        EntityID = id; //Sets ID for object, DO NOT TOUCH

    }

    public override void Start() //Called once at the start of the game
    {
        ResetKillCount();
    }

    public override void Update()
    {
        if (LevelSelection.SceneName == "Level5")
        {
            Console.WriteLine($"Kill Count: {killCount}");
            int spawnPoint = InternalCall.m_InternalCallGetTagID("ExitSpawn");
            if (killCount >= 5)
            {
                Vector2 spawnPointTransform;
                InternalCall.m_InternalGetTranslate((uint)spawnPoint, out spawnPointTransform);

                InternalCall.m_InternalCallAddPrefab(exitPrefab, spawnPointTransform.X, spawnPointTransform.Y, 0.0f);
            }
        }
    }

    public void IncrementKillCount()
    {
        killCount++;
        Console.WriteLine($"Kill Count: {killCount}"); 
    }

    public int GetKillCount()
    {
        return killCount;
    }

    public void ResetKillCount()
    {
        killCount = 0;
        Console.WriteLine("Kill Counter Reset"); // Help for debbugging if Ted need
    }

}
