using System;

namespace TurnipScript
{
    public abstract class Component
    {
        protected uint _entityID;

        internal Component(uint entityID)
        {
            _entityID = entityID;
        }

        public uint ID => _entityID;
    }
}
