namespace TurnipScript
{
    public class ScriptableEntity
    {
        public readonly uint _id;

        protected ScriptableEntity() { _id = 0; }
        internal ScriptableEntity(uint id) { _id = id; }

        #region Script Methods
        public virtual void OnSceneStart() { }
        public virtual void OnSceneEnd() { }

        public virtual void OnEntityInstanceCreated() { }
        public virtual void OnEntityInstanceDestroyed() { }

        public virtual void OnUpdate()  { }
        #endregion

        protected T GetComponent<T>() where T : Component
        {
            return Internal.GetComponent_Internal<T>(_id);
        }
    }
}
