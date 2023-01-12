#pragma once
class Grid;
class Snake;
class GridGraph;

class Game final
{
public:
	explicit Game( const Window& window );
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game( Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	~Game();

	void Update( float elapsedSec );
	void Draw( ) const;

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e );
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e );
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e );
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e );
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e );

private:
	// DATA MEMBERS
	const Window m_Window;

	std::unique_ptr<Grid> m_pGrid;
	std::unique_ptr<Snake> m_pSnake;
	std::shared_ptr<GridGraph> m_pGraph;

	float m_Timer{};
	const float m_Time{ .5f };

	int m_Score{};

	bool m_SnakeDead{false};

	bool m_UsePathFinding{ false };

	// FUNCTIONS
	void Initialize( );
	void Cleanup( );
	void ClearBackground( ) const;

	void UpdateGame();
};