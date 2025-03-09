using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

public class EnemyStateRangedSearch : EnemyState
{
    private float searchDuration = 5.0f;
    private float searchTimer;
    private float scanInterval = 0.5f;
    private float scanTimer = 0f;
    private Vector2 lastKnownPlayerPosition;

    public EnemyStateRangedSearch(EnemyScript enemyScript) : base(enemyScript)
    {
        searchTimer = searchDuration;
        lastKnownPlayerPosition = enemyScript.GetPlayerPosition();
    }

    public override void DoActionUpdate(float dTime)
    {
        if (enemyScript.isDead) return;

        scanTimer -= dTime;
        if (scanTimer <= 0)
        {
            scanTimer = scanInterval;
            enemyScript.UpdateRayCastToPlayerPosition();

            // Check if player is back in sight
            if (enemyScript.CheckPlayerWithinSight() && enemyScript.IsPlayerInFOV())
            {
                // Player spotted again, switch back to attack
                enemyScript.SetCurrentState(new EnemyStateRangedAttack(enemyScript));
                return;
            }
        }

        // Move toward last known player position
        enemyScript.MoveToLastKnownPosition(lastKnownPlayerPosition);

        searchTimer -= dTime;
        if (searchTimer <= 0)
        {
            enemyScript.SetCurrentState(new EnemyStateRangedPatrol(enemyScript));
        }
    }

    public override void LostTarget()
    {
        // Already searching, nothing to do
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
