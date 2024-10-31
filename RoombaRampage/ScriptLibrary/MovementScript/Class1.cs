using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Namespace
{
    public class MovementScript : ScriptBase
    {
        public int EntityID { get; set; }

        public override void Start()
        {

        }

        public override void Update()
        {
            EntityID = 1;

            // int firstEntityID = GetFirstEntityID();
            // Console.WriteLine("First Entity ID in C#: " + firstEntityID);
            Vector2 translate, scale;
            float rotate;
            m_InternalGetTransformComponent(0, out translate, out scale, out rotate);  // Corrected typo hereX: -1.18418E+22, Y: 4.590794E-41
            // Ensure position is checked, not `transform`

            Console.WriteLine($"Transform Position - X: {translate.X}, Y: {translate.Y}");
            Console.WriteLine($"Transform Scale - X: {scale.X}, Y: {scale.Y}");
            Console.WriteLine($"Transform Rotate - {rotate}");
            Console.WriteLine("id1234" + EntityID);
        }
    }
}