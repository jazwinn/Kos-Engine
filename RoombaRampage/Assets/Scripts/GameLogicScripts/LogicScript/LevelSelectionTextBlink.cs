using System;
using System.Collections;

public class LevelSelectionTextBlink : ScriptBase
{
    #region Entity ID
    private uint EntityID;

    public override void Awake(uint id)
    {
        EntityID = id;
    }
    #endregion

    private TextComponent textComp;

    public override void Start()
    {
        textComp = Component.Get<TextComponent>(EntityID);
        CoroutineManager.Instance.StartCoroutine(BlinkText(), "BlinkText");
    }

    public override void Update()
    {

    }

    private IEnumerator BlinkText()
    {
        while (true)
        {
            float bpm = 120f;
            float beatDuration = 60f / bpm; // One beat in seconds (0.5s for 120 BPM)
            float nextBlinkTime = CoroutineManager.Instance.GetTime();

            while (true)
            {
                textComp.m_fontLayer = -1;
                Component.Set<TextComponent>(EntityID, textComp);

                nextBlinkTime += beatDuration;
                yield return new CoroutineManager.WaitForSeconds(nextBlinkTime - CoroutineManager.Instance.GetTime());

                textComp.m_fontLayer = 10;
                Component.Set<TextComponent>(EntityID, textComp);

                nextBlinkTime += beatDuration;
                yield return new CoroutineManager.WaitForSeconds(nextBlinkTime - CoroutineManager.Instance.GetTime());
            }
        }

    }
}