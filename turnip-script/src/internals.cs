using System;
using System.Runtime.CompilerServices;

namespace TurnipScript
{
    public class Log
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void Info(string message);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void Trace(string message);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void Debug(string message);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void Warn(string message);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void Error(string message);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void Critical(string message);
    }

    internal class Internal
    {
        #region Components
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern Component GetComponent_Native(uint entityID, Type type);

        public static T GetComponent_Internal<T>(uint entityID) where T : Component
        {
            return (T)GetComponent_Native(entityID, typeof(T));
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void GetTransformPosition_Native(uint entityID, out Vector3D position);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void SetTransformPosition_Native(uint entityID, ref Vector3D position);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void GetTransformRotation_Native(uint entityID, out Vector3D rotation);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void SetTransformRotation_Native(uint entityID, ref Vector3D rotation);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void GetTransformScale_Native(uint entityID, out Vector3D scale);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void SetTransformScale_Native(uint entityID, ref Vector3D scale);
        #endregion
    }

    public class Input
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern Vector2D GetMousePositionWindow();
    }
}
