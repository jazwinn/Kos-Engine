//This system handles AI state transitions and behavior based on game logic

public class AIStateSystem {
    private Player player;                      //This one is player's position reference
    private AStarPathfinder pathfinder;
    private List<Vector2> currentPath;
    private int currentPathIndex;

    public AIStateSystem(Player player, Grid grid) {
        this.player = player;
        this.pathfinder = new AStarPathfinder(grid);
        this.currentPath = new List<Vector2>();
        this.currentPathIndex = 0;
    }

    public void Update(float deltaTime, Entity entity) {
        var aiState = entity.GetComponent<AIStateComponent>();
        var health = entity.GetComponent<HealthComponent>();
        var position = entity.GetComponent<PositionComponent>();
        var detection = entity.GetComponent<PlayerDetectionComponent>();

        // Update state timer
        aiState.StateTimer += deltaTime;

        switch (aiState.CurrentState) {
            case AIState.Idle:
                HandleIdleState(entity, detection, position);
                break;
            case AIState.Chase:
                HandleChaseState(entity, position);
                break;
            case AIState.Attack:
                HandleAttackState(entity, position);
                break;
            case AIState.Flee:
                HandleFleeState(entity, position);
                break;
            case AIState.Dead:
                HandleDeadState(entity);
                break;
            case AIState.BossPhase1:
                HandleBossPhase1State(entity);
                break;
            case AIState.BossPhase2:
                HandleBossPhase2State(entity);
                break;
            case AIState.BossEnraged:
                HandleBossEnragedState(entity);
                break;
        }
    }

    private void HandleIdleState(Entity entity, PlayerDetectionComponent detection, PositionComponent position) {
        if (IsPlayerInRange(position, detection)) {
            entity.GetComponent<AIStateComponent>().CurrentState = AIState.Chase;
            entity.GetComponent<AIStateComponent>().StateTimer = 0.0f;
        }
    }

   private void HandleChaseState(Entity entity, PositionComponent position) {
        var aiState = entity.GetComponent<AIStateComponent>();

        if (entity.GetComponent<PlayerDetectionComponent>().IsPlayerDetected) {
            if (currentPath.Count == 0 || aiState.StateTimer > RecalculatePathInterval) {
                currentPath = pathfinder.FindPath(position.Position, player.Position);
                currentPathIndex = 0;
                aiState.StateTimer = 0.0f;
            }

            if (currentPath != null && currentPathIndex < currentPath.Count) {
                MoveTowards(position, currentPath[currentPathIndex]);

                if (Vector2.Distance(position.Position, currentPath[currentPathIndex]) < PathPointThreshold) {
                    currentPathIndex++;
                }
            }

            if (position.DistanceTo(player.Position) <= AttackRange) {
                aiState.CurrentState = AIState.Attack;
                aiState.StateTimer = 0.0f;
            }
        } else {
            aiState.CurrentState = AIState.Idle;
        }
    }

    private void HandleAttackState(Entity entity, PositionComponent position) {
        if (position.DistanceTo(player.Position) <= AttackRange) {
            // Execute attack logic
            PerformAttack(entity);

            if (entity.GetComponent<AIStateComponent>().StateTimer > AttackCooldown) {
                entity.GetComponent<AIStateComponent>().CurrentState = AIState.Chase;
                entity.GetComponent<AIStateComponent>().StateTimer = 0.0f;
            }
        } else {
            entity.GetComponent<AIStateComponent>().CurrentState = AIState.Chase;
        }
    }

    private void HandleFleeState(Entity entity, PositionComponent position) {
        // Handle flee logic if needed......?
    }

    private void HandleDeadState(Entity entity) {
        // Handle death logic, removing the entity or playing an animation......?
    }

    private void HandleBossPhase1State(Entity entity) {
        // Boss phase 1 logic
        if (entity.GetComponent<HealthComponent>().Health <= BossPhase2HealthThreshold) {
            entity.GetComponent<AIStateComponent>().CurrentState = AIState.BossPhase2;
        }
    }

    private void HandleBossPhase2State(Entity entity) {
        // Boss phase 2 logic
        if (entity.GetComponent<HealthComponent>().Health <= EnrageThreshold) {
            entity.GetComponent<AIStateComponent>().CurrentState = AIState.BossEnraged;
        }
    }

    private void HandleBossEnragedState(Entity entity) {
        // Enraged attack pattern
        if (entity.GetComponent<HealthComponent>().Health <= 0) {
            entity.GetComponent<AIStateComponent>().CurrentState = AIState.Dead;
        }
    }

    // Helper methods
    private bool IsPlayerInRange(PositionComponent position, PlayerDetectionComponent detection) {
        return position.DistanceTo(player.Position) <= detection.DetectionRadius;
    }

    private void MoveTowards(PositionComponent from, PositionComponent to) {
        // Implement movement logic towards the player......
    }

    private void PerformAttack(Entity entity) {
        // Implement attack logic (e.g., damage the player, fire bullets, etc.)......
    }
}
