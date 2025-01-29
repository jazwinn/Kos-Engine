using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using System.Reflection.Emit;
using System.Data.SqlClient;

public static class InternalCall
{

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalGetTransformComponent(uint entity, out Vector2 pos, out Vector2 scale, out float rotate);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalSetTransformComponent(uint entity, in Vector2 pos, in Vector2 scale, in float rotate);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalGetTranslate(uint entity, out Vector2 pos);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalSetTranslate(uint entity, in Vector2 pos);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalGetColliderComponent(uint entity, out Vector2 size, out Vector2 offset, out bool drawDebug, out float radius, out uint bockflag, out float isCollided, out bool collisionCheck);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalSetColliderComponent(uint entity, in Vector2 size, in Vector2 offset, in bool drawDebug, in float radius, in uint bockflag, in float isCollided, in bool collisionCheck);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalGetEnemyComponent(uint entity, out int enemyTag, out int enemytype, out int enemybehaviour);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalSetEnemyComponent(uint entity, in int enemyTag, in int enemytype, in int enemybehaviour);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalGetRigidBodyComponent(uint entity, out Vector2 velocity, out Vector2 acceleration, out float rotation, out Vector2 previouspos, out Vector2 direction);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalSetRigidBodyComponent(uint entity, in Vector2 velocity, in Vector2 acceleration, in float rotation, in Vector2 previouspos, in Vector2 direction);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalGetTextComponent(uint entity, out string text, out string fileName, out int fontLayer, out float fontSize, out Vector3 color);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalSetTextComponent(uint entity, string text, string fileName, in int fontLayer, in float fontSize, in Vector3 color);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalGetAnimationComponent(uint entity, out int frameNumber, out int framesPerSecond, out float frameTimer, out bool isAnimating, out int stripcount);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalSetAnimationComponent(uint entity, in int frameNumber, in int framesPerSecond, in float frameTimer, in bool isAnimating, in int stripcount);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalGetSpriteComponent(uint entity, out string imageFile, out int layer, out Vector3 color, out float alpha);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalSetSpriteComponent(uint entity, string imageFile, in int layer, in Vector3 color, in float alpha);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalGetCameraComponent(uint entity, out float left, out float right, out float top, out float bottom, out float aspectRatio);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalSetCameraComponent(uint entity, in float left, in float right, in float top, in float bottom, in float aspectRatio);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalGetButtonComponent(uint entity, out Vector2 position, out Vector2 scale, out bool isClick);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalSetButtonComponent(uint entity, in Vector2 position, in Vector2 scale, bool isClick);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static string[] m_InternalGetScriptNames(uint entityID);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalAddScriptInstance(uint entityID, string scriptName, object instance);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalGetVelocity(uint entity, out Vector2 Velocity);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalSetVelocity(uint entity, in Vector2 Velocity);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static float m_InternalCallGetDeltaTime();

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static int m_InternalCallGetTagID(string tag);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static int[] m_InternalCallGetTagIDs(string tag);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static int[] m_InternalCallGetCollidedEntities(uint entity);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static string m_InternalCallGetTag(uint entity);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void m_InternalCallSetSceneActive(string sceneName);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void m_UnloadAllScene(); // ensure to follow up with a load scene, if no scene, engine will have undefine behaviour

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void m_InternalCallLoadScene(string path);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static int m_InternalCallAddPrefab(string prefab, in float x, in float y, in float degree);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void m_InternalCallDeleteEntity(uint id);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalCallIsKeyPressed(keyCode key);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static float m_InternalCallIsCollided(uint entity);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static Vector2 m_InternalGetMousePosition();

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalCallIsKeyReleased(keyCode key);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalCallIsKeyTriggered(keyCode key);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void m_InternalGetWorldMousePosition(out Vector2 mousepos);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void m_InternalCallSetTimeScale(in float timescale);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void m_InternalCallResetTimeScale();

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void m_InternalCallCloseWindow();

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static int[] m_InternalCallGetChildrenID(uint id);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void m_InternalCallPlayAudio(uint id, string monoString);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void m_InternalCallStopAudio(uint id, string monoString);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void m_InternalCallStopAllAudio();

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalCallIsWindowMinimise();

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static float m_getAccumulatedDeltaTime();

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static int m_InternalCallGetSteps();

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static float m_InternalCallGetGameTime();

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void m_EnableScript(uint id, string monostring);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void m_DisableScript(uint id, string monostring);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static int[] m_RetrieveCollidableEntities(string monostring); // insert scene name

    [MethodImpl(MethodImplOptions.InternalCall)]
    private extern static bool m_InternalCallGetRayCast(uint id, string monoString, out bool isRaycasting, out Vector2 targetposition, out float m_distance, out bool targetReached, out Vector2 hitposition);

