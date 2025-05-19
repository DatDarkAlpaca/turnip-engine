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

        public Vector3D rotation
        {
            get
            {
                Internal.GetTransformRotation_Native(_entityID, out Vector3D rot);
                return rot;
            }
            set
            {
                Internal.SetTransformRotation_Native(_entityID, ref value);
            }
        }

        public Vector3D scale
        {
            get
            {
                Internal.GetTransformScale_Native(_entityID, out Vector3D sca);
                return sca;
            }
            set
            {
                Internal.SetTransformScale_Native(_entityID, ref value);
            }
        }
    }
}
