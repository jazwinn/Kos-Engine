//This component stores the health of the entity

public class HealthComponent {
    public int Health;
    public int MaxHealth;
    
    public HealthComponent(int maxHealth) {
        MaxHealth = maxHealth;
        Health = maxHealth;
    }

    //To check if entity is dead or not
    public bool IsDead() {
        return Health <= 0;
    }
}
