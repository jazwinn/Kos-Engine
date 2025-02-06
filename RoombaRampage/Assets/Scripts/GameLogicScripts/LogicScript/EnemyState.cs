using System;
using System.Collections;
using System.Collections.Generic;

public abstract class EnemyState
{
    #region Default Functions
    #endregion

    protected EnemyScript enemyScript; //References to EnemyScript
    public EnemyState(EnemyScript enemyScript)
    {
        this.enemyScript = enemyScript;
    }

    public abstract void DoActionUpdate(float dTime);

    public abstract void LostTarget();

    public abstract void PlayerDead();

    public abstract void EnemyDead();



}



public class EnemyStatePatrol : EnemyState
{
    public EnemyStatePatrol(EnemyScript enemyScript) : base(enemyScript)
    {
    }

    public override void DoActionUpdate(float dTime)
    {
        enemyScript.UpdateRayCastToPlayerPosition(); //Update Location of player

        switch (enemyScript.enemyRoamBehaviour)
        {
            case EnemyScript.EnemyRoamType.Static:
                break;

            case EnemyScript.EnemyRoamType.Patrolling:
                enemyScript.StartPatrol();
                break;

            default:
                break;
        }

        if (enemyScript.CheckPlayerWithinSight()) //Checks if player is within sight
        {
            switch (enemyScript.enemyType)
            {
                case EnemyScript.EnemySelection.Helpless: //Start Helpless State
                    enemyScript.SetCurrentState(new EnemyStateRun(enemyScript));
                    enemyScript.isPatrolling = false;
                    break;

                case EnemyScript.EnemySelection.Melee: //Start Melee State
                    enemyScript.SetCurrentState(new EnemyStateChase(enemyScript));
                    break;

                case EnemyScript.EnemySelection.Ranged: //Start Ranged State
                    break;

                default:
                    break;
            }
        }
    }

    public override void LostTarget() { } //Do nothing, as enemies can't lose targets when in Patrolling state

    public override void PlayerDead() { } //Do nothing, as enemies won't react when player dies in Patrolling state

    public override void EnemyDead()
    {
    }

}

public class EnemyStateChase : EnemyState
{
    public EnemyStateChase(EnemyScript enemyScript) : base(enemyScript)
    {
    }

    public override void DoActionUpdate(float dTime)
    {
        enemyScript.RunAtPlayer();
    }

    public override void LostTarget()
    {
        enemyScript.SetCurrentState(new EnemyStatePatrol(enemyScript));
    }

    public override void PlayerDead()
    {
    }

    public override void EnemyDead()
    {
        enemyScript.SetCurrentState(new EnemyStateEnemyDead(enemyScript));
    }

}

public class EnemyStateRun : EnemyState
{

    public EnemyStateRun(EnemyScript enemyScript) : base(enemyScript)
    {

    }

    public override void DoActionUpdate(float dTime)
    {
        enemyScript.RunFromPlayer();
    }

    public override void LostTarget()
    {
    }

    public override void PlayerDead()
    {
    }

    public override void EnemyDead()
    {
        enemyScript.SetCurrentState(new EnemyStateEnemyDead(enemyScript));
    }

}
public class EnemyStateEnemyDead : EnemyState
{
    public EnemyStateEnemyDead(EnemyScript enemyScript) : base(enemyScript)
    {

    }

    public override void DoActionUpdate(float dTime)
    {

    }

    public override void LostTarget() { }

    public override void PlayerDead() { }

    public override void EnemyDead() { }

}
