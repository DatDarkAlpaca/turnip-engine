using System;

namespace TurnipScript
{
    public struct Vector2D
    {
        public float x;
        public float y;

        public Vector2D(float value)
        {
            x = y = value;
        }
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

        public static float sqrtMagnitude(Vector2D vector) => vector.x * vector.x + vector.y * vector.y;
        public static float magnitude(Vector2D vector) => (float)Math.Sqrt(vector.x * vector.x + vector.y * vector.y);
        public static float dot(Vector2D lhs, Vector2D rhs) => lhs.x * rhs.x + lhs.y * rhs.y;

        public static Vector2D Zero => new Vector2D(0.0f);
    }

    public struct Vector3D
    {
        public float x, y, z;

        public Vector3D(float value)
        {
            x = y = z = value;
        }
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

        public static float sqrtMagnitude(Vector3D vector)
        {
            return vector.x * vector.x + vector.y * vector.y + vector.z * vector.z;
        }
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

        public static Vector3D Zero => new Vector3D(0.0f);
    }

    public struct Vector4D
    {
        public float x, y, z, w;

        public Vector4D(float value)
        {
            x = y = z = w = value;
        }
        public Vector4D(float x, float y, float z, float w)
        {
            this.x = x;
            this.y = y;
            this.z = z;
            this.w = w;
        }

        public static Vector4D operator +(Vector4D vector) => vector;
        public static Vector4D operator -(Vector4D vector) => new Vector4D(-vector.x, -vector.y, -vector.z, -vector.w);

        public static Vector4D operator +(Vector4D lhs, Vector4D rhs) => new Vector4D(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
        public static Vector4D operator -(Vector4D lhs, Vector4D rhs) => lhs + (-rhs);
        public static Vector4D operator *(Vector4D lhs, float value) => new Vector4D(lhs.x * value, lhs.y * value, lhs.z * value, lhs.w * value);

        public static float sqrtMagnitude(Vector4D vector)
        {
            return vector.x * vector.x + vector.y * vector.y + vector.z * vector.z + vector.w * vector.w;
        }
        public static float magnitude(Vector4D vector)
        {
            return (float)Math.Sqrt(
                vector.x * vector.x +
                vector.y * vector.y +
                vector.z * vector.z +
                vector.w * vector.w
            );
        }
        public static float dot(Vector4D lhs, Vector4D rhs) => lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;       

        public static Vector4D Zero => new Vector4D(0.0f);
    }
}