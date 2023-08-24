#include "Game.h"
#include<cstdlib>
#include<ctime>
#include <iostream>

Game::Game(const std::string& config)
{
	init(config);
}

void Game::run()
{
	while (m_running) {
		m_entities.Update();

		sUserInput();
		if (!m_paused) {
			sMovement();
			sEnemySpawner();
			sCollision();
		}
		sRender();

		m_currentFrame++;
	}

	m_window.close();
}

void Game::init(const std::string& config)
{
	srand(time(0));
	if (!m_font.loadFromFile("fonts\\lazy.ttf"))
	{
		std::cerr << "load font err\n";
		exit(1);
	}
	m_text.setFont(m_font);
	m_text.setCharacterSize(24);
	m_text.setFillColor(sf::Color::White);
	

	m_playerConfig = { 32, 32,5,5, 5, 5, 255, 0, 0, 4, 8 };
	m_enemyConfig = { 32, 32, 3, 8, 255, 255, 255, 2, 3, 8, 90, 60 };
	m_bulletConfig = { 10,10,20, 255, 255, 255, 255, 255, 255, 2, 20, 20 };
	
	m_window.create(sf::VideoMode(1280, 720), "ECS1.0", sf::Style::Titlebar| sf::Style::Close);
	m_window.setFramerateLimit(60);

	spawnPlayer();

}

void Game::setPaused(bool paused)
{
	m_paused = paused;
}


void Game::sUserInput()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed) {
			m_running = false;
		}

		if(event.type == sf::Event::KeyPressed){
			switch (event.key.code)
			{
			case sf::Keyboard::W:
				m_player->cInput->up = true;
				break;
			case sf::Keyboard::A:
				m_player->cInput->left = true;
				break;
			case sf::Keyboard::S:
				m_player->cInput->down = true;
				break;
			case sf::Keyboard::D:
				m_player->cInput->right = true;
				break;
			case sf::Keyboard::K:
				setPaused(!m_paused);
			default:
				break;
			}
		}

		if (event.type == sf::Event::KeyReleased) {
			switch (event.key.code)
			{
			case sf::Keyboard::W:
				m_player->cInput->up = false;
				break;
			case sf::Keyboard::A:
				m_player->cInput->left = false;
				break;
			case sf::Keyboard::S:
				m_player->cInput->down = false;
				break;
			case sf::Keyboard::D:
				m_player->cInput->right = false;
				break;
			default:
				break;
			}
		}

		if (event.type == sf::Event::MouseButtonPressed) {
			Vec2 mousePos(sf::Mouse::getPosition(m_window).x, sf::Mouse::getPosition(m_window).y);
			spawnBullet(m_player, mousePos);
		}
	}
}

void Game::sMovement()
{
	m_player->cTransform->velocity = Vec2(0,0);

	if (m_player->cInput->up) {
		m_player->cTransform->velocity.y = -5;
	}
	if (m_player->cInput->right) {
		m_player->cTransform->velocity.x = 5;
	}
	if (m_player->cInput->down) {
		m_player->cTransform->velocity.y = 5;
	}
	if (m_player->cInput->left) {
		m_player->cTransform->velocity.x = -5;
	}

	m_player->cTransform->pos.x += m_player->cTransform->velocity.x;
	if (m_player->cTransform->pos.x - m_player->cCollision->radius < 0)
		m_player->cTransform->pos.x = m_player->cCollision->radius;
	else if (m_player->cTransform->pos.x + m_player->cCollision->radius > m_window.getSize().x)
		m_player->cTransform->pos.x = m_window.getSize().x - m_player->cCollision->radius;

	m_player->cTransform->pos.y += m_player->cTransform->velocity.y;
	if (m_player->cTransform->pos.y - m_player->cCollision->radius < 0)
		m_player->cTransform->pos.y = m_player->cCollision->radius;
	else if (m_player->cTransform->pos.y + m_player->cCollision->radius > m_window.getSize().y)
		m_player->cTransform->pos.y = m_window.getSize().y - m_player->cCollision->radius;

	for (auto e : m_entities.getEntities(E_TAG::ENEMY)) {
		e->cTransform->pos.x += e->cTransform->velocity.x;
		if (e->cTransform->pos.x - e->cCollision->radius < 0)
		{
			e->cTransform->pos.x = e->cCollision->radius;
			e->cTransform->velocity = Vec2(-e->cTransform->velocity.x, e->cTransform->velocity.y);
		}
		else if (e->cTransform->pos.x + e->cCollision->radius > m_window.getSize().x)
		{
			e->cTransform->pos.x = m_window.getSize().x - e->cCollision->radius;
			e->cTransform->velocity = Vec2(-e->cTransform->velocity.x, e->cTransform->velocity.y);
		}

		e->cTransform->pos.y += e->cTransform->velocity.y;
		if (e->cTransform->pos.y - e->cCollision->radius < 0)
		{
			e->cTransform->pos.y = e->cCollision->radius;
			e->cTransform->velocity = Vec2(e->cTransform->velocity.x, -e->cTransform->velocity.y);
		}
		else if (e->cTransform->pos.y + e->cCollision->radius > m_window.getSize().y)
		{
			e->cTransform->pos.y = m_window.getSize().y - e->cCollision->radius;
			e->cTransform->velocity = Vec2(e->cTransform->velocity.x, -e->cTransform->velocity.y);
		}
	}
	for (auto e : m_entities.getEntities(E_TAG::BULLET)) {
		e->cTransform->pos.x += e->cTransform->velocity.x;
		e->cTransform->pos.y += e->cTransform->velocity.y;
		e->cLifeSpan->remaining--;
		sf::Color cr = e->cShape->circle.getFillColor();
		unsigned alpha = unsigned(255 * (e->cLifeSpan->remaining / (e->cLifeSpan->total * 1.0f)));
		e->cShape->circle.setFillColor(sf::Color(cr.r, cr.g, cr.b, alpha));
		e->cShape->circle.setOutlineColor(sf::Color(cr.r, cr.g, cr.b, alpha));
		if (e->cLifeSpan->remaining <= 0)
			e->destory();
	}
	
}

