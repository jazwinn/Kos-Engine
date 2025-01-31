using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using System.Reflection.Emit;
using System.Data.SqlClient;
using System.Runtime.InteropServices;

public static class InternalCall
{

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalGetTransformComponent(uint entity, ref Vector2 pos, ref Vector2 scale, ref float rotate);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalSetTransformComponent(uint entity, ref Vector2 pos, ref Vector2 scale, ref float rotate);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalGetTranslate(uint entity, ref Vector2 pos);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalSetTranslate(uint entity, ref Vector2 pos);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalGetColliderComponent(uint entity, ref Vector2 size, ref Vector2 offset, ref bool drawDebug, ref float radius, ref uint bockflag, ref float isCollided, ref bool collisionCheck, ref bool collisionresponse);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalSetColliderComponent(uint entity, ref Vector2 size, ref Vector2 offset, ref bool drawDebug, ref float radius, ref uint bockflag, ref float isCollided, ref bool collisionCheck, ref bool collisionresponse);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalGetEnemyComponent(uint entity, ref int enemyTag, ref int enemytype, ref int enemybehaviour);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalSetEnemyComponent(uint entity, ref int enemyTag, ref int enemytype, ref int enemybehaviour);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalGetRigidBodyComponent(uint entity, ref Vector2 velocity, ref Vector2 acceleration, ref float rotation, ref Vector2 previouspos, ref Vector2 direction);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalSetRigidBodyComponent(uint entity, ref Vector2 velocity, ref Vector2 acceleration, ref float rotation, ref Vector2 previouspos, ref Vector2 direction);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalGetTextComponent(uint entity, ref string text, ref string fileName, ref int fontLayer, ref float fontSize, ref Vector3 color);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalSetTextComponent(uint entity, string text, string fileName, ref int fontLayer, ref float fontSize, ref Vector3 color);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalGetAnimationComponent(uint entity, ref int frameNumber, ref int framesPerSecond, ref float frameTimer, ref bool isAnimating, ref int stripcount);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalSetAnimationComponent(uint entity, ref int frameNumber, ref int framesPerSecond, ref float frameTimer, ref bool isAnimating, ref int stripcount);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalGetSpriteComponent(uint entity, ref string imageFile, ref int layer, ref Vector3 color, ref float alpha);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalSetSpriteComponent(uint entity, string imageFile, ref int layer, ref Vector3 color, ref float alpha);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalGetCameraComponent(uint entity, ref float left, ref float right, ref float top, ref float bottom, ref float aspectRatio);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalSetCameraComponent(uint entity, ref float left, ref float right, ref float top, ref float bottom, ref float aspectRatio);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalGetButtonComponent(uint entity, ref Vector2 position, ref Vector2 scale, ref bool isClick);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalSetButtonComponent(uint entity, ref Vector2 position, ref Vector2 scale, bool isClick);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static string[] m_InternalGetScriptNames(uint entityID);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalAddScriptInstance(uint entityID, string scriptName, object instance);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalGetVelocity(uint entity, ref Vector2 Velocity);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalSetVelocity(uint entity, ref Vector2 Velocity);

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
    public extern static int m_InternalCallAddPrefab(string prefab, ref float x, ref float y, ref float degree);

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
    public extern static void m_InternalGetWorldMousePosition(ref Vector2 mousepos);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void m_InternalCallSetTimeScale(ref float timescale);

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
    private extern static bool m_InternalCallGetRayCast(uint id, string monoString, ref bool isRaycasting, ref Vector2 targetposition, ref float m_distance, ref bool targetReached, ref Vector2 hitposition);

    [MethodImpl(MethodImplOptions.InternalCall)]
    private extern static bool m_InternalCallSetRayCast(uint id, string monoString, ref bool isRaycasting, ref Vector2 targetposition, ref float m_distance, ref bool targetReached, ref Vector2 hitposition);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalGetGridComponent(uint id, ref Vector2 anchor, ref int gridRowLength, ref int gridColumnLength, ref bool setCollidable, ref int gridKey);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalSetGridComponent(uint id, ref Vector2 anchor, ref int gridRowLength, ref int gridColumnLength, ref bool setCollidable, ref int gridKey);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static int m_InternalGetEntityIdFromGridKey(int gridkey);
    public static Raycast m_GetRay(uint id, string monoString)
    {
        Raycast ray = new Raycast();

        m_InternalCallGetRayCast(id, monoString, ref ray.m_isRaycasting, ref ray.m_targetposition, ref ray.m_distance, ref ray.m_targetReached, ref ray.m_hitPosition);

        return ray;
    }

