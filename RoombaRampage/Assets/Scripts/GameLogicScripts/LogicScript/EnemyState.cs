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

//public class EnemyStateRangedChase : EnemyState
//{
//    public EnemyStateRangedChase(EnemyScript enemyScript) : base(enemyScript)
//    {
//        enemyScript.SetChaseValues();

//        if (enemyScript.canChase == false)
//        {
//            enemyScript.StartCoroutine("SawPlayerAlert");
//        }
//    }

//    public override void DoActionUpdate(float dTime)
//    {
//        enemyScript.ChasePlayerRanged();
//    }

//    public override void LostTarget()
//    {
//        enemyScript.SetCurrentState(new EnemyStateSearch(enemyScript));
//    }

//    public override void PlayerDead()
//    {
//        enemyScript.SetCurrentState(new EnemyStatePlayerDead(enemyScript));
//    }

//    public override void EnemyDead()
//    {
//        enemyScript.SetCurrentState(new EnemyStateEnemyDead(enemyScript));
//    }

//}

//public class EnemyStateSearch : EnemyState
//{
//    float searchTimer = 7f;

//    public EnemyStateSearch(EnemyScript enemyScript) : base(enemyScript)
//    {
//        enemyScript.Search();
//    }

//    public override void DoActionUpdate(float dTime)
//    {
//        searchTimer -= dTime;

//        if (enemyScript.CheckPlayerWithinSight() == true && enemyScript.enemyType == EnemyScript.EnemySelection.Melee)
//        {
//            //Debug.Log("Spotted player and chasing");

//            enemyScript.SetCurrentState(new EnemyStateChase(enemyScript));
//        }

//        if (enemyScript.CheckPlayerWithinSight() == true && enemyScript.enemyType == EnemyScript.EnemySelection.Ranged)
//        {
//            enemyScript.SetCurrentState(new EnemyStateRangedChase(enemyScript));
//        }

//        enemyScript.SearchSurrounding();

//        if (searchTimer <= 0)
//        {
//            enemyScript.aiPath.destination = enemyScript.transform.position;
//            enemyScript.SetCurrentState(new EnemyStatePatrol(enemyScript));
//        }
//    }

//    public override void LostTarget() { }

//    public override void PlayerDead()
//    {
//        enemyScript.SetCurrentState(new EnemyStatePlayerDead(enemyScript));
//    }

//    public override void EnemyDead()
//    {
//        enemyScript.SetCurrentState(new EnemyStateEnemyDead(enemyScript));
//    }

//}

//public class EnemyStatePlayerDead : EnemyState
//{
//    public EnemyStatePlayerDead(EnemyScript enemyScript) : base(enemyScript)
//    {
//        enemyScript.SetPatrolValues();

//        enemyScript.enemyFaceDirection.turn = true;
//        enemyScript.enemyFaceDirection.forceLookAtPlayer = false;
//    }

//    public override void DoActionUpdate(float dTime)
//    {
//        enemyScript.Patrol();
//    }

//    public override void LostTarget() { }

//    public override void PlayerDead() { }

//    public override void EnemyDead() { }

//}

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
