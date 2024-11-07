public class AStarPathfinder {
    private Grid grid;  // Assume a grid-based map using the grid and node class ive implemented

    public AStarPathfinder(Grid grid) {
        this.grid = grid;
    }

    public List<Node> FindPath(Vector2 start, Vector2 target) {
        var openSet = new List<Node>();
        var closedSet = new HashSet<Node>();

        var startNode = grid.GetNode(start);
        var targetNode = grid.GetNode(target);
        
        openSet.Add(startNode);

        while (openSet.Count > 0) {
            Node currentNode = openSet.OrderBy(node => node.FCost).ThenBy(node => node.HCost).First();
            openSet.Remove(currentNode);
            closedSet.Add(currentNode);

            if (currentNode == targetNode) {
                return RetracePath(startNode, targetNode);
            }

            foreach (var neighbor in grid.GetNeighbors(currentNode)) {
                if (closedSet.Contains(neighbor) || !neighbor.IsWalkable) continue;

                int newCostToNeighbor = currentNode.GCost + GetDistance(currentNode, neighbor);
                if (newCostToNeighbor < neighbor.GCost || !openSet.Contains(neighbor)) {
                    neighbor.GCost = newCostToNeighbor;
                    neighbor.HCost = GetDistance(neighbor, targetNode);
                    neighbor.Parent = currentNode;

                    if (!openSet.Contains(neighbor)) openSet.Add(neighbor);
                }
            }
        }
        return null;
    }

    private List<Node> RetracePath(Node startNode, Node endNode) {
        var path = new List<Node>();
        var currentNode = endNode;

        while (currentNode != startNode) {
            path.Add(currentNode);
            currentNode = currentNode.Parent;
        }
        path.Reverse();
        return path;
    }

    private int GetDistance(Node a, Node b) {
        int dstX = Math.Abs(a.GridX - b.GridX);
        int dstY = Math.Abs(a.GridY - b.GridY);
        return dstX > dstY ? 14 * dstY + 10 * (dstX - dstY) : 14 * dstX + 10 * (dstY - dstX);
    }
}
