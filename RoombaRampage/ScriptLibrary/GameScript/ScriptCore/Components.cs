
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

public struct Vector2
{
    public Vector2(float x, float y)
    {
        X = x;
        Y = y;
    }

    public float X, Y;
}

public struct Vector3
{

    public float R, G, B;
}

public enum CollisionFlag
{
    NONE = 0,
    LEFT = 1 << 0,
    RIGHT = 1 << 1,
    UP = 1 << 2,
    DOWN = 1 << 3,

}
public class TransformComponent
{
    public Vector2 m_position;
    public float m_rotation;
    public Vector2 m_scale;
}

public class RigidBodyComponent
{
    public Vector2 m_Velocity;
    public Vector2 m_Acceleration;
    public float m_Rotation;
    public float m_AngularVelocity;
    public float m_AngularAcceleration;
    public float m_Mass;
    public float m_InverseMass;
    public float m_LinearDamping;
    public float m_AngularDamping;
    public Vector2 m_Force;
    public float m_Torque;
    public bool m_IsKinematic;
    public bool m_IsStatic;
    public Vector2 m_prevPos;
    public Vector2 m_direction;

}

public class ColliderComponent
{
    public Vector2 m_Size;
    public Vector2 m_Offset;
    public bool m_drawDebug;
    public bool m_collisionResponse;
    public bool m_collisionCheck;
    public float m_radius;
    public float m_isCollided;
    public uint m_blockedFlag;
}

public class EnemyComponent
{
    public int m_tag;
    public int m_enemyTypeInt;
    public int m_enemyRoamBehaviourInt;
}

public class TextComponent
{
    public string m_text;
    public string m_fileName;
    public int m_fontLayer;
    public float m_fontSize;

    public Vector3 m_color;
}

public class SpriteComponent
{
    public string m_imageFile;
    public int m_layer;
    public Vector3 m_color;
    public float m_alpha;
}

public class AnimationComponent
{
    public int m_frameNumber;
    public int m_framesPerSecond;
    public float m_frameTimer;
    public bool m_isAnimating;
    public int m_stripCount;
}

public class CameraComponent
{
    public float m_left;
    public float m_right;
    public float m_top;
    public float m_bottom;
    public float m_aspectRatio;
}

public class ButtomComponent
{
    public Vector2 m_Position;
    public Vector2 m_Scale;
    public bool m_IsClick;
}

public class PathfindingComponent
{
    public Vector2 m_startPosition;
    public Vector2 m_targetPosition;
    public int m_gridkey;
    public List<Vector2> m_node;
}

public class Raycast
{
    [MarshalAs(UnmanagedType.I1)]
    public bool m_isRaycasting;
    public Vector2 m_targetposition;
    public float m_distance;
    [MarshalAs(UnmanagedType.I1)]
    public bool m_targetReached;
    public Vector2 m_hitPosition;
}