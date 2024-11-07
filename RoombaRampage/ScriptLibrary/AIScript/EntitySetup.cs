//Each enemy or boss will need these components to function within the FSM

var enemy = new Entity();
enemy.AddComponent(new AIStateComponent());
enemy.AddComponent(new HealthComponent(100));
enemy.AddComponent(new PositionComponent(0, 0));
enemy.AddComponent(new PlayerDetectionComponent(50.0f));  // Detection radius of 50 units