    [MethodImpl(MethodImplOptions.InternalCall)]
    private extern static bool m_InternalCallSetRayCast(uint id, string monoString, in bool isRaycasting, in Vector2 targetposition, in float m_distance, in bool targetReached, in Vector2 hitposition);

    public static Raycast m_GetRay(uint id, string monoString)
    {
        Raycast ray = new Raycast();

        m_InternalCallGetRayCast(id, monoString, out ray.m_isRaycasting, out ray.m_targetposition, out ray.m_distance, out ray.m_targetReached, out ray.m_hitPosition);

        return ray;
    }

    public static void m_SetRay(uint id, string monoString, Raycast ray)
    {

        m_InternalCallSetRayCast(id, monoString, in ray.m_isRaycasting, in ray.m_targetposition, in ray.m_distance, in ray.m_targetReached, in ray.m_hitPosition);

    }

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalCallGetPathfinding(uint id, out Vector2 m_startpos, out Vector2 m_startend, out int gridkey, out int[] nodeArray_x, out int[] nodeArray_y);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void m_EnableLayer(uint layer);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void m_DisableLayer(uint layer);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void m_InternalCallSetTargetPathfinding(uint id, in Vector2 m_targetgridposition);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static float m_GetUnfixedDeltaTie();

}


public static class Component
{
    public static T Get<T>(uint id) where T : new()
    {
        T component = new T();

        if (typeof(T) == typeof(SpriteComponent))
        {
            var spriteComponent = component as SpriteComponent;
            InternalCall.m_InternalGetSpriteComponent(id, out spriteComponent.m_imageFile, out spriteComponent.m_layer, out spriteComponent.m_color, out spriteComponent.m_alpha);
        }
        else if (typeof(T) == typeof(TransformComponent))
        {
            var transformComponent = component as TransformComponent;
            InternalCall.m_InternalGetTransformComponent(id, out transformComponent.m_position, out transformComponent.m_scale, out transformComponent.m_rotation);
        }
        else if (typeof(T) == typeof(ColliderComponent))
        {
            var colliderComponent = component as ColliderComponent;
            InternalCall.m_InternalGetColliderComponent(id, out colliderComponent.m_Size, out colliderComponent.m_Offset, out colliderComponent.m_drawDebug, out colliderComponent.m_radius, out colliderComponent.m_blockedFlag, out colliderComponent.m_isCollided, out colliderComponent.m_collisionCheck);
        }
        else if (typeof(T) == typeof(TextComponent))
        {
            var textComponent = component as TextComponent;
            InternalCall.m_InternalGetTextComponent(id, out textComponent.m_text, out textComponent.m_fileName, out textComponent.m_fontLayer, out textComponent.m_fontSize, out textComponent.m_color);
        }
        else if (typeof(T) == typeof(RigidBodyComponent))
        {
            var rigidComponent = component as RigidBodyComponent;
            InternalCall.m_InternalGetRigidBodyComponent(id, out rigidComponent.m_Velocity, out rigidComponent.m_Acceleration, out rigidComponent.m_Rotation, out rigidComponent.m_prevPos, out rigidComponent.m_direction);
        }
        else if (typeof(T) == typeof(AnimationComponent))
        {
            var animationComponent = component as AnimationComponent;
            InternalCall.m_InternalGetAnimationComponent(id, out animationComponent.m_frameNumber, out animationComponent.m_framesPerSecond, out animationComponent.m_frameTimer, out animationComponent.m_isAnimating, out animationComponent.m_stripCount);
        }
        else if (typeof(T) == typeof(ButtomComponent))
        {
            var buttonComponent = component as ButtomComponent;
            InternalCall.m_InternalGetButtonComponent(id, out buttonComponent.m_Position, out buttonComponent.m_Scale, out buttonComponent.m_IsClick);
        }
        else if(typeof(T) == typeof(PathfindingComponent))
        {
            var pathfindingcomponent = component as PathfindingComponent;
            int[] x, y;
            InternalCall.m_InternalCallGetPathfinding(id, out pathfindingcomponent.m_startPosition, out pathfindingcomponent.m_targetPosition, out pathfindingcomponent.m_gridkey, out x, out y);

            if (x != null && y != null)
            {
                for (int n = 0; n < x.Length; n++)
                {
                    if (pathfindingcomponent.m_node == null)
                    {
                        pathfindingcomponent.m_node = new List<Vector2>();
                    }
                    pathfindingcomponent.m_node.Add(new Vector2(x[n], y[n]));
                }
            }
        }
        else if (typeof(T) == typeof(EnemyComponent))
        {
            var enemyComponent = component as EnemyComponent;
            InternalCall.m_InternalGetEnemyComponent(id, out enemyComponent.m_tag, out enemyComponent.m_enemyTypeInt, out enemyComponent.m_enemyRoamBehaviourInt);
        }
        else
        {
            throw new NotSupportedException($"Component type {typeof(T).Name} is not supported.");
        }

        return component;
    }

