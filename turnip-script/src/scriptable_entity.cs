using System;
using System.Runtime.CompilerServices;

namespace TurnipScript
{
    public class ScriptableEntity
    {
        public readonly uint _id;

        protected ScriptableEntity() { _id = 0; }
        internal ScriptableEntity(uint id) { _id = id; }
        
        // TODO: EntityInstantiated, EntityDestroyed, SceneStart, SceneEnd, etc
        public virtual void OnUpdate()  { }

        protected T GetComponent<T>() where T : Component
        {
            return Internal.GetComponent_Internal<T>(_id);
        }
    }
}
