public class Grid {
    private int gridSizeX, gridSizeY;
    private float nodeSize;
    private Node[,] nodes;

    public Grid(int width, int height, float nodeSize) {
        this.gridSizeX = width;
        this.gridSizeY = height;
        this.nodeSize = nodeSize;
        CreateGrid();
    }

    private void CreateGrid() {
        nodes = new Node[gridSizeX, gridSizeY];

        for (int x = 0; x < gridSizeX; x++) {
            for (int y = 0; y < gridSizeY; y++) {
                Vector2 worldPosition = new Vector2(x * nodeSize, y * nodeSize);
                nodes[x, y] = new Node(worldPosition, true, x, y); // This one sets all grid nodes to be walkable
            }
        }
    }

    public Node GetNode(Vector2 worldPosition) {
        int x = Mathf.Clamp((int)(worldPosition.x / nodeSize), 0, gridSizeX - 1);
        int y = Mathf.Clamp((int)(worldPosition.y / nodeSize), 0, gridSizeY - 1);
        return nodes[x, y];
    }

    public List<Node> GetNeighbors(Node node) {
        var neighbors = new List<Node>();

        int[,] directions = {
            {0, 1},  // Up
            {1, 0},  // Right
            {0, -1}, // Down
            {-1, 0}  // Left
        }

        foreach (var dir in directions){
            int checkX = node.GridX + dir[0];
            int checkY = node.GridY + dir[1];

            if(checkX >= 0 && checkX < gridSizeX && checkY >= 0 && checkY < gridSizeY){
                neighbors.Add{nodes[checkX,checkY]};
            }
        }

        return neighbors;
    }

    public void SetWalkable(int x, int y, bool isWalkable) {
        if (x >= 0 && x < gridSizeX && y >= 0 && y < gridSizeY) {
            nodes[x, y].IsWalkable = isWalkable;
        }
    }
}
