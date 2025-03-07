using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

// EnemyStateRangedAttack.cs
public class EnemyStateRangedAttack : EnemyState
{
    private float fireRate = 1.5f; // Seconds between shots
    private float fireTimer = 0f;
    private float targetCheckInterval = 0.3f;
    private float targetCheckTimer = 0f;

    public EnemyStateRangedAttack(EnemyScript enemyScript) : base(enemyScript)
    {
        // Initial delay before first shot
        fireTimer = fireRate * 0.5f;
    }

    public override void DoActionUpdate(float dTime)
    {
        if (enemyScript.isDead) return;

        // Check if player is still in sight periodically
        targetCheckTimer -= dTime;
        if (targetCheckTimer <= 0)
        {
            targetCheckTimer = targetCheckInterval;
            enemyScript.UpdateRayCastToPlayerPosition();

            if (!enemyScript.CheckPlayerWithinSight() || !enemyScript.IsPlayerInFOV())
            {
                // Lost sight of player, switch to search state
                enemyScript.SetCurrentState(new EnemyStateRangedSearch(enemyScript));
                return;
            }
        }

        // Always face the player when attacking
        enemyScript.FaceTarget();

        // Fire at player when timer expires
        fireTimer -= dTime;
        if (fireTimer <= 0)
        {
            fireTimer = fireRate;
            enemyScript.FireAtPlayer();
        }
    }

    public override void LostTarget()
    {
        // Switch to search state
        enemyScript.SetCurrentState(new EnemyStateRangedSearch(enemyScript));
    }

    public override void PlayerDead()
    {
        // Return to patrol
        enemyScript.SetCurrentState(new EnemyStateRangedPatrol(enemyScript));
    }

    public override void EnemyDead()
    {
        // Handle enemy death
    }
}
