namespace TurnipEngine
{
    public static class Input 
    {
        [System.Runtime.InteropServices.DllImport("__Internal")]
        public static extern bool IsKeyPressed(int key);
    }
}