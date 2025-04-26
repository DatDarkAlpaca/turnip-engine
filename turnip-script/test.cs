using System;
using TurnipScript;

namespace TurnipScript
{
    public class Test : ScriptableEntity
    {
        Transform transform;

        public Test()
        {
            Internal.LogInfo(String.Format("Test() constructor invoked: EID: {0}", _id));
        }

        public override void OnUpdate()
        {
            Internal.LogInfo(GetComponent<Transform>().position.x.ToString());
        }
    }
}
