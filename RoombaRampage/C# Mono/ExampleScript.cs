public class ExampleScript
{
    //MUST ALWAYS USE STATIC VOID! IF NOT THEY CANT DETECT!

    public static void HelloWorld()
    {
        System.Console.WriteLine("C# is calling this method!!");
    }

    public static void PrintMessage(int number, string message)
    {
        System.Console.WriteLine($"Received number: {number}, message: {message}");
    }
}
