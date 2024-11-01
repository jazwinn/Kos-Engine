using Namespace;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AiScript
{
    public class AiScript : ScriptBase
    {
        public override void Start()
        {

        }

        public override void Update()
        {
            Vector2 postion, scale;
            float rotate;
            m_InternalGetTransformComponent(0, out postion, out scale, out rotate);
        }
    }
}
