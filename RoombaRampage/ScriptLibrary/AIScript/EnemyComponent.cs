//This component stores the position of the entity

public class PositionComponent {
    public float X, Y;

    public PositionComponent(float x, float y) {
        X = x;
        Y = y;
    }

    // This function will help to calculate distance
    public float DistanceTo(PositionComponent other) {
        return MathF.Sqrt(MathF.Pow(X - other.X, 2) + MathF.Pow(Y - other.Y, 2));
    }
}
