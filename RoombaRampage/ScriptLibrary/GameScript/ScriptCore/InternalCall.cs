

using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using System.Reflection.Emit;

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
    public extern static bool m_InternalGetColliderComponent(uint entity, out Vector2 size, out Vector2 offset, out bool drawDebug, out float radius, out bool isCollided);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalSetColliderComponent(uint entity, in Vector2 size, in Vector2 offset, in bool drawDebug, in float radius, in bool isCollided);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalGetPlayerComponent(uint entity, out bool control);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalSetPlayerComponent(uint entity, in bool control);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalGetRigidBodyComponent(uint entity, out Vector2 velocity, out Vector2 acceleration, out float rotation);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalSetRigidBodyComponent(uint entity, in Vector2 velocity, in Vector2 acceleration, in float rotation);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalGetTextComponent(uint entity,out string text,out string fileName, out int fontLayer,out float fontSize,out Vector3 color );

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalSetTextComponent(uint entity, in string text, in string fileName, in int fontLayer, in float fontSize, in Vector3 color);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalGetAnimationComponent(uint entity, out int frameNumber, out int framesPerSecond, out float frameTimer, out bool isAnimating);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalSetAnimationComponent(uint entity, in int frameNumber, in int framesPerSecond, in float frameTimer, in bool isAnimating);

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
    public extern static bool m_InternalGetScriptNames(uint entityID, out IntPtr scriptArray);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalAddScriptInstance(uint entity, string scriptName, object instance);
    
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
}
