public class Node {
    public Vector2 Position { get; private set; }
    public bool IsWalkable { get; set; }
    public int GCost { get; set; }
    public int HCost { get; set; }
    public Node Parent { get; set; }
    public int GridX { get; private set; }
    public int GridY { get; private set; }

    public int FCost => GCost + HCost;

    public Node(Vector2 position, bool isWalkable, int gridX, int gridY) {
        Position = position;
        IsWalkable = isWalkable;
        GridX = gridX;
        GridY = gridY;
    }
}
