using System;
using System.Reflection;
using System.Runtime.CompilerServices;

namespace TurnipScript
{
    internal class Internal
    {
        #region Reflection
        public static IntPtr MakeGenericMethod(MethodInfo methodInfo, Type type)
        {
            return methodInfo.MakeGenericMethod(type).MethodHandle.Value;
        }
        #endregion

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
        public static extern Component GetComponent_InternalNative(uint entityID, Type type);

        public static T GetComponent_Internal<T>(uint entityID) where T : Component
        {
            return (T)GetComponent_InternalNative(entityID, typeof(T));
        }
        #endregion
    }
}
