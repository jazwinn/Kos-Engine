using System;

namespace Namespace
{
    public class ExampleScriptB : ScriptBase
    {
        public int EntityID { get; set; }

        public override void Start()
        {
            base.Start();

            TransformComponent transform = GetTransformComponent();
            if (transform != null)
            {
                Console.WriteLine($"Transform Position - X: {transform.m_position.x}, Y: {transform.m_position.y}");
            }
            else
            {
                Console.WriteLine("TransformComponent is null.");
            }
        }

        public override void Update()
        {
            base.Update();
        }
    }
}
