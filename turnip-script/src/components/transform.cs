using System.Runtime.InteropServices;

namespace TurnipScript
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Transform : Component
    {
        public Vector3D position;
        public Vector3D rotation;
        public Vector3D scale;
    }
}
