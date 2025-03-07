using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using static EnemyScript;
// EnemyStateRangedPatrol.cs
public class EnemyStateRangedPatrol : EnemyState
{
    private float scanInterval = 0.5f;
    private float scanTimer = 0f;

    public EnemyStateRangedPatrol(EnemyScript enemyScript) : base(enemyScript)
    {
    }

    public override void DoActionUpdate(float dTime)
    {
        if (enemyScript.isDead) return;

        // Patrol behavior based on enemy roam type
        if (enemyScript.enemyRoamBehaviour == EnemyScript.EnemyRoamType.Patrolling)
        {
            enemyScript.StartPatrol();
        }

        // Scan for player periodically
        scanTimer -= dTime;
        if (scanTimer <= 0)
        {
            scanTimer = scanInterval;
            enemyScript.UpdateRayCastToPlayerPosition();

            // Check if player is in sight and FOV
            if (enemyScript.CheckPlayerWithinSight() && enemyScript.IsPlayerInFOV())
            {
                // Player spotted, switch to attack state
                enemyScript.SetCurrentState(new EnemyStateRangedAttack(enemyScript));
            }
        }
    }

    public override void LostTarget()
    {
        // Not applicable in patrol state
    }

    public override void PlayerDead()
    {
        // Return to patrolling
    }

    public override void EnemyDead()
    {
        // Handle enemy death
    }
}

