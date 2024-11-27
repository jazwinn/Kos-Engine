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

    public override void GetEntityID(uint id)
    {
        EntityID = id;
    }

    public override void Start()
    {
        InternalCall.m_InternalGetTransformComponent(EntityID, out startCamPos, out startCamScale, out startCamRotate);

        playerID = InternalCall.m_InternalCallGetTagID("Player");

        if (playerID < 0)
        {
            Console.WriteLine("Player Component not present in entity");
        }

        int[] collidedEntities = InternalCall.m_InternalCallGetTagIDs("Default");
        foreach (var tagid in collidedEntities)
        {

            Console.WriteLine($"Default tag is {tagid}");
        }
    }

    public override void Update()
    {
        if (playerID < 0) return;
        InternalCall.m_InternalGetTransformComponent((uint)playerID, out pos, out playerScale, out playerRotate);
        InternalCall.m_InternalSetTransformComponent(EntityID, pos, startCamScale, startCamRotate);
    }
}