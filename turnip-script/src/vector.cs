using System;
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

        public static Vector2D operator+ (Vector2D vector) => vector;
        public static Vector2D operator- (Vector2D vector) => new Vector2D(-vector.x, -vector.y);

        public static Vector2D operator+ (Vector2D lhs, Vector2D rhs) => new Vector2D(lhs.x + rhs.x, lhs.y + rhs.y);
        public static Vector2D operator- (Vector2D lhs, Vector2D rhs) => lhs + (-rhs);

        public static Vector2D operator* (Vector2D lhs, float value) => new Vector2D(lhs.x * value, lhs.y * value);

        public static float magnitude(Vector2D vector) => (float)Math.Sqrt(vector.x * vector.x + vector.y * vector.y);
        public static float dot(Vector2D lhs, Vector2D rhs) => lhs.x * rhs.x + lhs.y * rhs.y;
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct Vector3D
    {
        public float x, y, z;

        public Vector3D(float x, float y, float z)
        {
            this.x = x;
            this.y = y;
            this.z = z;
        }

        public static Vector3D operator+ (Vector3D vector) => vector;
        public static Vector3D operator- (Vector3D vector) => new Vector3D(-vector.x, -vector.y, -vector.z);

        public static Vector3D operator+ (Vector3D lhs, Vector3D rhs) => new Vector3D(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
        public static Vector3D operator- (Vector3D lhs, Vector3D rhs) => lhs + (-rhs);

        public static Vector3D operator* (Vector3D lhs, float value) => new Vector3D(lhs.x * value, lhs.y * value, lhs.z * value);

        public static float magnitude(Vector3D vector)
        {
            return (float)Math.Sqrt(
                vector.x * vector.x + 
                vector.y * vector.y + 
                vector.z * vector.z
            );
        }
        public static float dot(Vector3D lhs, Vector3D rhs) => lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
        public static Vector3D cross(Vector3D lhs, Vector3D rhs) 
        {
            return new Vector3D(
                lhs.y * rhs.z - lhs.z * rhs.y,
                lhs.z * rhs.x - lhs.x * rhs.z,
                lhs.x * rhs.y - lhs.y * rhs.x
            );
        }
    }
}