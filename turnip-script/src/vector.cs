using System.Runtime.InteropServices;

namespace TurnipScript
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Vector2D
    {
        public float x;
        public float y;

        public Vector2D(float x, float y)
        {
            this.x = x;
            this.y = y;
        }
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct Vector3D
    {
        public float x;
        public float y;
        public float z;

        public Vector3D(float x, float y, float z)
        {
            this.x = x;
            this.y = y;
            this.z = z;
        }
    }
}
