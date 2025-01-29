using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Policy;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Xml;

public class testscript : ScriptBase
{

    private uint EntityID;

    public int test = 0;

    public float test2 = 0;

    public bool test3 = true;

    public string test4 = "hello";

    public override void Awake(uint id)
    {
        EntityID = id;

    }


    public override void Start()
    {
       
    }

    public override void Update()
    {
        Console.WriteLine(test);
       
    }
}