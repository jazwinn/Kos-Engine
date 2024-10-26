namespace Namespace
{
    public class ExampleScriptA
    {
        //MUST ALWAYS USE STATIC VOID! IF NOT THEY CANT DETECT!

        public static void PrintA()
        {
            System.Console.WriteLine("C# is calling method 1 from SCRIPT A");
        }

        public static void PrintMessage(int number, string message)
        {
            System.Console.WriteLine($"Received number: {number}, message: {message}");
        }
    }
}