    public static void Set<T>(uint id, T component)
    {
        if (component is SpriteComponent sprite)
        {
            InternalCall.m_InternalSetSpriteComponent(id, sprite.m_imageFile, in sprite.m_layer, in sprite.m_color, in sprite.m_alpha);
        }
        else if (component is TransformComponent transform)
        {
            InternalCall.m_InternalSetTransformComponent(id, in transform.m_position, in transform.m_scale, in transform.m_rotation);
        }
        else if (component is ColliderComponent collider)
        {
            InternalCall.m_InternalSetColliderComponent(id, in collider.m_Size, in collider.m_Offset, in collider.m_drawDebug, in collider.m_radius, in collider.m_blockedFlag, in collider.m_isCollided, in collider.m_collisionCheck);
        }
        else if (component is TextComponent text)
        {
            InternalCall.m_InternalSetTextComponent(id, text.m_text, text.m_fileName, in text.m_fontLayer, in text.m_fontSize, in text.m_color);
        }
        else if (component is RigidBodyComponent rigid)
        {
            InternalCall.m_InternalSetRigidBodyComponent(id, in rigid.m_Velocity, in rigid.m_Acceleration, in rigid.m_Rotation, in rigid.m_prevPos, in rigid.m_direction);
        }
        else if (component is AnimationComponent animation)
        {
            InternalCall.m_InternalSetAnimationComponent(id, in animation.m_frameNumber, in animation.m_framesPerSecond, in animation.m_frameTimer, in animation.m_isAnimating, in animation.m_stripCount);
        }
        else if (component is EnemyComponent enemy)
        {
            InternalCall.m_InternalSetEnemyComponent(id, in enemy.m_tag, in enemy.m_enemyTypeInt, in enemy.m_enemyRoamBehaviourInt);
        }
        else
        {
            throw new NotSupportedException($"Component type {typeof(T).Name} is not supported.");
        }
    }
}

public static class GetComponent
{
    public static SpriteComponent GetSpriteComponent(uint id)
    {
        SpriteComponent temp = new SpriteComponent();
        InternalCall.m_InternalGetSpriteComponent(id, out temp.m_imageFile, out temp.m_layer, out temp.m_color, out temp.m_alpha);

        return temp;
    }

    public static TransformComponent GetTransformComponent(uint id)
    {
        TransformComponent temp = new TransformComponent();
        InternalCall.m_InternalGetTransformComponent(id, out temp.m_position, out temp.m_scale, out temp.m_rotation);

        return temp;
    }

    public static ColliderComponent GetColliderComponent(uint id)
    {
        ColliderComponent temp = new ColliderComponent();
        InternalCall.m_InternalGetColliderComponent(id, out temp.m_Size, out temp.m_Offset, out temp.m_drawDebug, out temp.m_radius, out temp.m_blockedFlag, out temp.m_isCollided, out temp.m_collisionCheck);
        return temp;
    }

    public static TextComponent GetTextComponent(uint id)
    {
        TextComponent temp = new TextComponent();
        InternalCall.m_InternalGetTextComponent(id, out temp.m_text, out temp.m_fileName, out temp.m_fontLayer, out temp.m_fontSize, out temp.m_color);
        return temp;
    }
}
public static class SetComponent
{
    public static void SetSpriteComponent(uint id, SpriteComponent sprite)
    {
        InternalCall.m_InternalSetSpriteComponent(id, sprite.m_imageFile, in sprite.m_layer, in sprite.m_color, in sprite.m_alpha);
    }

    public static void SetTransformComponent(uint id, TransformComponent transform)
    {
        InternalCall.m_InternalSetTransformComponent(id, in transform.m_position, in transform.m_scale, in transform.m_rotation);
    }

    public static void SetCollisionComponent(uint id, ColliderComponent transform)
    {
        InternalCall.m_InternalSetColliderComponent(id, in transform.m_Size, in transform.m_Offset, in transform.m_drawDebug, in transform.m_radius, in transform.m_blockedFlag, in transform.m_isCollided, in transform.m_collisionCheck);
    }

    public static void SetTextComponent(uint id, TextComponent text)
    {
        InternalCall.m_InternalSetTextComponent(id, text.m_text, text.m_fileName, in text.m_fontLayer, in text.m_fontSize, in text.m_color);
    }
}
