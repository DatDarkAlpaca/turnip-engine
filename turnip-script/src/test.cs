using System;
using TurnipScript;

namespace TurnipScript
{
    public class Test : ScriptableEntity
    {
        private Transform transform;

        public Test()
        {
            transform = GetComponent<Transform>();
        }

        public override void OnUpdate()
        {
            transform.position += new Vector3D(0.1f, 0.0f, 0.0f);
        }
    }
}
