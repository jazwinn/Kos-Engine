//Tracks whether the AI has detected the player and the range at which it can detect them

public class PlayerDetectionComponent {
    public bool IsPlayerDetected = false;
    public float DetectionRadius;

    public PlayerDetectionComponent(float detectionRadius) {
        DetectionRadius = detectionRadius;
    }
}

