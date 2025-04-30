using System;
using System.Reflection;
using System.Runtime.CompilerServices;

namespace TurnipScript
{
    internal class Internal
    {
        #region Logging
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void LogInfo(string message);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void LogTrace(string message);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void LogDebug(string message);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void LogWarn(string message);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void LogError(string message);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void LogCritical(string message);
        #endregion

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
}
