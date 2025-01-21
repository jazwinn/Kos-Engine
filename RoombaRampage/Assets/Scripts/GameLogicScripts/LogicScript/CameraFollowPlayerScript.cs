using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

public class CameraFollowPlayerScript : ScriptBase
{
    private uint EntityID;
    private int playerID;
    private Vector2 pos;
    private Vector2 playerScale;
    private float playerRotate;

    private Vector2 startCamPos;
    private Vector2 startCamScale;
    private float startCamRotate;

    //private float deltatime;

    //private static Vector2 previousplayerpos;

    public override void Awake(uint id)
    {
        EntityID = id;

        InternalCall.m_InternalGetTransformComponent(EntityID, out startCamPos, out startCamScale, out startCamRotate);

        playerID = InternalCall.m_InternalCallGetTagID("Player");
    }

    public override void Start()
    {


        //if (playerID < 0)
        //{
        //    Console.WriteLine("Player Component not present in entity");
        //}

        //int[] collidedEntities = InternalCall.m_InternalCallGetTagIDs("Default");
        //foreach (var tagid in collidedEntities)
        //{

        //    //Console.WriteLine($"Default tag is {tagid}");
        //}
    }

    public override void Update()
    {
        if (playerID < 0) return;
        InternalCall.m_InternalGetTransformComponent((uint)playerID, out pos, out playerScale, out playerRotate);
        InternalCall.m_InternalGetTransformComponent(EntityID, out startCamPos, out startCamScale, out startCamRotate);
        //deltatime = InternalCall.m_InternalCallGetDeltaTime();

        //Vector2 targetpos = Mix(previousplayerpos, pos, deltatime);
        //Vector2 cameraCoord = Mix(startCamPos, targetpos, deltatime * 5.0f);
        InternalCall.m_InternalSetTransformComponent(EntityID, pos, startCamScale, startCamRotate);

        //previousplayerpos = pos;
    }

    //public Vector2 Mix(Vector2 lhs, Vector2 rhs, float time)
    //{
    //    float oneMinusTime = 1 - time;

    //    float x = (lhs.X * oneMinusTime) + (rhs.X * time);
    //    float y = (lhs.Y * oneMinusTime) + (rhs.Y * time);

    //    return new Vector2(x, y);
    //}

}