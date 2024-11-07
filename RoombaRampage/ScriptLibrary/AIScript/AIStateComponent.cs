//This component will store the current state of the AI entity (Enemy)

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

public class AIStateComponent {
    public AIState CurrentState = AIState.Idle;
    public float StateTimer = 0.0f;  // Tracks time spent in the current state
}
