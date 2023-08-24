#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "EntityManager.h"

struct PlayerConfig { int SR, CR; float S; int FR, FG, FB, OR, OG, OB, OT, V; };
struct EnemyConfig { int SR, CR; float SMIN, SMAX; int OR, OG, OB, OT, VMIN, VMAX, L, SP; };
struct BulletConfig { int SR, CR; float S; int FR, FG, FB, OR, OG, OB, OT, V, L; };

class Game {
private:
	sf::RenderWindow m_window;
	EntityManager m_entities;
	sf::Font m_font;
	sf::Text m_text;
	PlayerConfig m_playerConfig;
	EnemyConfig m_enemyConfig;
	BulletConfig m_bulletConfig;
	std::shared_ptr<Entity> m_player;

	int m_score = 0;
	int m_currentFrame = 0;
	int last_EnemySpawnTime = 0;

	bool m_paused = false;
	bool m_running = true;

	void init(const std::string& config);
	void setPaused(bool paused);

	//Systems
	void sMovement();
	void sUserInput();
	void sRender();
	void sEnemySpawner();
	void sCollision();

	//help func
	void spawnPlayer();
	void spawnEnemy();
	void spawnSmallEnemies(std::shared_ptr<Entity>);
	void spawnBullet(std::shared_ptr<Entity> entity, const Vec2& mousePos);
	void spawnSpecialWeapon(std::shared_ptr<Entity> entity);
	bool isCollision(std::shared_ptr<Entity> lhs, std::shared_ptr<Entity> rhs);
	
public:
	Game(const std::string& config);
	void run();
};