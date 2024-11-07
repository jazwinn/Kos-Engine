//This component stores the position of the entity
    public enum AIState {
        Idle,
        Chase,
        Attack,
        Flee,
        Dead,
        // Boss-specific states
        BossPhase1,
        BossPhase2,
        BossEnraged
    }

public class EnemyComponent {
    public int Health;
    public int MaxHealth;
    public bool IsPlayerDetected = false;
    public float DetectionRadius;
    public float StateTimer = 0.0f;
    public AIState CurrentState = AIState.Idle;
}

public class PlayerDetectionComponent {
    public bool IsPlayerDetected = false;
    public float DetectionRadius;

    public PlayerDetectionComponent(float detectionRadius) {
        DetectionRadius = detectionRadius;
    }
}