void Game::sRender()
{
	m_window.clear();

	for (auto e : m_entities.getEntities()) {
		e->cTransform->angle++;
		e->cShape->circle.setPosition(e->cTransform->pos.x, e->cTransform->pos.y);
		e->cShape->circle.setRotation(e->cTransform->angle);
		m_window.draw((e->cShape)->circle);
	}

	m_text.setString("Score: " + std::to_string(m_player->cScore->score));
	m_window.draw(m_text);

	m_window.display();
}

void Game::sEnemySpawner()
{
	if (m_currentFrame - last_EnemySpawnTime >= m_enemyConfig.L) {
		spawnEnemy();
		last_EnemySpawnTime = m_currentFrame;
	}
}

void Game::sCollision()
{
	for (auto enemy : m_entities.getEntities(E_TAG::ENEMY)) {
		if (isCollision(m_player, enemy)) {
			enemy->destory();
			m_player->cTransform->pos = Vec2(m_window.getSize().x / 2, m_window.getSize().y / 2);
			m_player->cScore->score = 0;
			break;
		}
	}

	for (auto bullet : m_entities.getEntities(E_TAG::BULLET)) {
		for (auto enemy : m_entities.getEntities(E_TAG::ENEMY)) {
			if (isCollision(bullet, enemy)) {
				enemy->destory();
				bullet->destory();
				m_player->cScore->score += 100;
			}
		}
	}
}

void Game::spawnPlayer()
{
	auto e = m_entities.addEntity(E_TAG::PLAYER);

	float ex = m_window.getSize().x / 2;
	float ey = m_window.getSize().y / 2;

	e->cTransform = std::make_shared<CTransform>(Vec2(ex, ey), Vec2(), 0);
	e->cShape = std::make_shared<CShape>
		(m_playerConfig.SR, m_playerConfig.V,
			sf::Color(m_playerConfig.FR, m_playerConfig.FG, m_playerConfig.FB),
			sf::Color(m_playerConfig.OR, m_playerConfig.OG, m_playerConfig.OB),
			m_playerConfig.OT
		);
	e->cCollision = std::make_shared<CCollision>(m_playerConfig.CR);
	e->cInput = std::make_shared<CInput>();
	e->cScore = std::make_shared<CScore>();

	m_player = e;
}

void Game::spawnEnemy()
{
	auto e = m_entities.addEntity(E_TAG::ENEMY);

	float ex = rand() % m_window.getSize().x;
	float ey = rand() % m_window.getSize().y;
	int ev = rand() % (m_enemyConfig.VMAX - m_enemyConfig.VMIN) + m_enemyConfig.VMIN;
	int es = rand() % (int)(m_enemyConfig.SMAX - m_enemyConfig.SMIN) + m_enemyConfig.SMIN;
	Vec2 dir = Vec2(rand() % 5, rand() % 5).normalized();

	e->cTransform = std::make_shared<CTransform>(Vec2(ex,ey), Vec2(dir.x*es, dir.y*es), 0);
	e->cShape = std::make_shared<CShape>
		(m_enemyConfig.SR, ev,
			sf::Color(rand() % 256, rand() % 256, rand() % 256),
			sf::Color(m_enemyConfig.OR, m_enemyConfig.OG, m_enemyConfig.OB),
			m_enemyConfig.OT
		);
	e->cCollision = std::make_shared<CCollision>(m_enemyConfig.CR);
	
}

void Game::spawnSmallEnemies(std::shared_ptr<Entity>)
{

}

void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2& mousePos)
{
	auto e = m_entities.addEntity(E_TAG::BULLET);

	Vec2 startpos(entity->cTransform->pos.x, entity->cTransform->pos.y);
	Vec2 dir = (mousePos - startpos).normalized();

	e->cTransform = std::make_shared<CTransform>(startpos, Vec2(dir.x*m_bulletConfig.S, dir.y* m_bulletConfig.S), 0);
	e->cShape = std::make_shared<CShape>
		(m_bulletConfig.SR, m_bulletConfig.V,
			sf::Color(m_bulletConfig.FR, m_bulletConfig.FG, m_bulletConfig.FB),
			sf::Color(m_bulletConfig.OR, m_bulletConfig.OG, m_bulletConfig.OB),
			m_bulletConfig.OT
		);
	e->cCollision = std::make_shared<CCollision>(m_bulletConfig.CR);
	e->cLifeSpan = std::make_shared<CLifeSpan>(m_bulletConfig.L);

}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity)
{

}

bool Game::isCollision(std::shared_ptr<Entity> lhs, std::shared_ptr<Entity> rhs)
{
	return (lhs->cTransform->pos - rhs->cTransform->pos).length() < lhs->cCollision->radius + rhs->cCollision->radius;
}


