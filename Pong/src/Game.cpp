#include "Game.h"

Game::Game()
	:gameState(GameState::START)
{
	initialize();
}

Game::~Game()
{
	glfwTerminate();
}

void Game::run()
{
}

void Game::initialize()
{

}

void Game::handleInput()
{
}

void Game::update()
{
}

void Game::render()
{
}

void Game::restart()
{
}
