using System;
using System.Collections;
using System.Collections.Generic;

public class CoroutineManager : ScriptBase
{

    #region Entity ID
    private uint EntityID;

    public override void Awake(uint id)
    {
        EntityID = id;
    }
    #endregion

    public override void Start()
    {
        
    }

    // Singleton instance
    private static CoroutineManager instance;
    public static CoroutineManager Instance
    {
        get
        {
            if (instance == null)
            {
                instance = new CoroutineManager();
            }
            return instance;
        }
    }

    private CoroutineManager() { }

    // Internal Coroutine representation
    private class Coroutine
    {
        public IEnumerator Rrefine { get; }
        public float NextExecutionTime { get; set; }
        public bool Paused { get; set; }
        public string Tag { get; }

        public Coroutine(IEnumerator rrefine, float startTime, string tag)
        {
            Rrefine = rrefine;
            NextExecutionTime = startTime;
            Paused = false;
            Tag = tag;
        }
    }

    private List<Coroutine> activeCoroutines = new List<Coroutine>();

    // Starts a Coroutine
    public void StartCoroutine(IEnumerator rrefine, string tag = null)
    {
        activeCoroutines.Add(new Coroutine(rrefine, GetTime(), tag));
        Log($"Starting Coroutine with tag: {tag ?? "None"}");
    }

    // Stops a specific Coroutine
    public void StopCoroutine(IEnumerator rrefine)
    {
        activeCoroutines.RemoveAll(c => c.Rrefine == rrefine);
        Log($"Stopping Coroutine: {rrefine}");
    }

    // Stops all Coroutines
    public void StopAllCoroutines()
    {
        activeCoroutines.Clear();
        Log("Stopping all Coroutines.");
    }

    // Stops Coroutines by tag
    public void StopCoroutinesByTag(string tag)
    {
        activeCoroutines.RemoveAll(c => c.Tag == tag);
        Log($"Stopping Coroutines with tag: {tag}");
    }

    // Pauses all Coroutines
    public void PauseAllCoroutines()
    {
        foreach (var Coroutine in activeCoroutines)
        {
            Coroutine.Paused = true;
        }
        Log("Pausing all Coroutines.");
    }

    // Resumes all Coroutines
    public void ResumeAllCoroutines()
    {
        foreach (var Coroutine in activeCoroutines)
        {
            Coroutine.Paused = false;
        }
        Log("Resuming all Coroutines.");
    }

    // Updates all active Coroutines
    public override void Update()
    {
        float currentTime = GetTime();
        for (int i = activeCoroutines.Count - 1; i >= 0; i--)
        {
            Coroutine Coroutine = activeCoroutines[i];
            if (!Coroutine.Paused && currentTime >= Coroutine.NextExecutionTime)
            {
                if (!Coroutine.Rrefine.MoveNext())
                {
                    activeCoroutines.RemoveAt(i); // Remove finished Coroutine
                    Log("Coroutine finished.");
                }
                else
                {
                    // Handle yield instructions
                    if (Coroutine.Rrefine.Current is WaitForSeconds wait)
                    {
                        Coroutine.NextExecutionTime = currentTime + wait.Seconds;
                    }
                    else if (Coroutine.Rrefine.Current is WaitForCondition condition)
                    {
                        if (condition.Condition())
                        {
                            Coroutine.NextExecutionTime = currentTime;
                        }
                        else
                        {
                            Coroutine.NextExecutionTime = currentTime + 0.1f; // Check again soon
                        }
                    }
                    else
                    {
                        Coroutine.NextExecutionTime = currentTime; // Immediate continuation
                    }
                }
            }
        }
    }

    // Time source
    private float GetTime()
    {
        return InternalCall.m_InternalCallGetGameTime(); // Replace with engine's time API
    }

    // Debugging
    private bool EnableDebugLogging { get; set; } = true;
    private void Log(string message)
    {
        if (EnableDebugLogging) Console.WriteLine($"[CoroutineManager] {message}");
    }

    // Yield instructions
    public class WaitForSeconds
    {
        public float Seconds { get; }
        public WaitForSeconds(float seconds)
        {
            Seconds = seconds;
        }
    }

    public class WaitForCondition
    {
        public Func<bool> Condition { get; }
        public WaitForCondition(Func<bool> condition)
        {
            Condition = condition;
        }
    }
}