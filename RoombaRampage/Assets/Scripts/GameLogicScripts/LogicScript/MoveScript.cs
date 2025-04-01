using System;

public class MoveScript : ScriptBase
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

    public override void Update()
    {
        RigidBodyComponent rb = Component.Get<RigidBodyComponent>(EntityID);
        rb.m_Velocity.Y = -1.9f;
        Component.Set<RigidBodyComponent>(EntityID, rb);
    }
}