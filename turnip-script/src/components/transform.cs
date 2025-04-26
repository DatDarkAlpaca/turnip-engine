using System.Runtime.InteropServices;

namespace TurnipScript
{
    public class Transform : Component
    {
        internal Transform(uint entityID) 
            : base(entityID) 
        {
        }

        public Vector3D position
        {
            get
            {
                Internal.GetTransformPosition_Native(_entityID, out Vector3D pos);
                return pos;
            }
            set
            {
                Internal.SetTransformPosition_Native(_entityID, ref value);
            }
        }

        public Vector3D rotation;
        public Vector3D scale;
    }
}