    public static void m_SetRay(uint id, string monoString, Raycast ray)
    {

        m_InternalCallSetRayCast(id, monoString, ref ray.m_isRaycasting, ref ray.m_targetposition, ref ray.m_distance, ref ray.m_targetReached, ref ray.m_hitPosition);

    }

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalCallGetPathfinding(uint id, ref Vector2 m_startpos, ref Vector2 m_startend, ref int gridkey, ref int[] nodeArray_x, ref int[] nodeArray_y);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void m_EnableLayer(uint layer);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void m_DisableLayer(uint layer);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void m_InternalCallSetTargetPathfinding(uint id, ref Vector2 m_targetgridposition);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static float m_GetUnfixedDeltaTie();

    [MethodImpl(MethodImplOptions.InternalCall)]
    public static extern void m_GetNameComponent(uint id, ref IntPtr refPtr);

}


public static class Component
{
    public static T Get<T>(uint id) where T : new()
    {
        T component = new T();

        if (typeof(T) == typeof(SpriteComponent))
        {
            var spriteComponent = component as SpriteComponent;
            InternalCall.m_InternalGetSpriteComponent(id, ref spriteComponent.m_imageFile, ref spriteComponent.m_layer, ref spriteComponent.m_color, ref spriteComponent.m_alpha);
        }
        else if (typeof(T) == typeof(TransformComponent))
        {
            var transformComponent = component as TransformComponent;
            InternalCall.m_InternalGetTransformComponent(id, ref transformComponent.m_position, ref transformComponent.m_scale, ref transformComponent.m_rotation);
        }
        else if (typeof(T) == typeof(ColliderComponent))
        {
            var colliderComponent = component as ColliderComponent;
            InternalCall.m_InternalGetColliderComponent(id, ref colliderComponent.m_Size, ref colliderComponent.m_Offset, ref colliderComponent.m_drawDebug, ref colliderComponent.m_radius, ref colliderComponent.m_blockedFlag, ref colliderComponent.m_isCollided, ref colliderComponent.m_collisionCheck, ref colliderComponent.m_collisionResponse);
        }
        else if (typeof(T) == typeof(TextComponent))
        {
            var textComponent = component as TextComponent;
            InternalCall.m_InternalGetTextComponent(id, ref textComponent.m_text, ref textComponent.m_fileName, ref textComponent.m_fontLayer, ref textComponent.m_fontSize, ref textComponent.m_color);
        }
        else if (typeof(T) == typeof(RigidBodyComponent))
        {
            var rigidComponent = component as RigidBodyComponent;
            InternalCall.m_InternalGetRigidBodyComponent(id, ref rigidComponent.m_Velocity, ref rigidComponent.m_Acceleration, ref rigidComponent.m_Rotation, ref rigidComponent.m_prevPos, ref rigidComponent.m_direction);
        }
        else if (typeof(T) == typeof(AnimationComponent))
        {
            var animationComponent = component as AnimationComponent;
            InternalCall.m_InternalGetAnimationComponent(id, ref animationComponent.m_frameNumber, ref animationComponent.m_framesPerSecond, ref animationComponent.m_frameTimer, ref animationComponent.m_isAnimating, ref animationComponent.m_stripCount);
        }
        else if (typeof(T) == typeof(ButtomComponent))
        {
            var buttonComponent = component as ButtomComponent;
            InternalCall.m_InternalGetButtonComponent(id, ref buttonComponent.m_Position, ref buttonComponent.m_Scale, ref buttonComponent.m_IsClick);
        }
        else if(typeof(T) == typeof(PathfindingComponent))
        {
            var pathfindingcomponent = component as PathfindingComponent;
            int[] x = null, y = null;
            InternalCall.m_InternalCallGetPathfinding(id, ref pathfindingcomponent.m_startPosition, ref pathfindingcomponent.m_targetPosition, ref pathfindingcomponent.m_gridkey, ref x, ref y);

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
            InternalCall.m_InternalGetEnemyComponent(id, ref enemyComponent.m_tag, ref enemyComponent.m_enemyTypeInt, ref enemyComponent.m_enemyRoamBehaviourInt);
        }
        else if(typeof(T) == typeof(NameComponent))
        {
            //IntPtr ptr = null;
            //InternalCall.m_GetNameComponent(id, ref ptr); // Get pointer from C++

            //if (ptr == IntPtr.Zero)
            //    return default; // Handle null case safely

            //// Marshal memory into an existing instance of NameComponent
            //component = (T)(object)Marshal.PtrToStructure<NameComponent>(ptr);
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
            InternalCall.m_InternalSetSpriteComponent(id, sprite.m_imageFile, ref sprite.m_layer, ref sprite.m_color, ref sprite.m_alpha);
        }
        else if (component is TransformComponent transform)
        {
            InternalCall.m_InternalSetTransformComponent(id, ref transform.m_position, ref transform.m_scale, ref transform.m_rotation);
        }
        else if (component is ColliderComponent collider)
        {
            InternalCall.m_InternalSetColliderComponent(id, ref collider.m_Size, ref collider.m_Offset, ref collider.m_drawDebug, ref collider.m_radius, ref collider.m_blockedFlag, ref collider.m_isCollided, ref collider.m_collisionCheck, ref collider.m_collisionResponse);
        }
        else if (component is TextComponent text)
        {
            InternalCall.m_InternalSetTextComponent(id, text.m_text, text.m_fileName, ref text.m_fontLayer, ref text.m_fontSize, ref text.m_color);
        }
        else if (component is RigidBodyComponent rigid)
        {
            InternalCall.m_InternalSetRigidBodyComponent(id, ref rigid.m_Velocity, ref rigid.m_Acceleration, ref rigid.m_Rotation, ref rigid.m_prevPos, ref rigid.m_direction);
        }
        else if (component is AnimationComponent animation)
        {
            InternalCall.m_InternalSetAnimationComponent(id, ref animation.m_frameNumber, ref animation.m_framesPerSecond, ref animation.m_frameTimer, ref animation.m_isAnimating, ref animation.m_stripCount);
        }
        else if (component is EnemyComponent enemy)
        {
            InternalCall.m_InternalSetEnemyComponent(id, ref enemy.m_tag, ref enemy.m_enemyTypeInt, ref enemy.m_enemyRoamBehaviourInt);
        }
        else if (component is GridComponent grid)
        {
            InternalCall.m_InternalSetGridComponent(id, ref grid.m_Anchor, ref grid.m_GridRowLength, ref grid.m_GridColumnLength, ref grid.m_SetCollidable, ref grid.m_GridKey);
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
        InternalCall.m_InternalGetSpriteComponent(id, ref temp.m_imageFile, ref temp.m_layer, ref temp.m_color, ref temp.m_alpha);

        return temp;
    }

    public static TransformComponent GetTransformComponent(uint id)
    {
        TransformComponent temp = new TransformComponent();
        InternalCall.m_InternalGetTransformComponent(id, ref temp.m_position, ref temp.m_scale, ref temp.m_rotation);

        return temp;
    }

    public static ColliderComponent GetColliderComponent(uint id)
    {
        ColliderComponent temp = new ColliderComponent();
        InternalCall.m_InternalGetColliderComponent(id, ref temp.m_Size, ref temp.m_Offset, ref temp.m_drawDebug, ref temp.m_radius, ref temp.m_blockedFlag, ref temp.m_isCollided, ref temp.m_collisionCheck, ref temp.m_collisionResponse );
        return temp;
    }

    public static TextComponent GetTextComponent(uint id)
    {
        TextComponent temp = new TextComponent();
        InternalCall.m_InternalGetTextComponent(id, ref temp.m_text, ref temp.m_fileName, ref temp.m_fontLayer, ref temp.m_fontSize, ref temp.m_color);
        return temp;
    }
}
public static class SetComponent
{
    public static void SetSpriteComponent(uint id, SpriteComponent sprite)
    {
        InternalCall.m_InternalSetSpriteComponent(id, sprite.m_imageFile, ref sprite.m_layer, ref sprite.m_color, ref sprite.m_alpha);
    }

    public static void SetTransformComponent(uint id, TransformComponent transform)
    {
        InternalCall.m_InternalSetTransformComponent(id, ref transform.m_position, ref transform.m_scale, ref transform.m_rotation);
    }

    public static void SetCollisionComponent(uint id, ColliderComponent transform)
    {
        InternalCall.m_InternalSetColliderComponent(id, ref transform.m_Size, ref transform.m_Offset, ref transform.m_drawDebug, ref transform.m_radius, ref transform.m_blockedFlag, ref transform.m_isCollided, ref transform.m_collisionCheck, ref transform.m_collisionResponse);
    }

    public static void SetTextComponent(uint id, TextComponent text)
    {
        InternalCall.m_InternalSetTextComponent(id, text.m_text, text.m_fileName, ref text.m_fontLayer, ref text.m_fontSize, ref text.m_color);
    }
}
