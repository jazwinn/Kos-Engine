using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

public class CollisionResponse : ScriptBase
{
    private uint EntityID;

    public override void GetEntityID(uint id)
    {
        EntityID = id;
    }

    private Vector2 startingPlayerPos;
    private Vector2 startingPlayerScale;
    private float startingPlayerRotate;

    private Vector2 playerVelocity;
    private Vector2 playerAcceleration;
    private float playerRotationalForce;

    private Vector2 colSize;
    private Vector2 colOffset;
    private bool colDraw;
    private float colRadius;
    private int colFlag;
    private float isCollided;
    private bool colCheck;
 

    public override void Start()
    {
        InternalCall.m_InternalGetTransformComponent(EntityID, out startingPlayerPos, out startingPlayerScale, out startingPlayerRotate);
        InternalCall.m_InternalGetRigidBodyComponent(EntityID,out playerVelocity, out playerAcceleration,out playerRotationalForce);
        InternalCall.m_InternalGetColliderComponent(EntityID,out colSize,out colOffset, out colDraw,out colRadius,out colFlag,out isCollided,out colCheck); 

    }

    public override void Update()
    {
        #region Collision

       
        if (InternalCall.m_InternalCallGetTagIDs("Wall") != null)
        {
            int[] wallEntities = InternalCall.m_InternalCallGetTagIDs("Wall");
            foreach (int wallEntitiesID in wallEntities)
            {
                Vector2 wallSize, wallOffset, wallPos, wallScale;
                float wallRadius, wallRotate, wallCollided;
                bool wallDraw, wallCheck;
                int wallFlag;

                InternalCall.m_InternalGetColliderComponent((uint)wallEntitiesID, out wallSize, out wallOffset, out wallDraw, out wallRadius, out wallFlag, out wallCollided, out wallCheck);
                InternalCall.m_InternalGetTransformComponent((uint)wallEntitiesID, out wallPos, out wallScale, out wallRotate);

                float xMin = wallPos.X + wallOffset.X - (wallSize.X * wallScale.X) / 2;
                float xMax = wallPos.X + wallOffset.X + (wallSize.X * wallScale.X) / 2;
                float yMin = wallPos.Y + wallOffset.Y - (wallSize.Y * wallScale.Y) / 2;
                float yMax = wallPos.Y + wallOffset.Y + (wallSize.Y * wallScale.Y) / 2;

                float exMin = xMin - colRadius;
                float exMax = xMax + colRadius;
                float eyMin = yMin - colRadius;
                float eyMax = yMax + colRadius;

                // Console.WriteLine($"Wall Pos: ({wallPos.X}, {wallPos.Y}), Wall Size: ({wallSize.X}, {wallSize.Y})");
                //Console.WriteLine($"Expanded AABB: ({exMin}, {exMax}, {eyMin}, {eyMax})");



                Vector2 playerVelocity;
                InternalCall.m_InternalGetVelocity(EntityID, out playerVelocity);
                Vector2 startingPlayerPos, startingPlayerScale;
                float startingPlayerRotate;
                InternalCall.m_InternalGetTransformComponent(EntityID, out startingPlayerPos, out startingPlayerScale, out startingPlayerRotate);


                int steps = InternalCall.m_InternalCallGetSteps();
                float delta = InternalCall.m_InternalCallGetDeltaTime();
                if (steps > 1)
                {
                    delta *= steps;
                    //Console.WriteLine(steps);
                }
                float cxStart = startingPlayerPos.X;
                float cyStart = startingPlayerPos.Y;
                float cxEnd = cxStart + playerVelocity.X * delta;
                float cyEnd = cyStart + playerVelocity.Y * delta;

                // Console.WriteLine(delta);

                if (!(cxEnd > exMin && cxEnd < exMax && cyEnd < eyMax && cyEnd > eyMin))
                {
                    continue;
                }
                else if ((cxStart > xMin + colRadius && cxStart < xMax - colRadius && cyStart < eyMax - colRadius && cyStart > eyMin + colRadius)) //Completely Inside
                {
                    // Circle's position (cxStart, cyStart) and radius
                    float circleRadius = colRadius;

                    // Calculate distances from the circle to each edge of the AABB
                    float distLeft = cxStart - exMin;
                    float distRight = exMax - cxStart;
                    float distTop = eyMax - cyStart;
                    float distBottom = cyStart - eyMin;

                    // Determine the closest edge
                    float minDist = Math.Min(Math.Min(distLeft, distRight), Math.Min(distTop, distBottom));

                    // Push the circle outside the closest edge
                    if (minDist == distLeft)
                    {
                        cxStart = exMin - circleRadius; // Push left
                    }
                    else if (minDist == distRight)
                    {
                        cxStart = exMax + circleRadius; // Push right
                    }
                    else if (minDist == distTop)
                    {
                        cyStart = eyMax + circleRadius; // Push up
                    }
                    else if (minDist == distBottom)
                    {
                        cyStart = eyMin - circleRadius; // Push down
                    }

                    // Update the entity's position
                    Vector2 newPos = new Vector2(cxStart, cyStart);
                    InternalCall.m_InternalSetTranslate(EntityID, in newPos);
                    continue;
                }
                else if ((cxStart > exMin && cxStart < exMax && cyStart < eyMax && cyStart > eyMin)) // Slightly Outside
                {
                    // Circle's position (cxStart, cyStart) and radius
                    float circleRadius = colRadius;

                    float closestX = cxStart;
                    if (cxStart < xMin) closestX = xMin;
                    if (cxStart > xMax) closestX = xMax;

                    float closestY = cyStart;
                    if (cyStart < yMin) closestY = yMin;
                    if (cyStart > yMax) closestY = yMax;

                    //Console.WriteLine(eyMax);

                    // Calculate the vector from the circle's center to the closest point on the AABB
                    float offsetX = cxStart - closestX;
                    float offsetY = cyStart - closestY;

                    // Calculate the distance from the circle's center to the closest point on the AABB
                    float distance = (float)Math.Sqrt(offsetX * offsetX + offsetY * offsetY);

                    // If the circle is overlapping the AABB, move it outside
                    if (distance < circleRadius)
                    {
                        // Calculate the amount to push the circle outside of the AABB
                        float pushDistance = circleRadius - distance;

                        // Normalize the offset to get the direction
                        if (distance == 0)
                        {
                            continue;
                        }
                        float normalizedX = offsetX / distance;
                        float normalizedY = offsetY / distance;

                        // Apply the push to the circle’s position (move it outside the AABB)
                        cxStart += normalizedX * pushDistance * 1.01f;
                        cyStart += normalizedY * pushDistance * 1.01f;



                        // Update the entity's position
                        Vector2 newPos;
                        newPos.X = cxStart;
                        newPos.Y = cyStart;
                        InternalCall.m_InternalSetTranslate(EntityID, in newPos);
                    }
                    continue;
                }



                //Console.WriteLine($"Start Pos: ({cxStart}, {cyStart}), End Pos: ({cxEnd}, {cyEnd})");

                //Console.WriteLine($"Velocity: ({playerVelocity.X}, {playerVelocity.Y}), Delta: {delta}");


                float dx = cxEnd - cxStart;
                float dy = cyEnd - cyStart;
                float tMin = 0.0f, tMax = 1.0f;

                if (dx != 0.0f)
                {
                    float t1 = (exMin - cxStart) / dx;
                    float t2 = (exMax - cxStart) / dx;
                    if (t1 > t2) (t1, t2) = (t2, t1);
                    tMin = Math.Max(tMin, t1);
                    tMax = Math.Min(tMax, t2);
                    // Console.WriteLine($"Axis: X, t1: {t1}, t2: {t2}, tMin: {tMin}, tMax: {tMax}");
                }

                if (dy != 0.0f)
                {
                    float t1 = (eyMin - cyStart) / dy;
                    float t2 = (eyMax - cyStart) / dy;
                    if (t1 > t2) (t1, t2) = (t2, t1);
                    tMin = Math.Max(tMin, t1);
                    tMax = Math.Min(tMax, t2);
                    // Console.WriteLine($"Axis: Y, t1: {t1}, t2: {t2}, tMin: {tMin}, tMax: {tMax}");
                }



                if (tMin <= tMax + 0.001f)
                {
                    float tImpact = tMin;
                    float newPosX = cxStart + tImpact * dx;
                    float newPosY = cyStart + tImpact * dy;

                    // Console.WriteLine($"Start Pos: ({cxStart}, {cyStart}), End Pos: ({newPosX}, {newPosY})");

                    Vector2 newPos;
                    newPos.X = newPosX;
                    newPos.Y = newPosY;

                    InternalCall.m_InternalSetTranslate(EntityID, in newPos);

                    Vector2 movement;

                    movement.X = 0;
                    movement.Y = 0;

                    if (tMax != 0)
                    {
                        InternalCall.m_InternalSetVelocity(EntityID, movement);
                    }

                }
            }
        }
                
        

        #endregion

    }
}