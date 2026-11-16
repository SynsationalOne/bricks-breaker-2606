#include "stdafx.h"
#include "Game.h"

Game::Game()
{
	Reset();
}

void Game::Reset()
{   bricks.clear();

    Console::SetWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    Console::CursorVisible(false);
	paddle.width = 12;
	paddle.height = 2;
	paddle.x_position = 32;
	paddle.y_position = 30;

	ball.visage = 'O';
	ball.color = ConsoleColor::Cyan;
	ResetBall();

	// DONE #2 - Add this brick and 4 more bricks to the vector
	Box brick;

	brick.width = 10;
	brick.height = 2;
	brick.x_position = 0;
	brick.y_position = 5;
	brick.doubleThick = true;
	brick.color = ConsoleColor::DarkGreen;

    
	for (int i = 0; i < 5; ++i)  // used for loop to keep from doing repeated code
    {   brick.x_position = i * 12;
        bricks.push_back(brick);
	}
}

void Game::ResetBall()
{
	ball.x_position = paddle.x_position + paddle.width / 2;
	ball.y_position = paddle.y_position - 1;
	ball.x_velocity = rand() % 2 ? 1 : -1;
	ball.y_velocity = -1;
	ball.moving = false;
}

bool Game::Update()
{
	if (GetAsyncKeyState(VK_ESCAPE) & 0x1)
		return false;

	if (GetAsyncKeyState(VK_RIGHT) && paddle.x_position < WINDOW_WIDTH - paddle.width)
		paddle.x_position += 2;

	if (GetAsyncKeyState(VK_LEFT) && paddle.x_position > 0)
		paddle.x_position -= 2;

	if (GetAsyncKeyState(VK_SPACE) & 0x1)
		ball.moving = !ball.moving;

	if (GetAsyncKeyState('R') & 0x1)
		Reset();

	ball.Update();
	CheckCollision();
	return true;
}

//  All rendering, including text, should occur in the Render function
void Game::Render() const
{
	Console::Lock(true);
	Console::Clear(); 
	
	paddle.Draw();
	ball.Draw();

	// DONE #3 - Update render to render all bricks
    for (int i = 0; i < bricks.size(); ++i)
    {   bricks[i].Draw();
    }
    
    if (bricks.empty())
    {   Console::WordWrap(WINDOW_WIDTH / 4, WINDOW_HEIGHT / 2, WINDOW_WIDTH * 3/4, "Congrats, you win! Press 'R' to play again.");
    }
    
    if (ball.y_position >= WINDOW_HEIGHT - 1)
    {
        Console::WordWrap(WINDOW_WIDTH / 4, WINDOW_HEIGHT / 2, WINDOW_WIDTH * 3 / 4, "Yikes, defeat! Press 'R' to play again.");
    }

    Console::Lock(false);

}

void Game::CheckCollision()
{
	// DONE #4 - Update collision to check all bricks
    for (int i = 0; i < bricks.size(); ++i)
    {   if (bricks[i].Contains(ball.x_position + ball.x_velocity, ball.y_position + ball.y_velocity))
        {   bricks[i].color = ConsoleColor(bricks[i].color - 1);
            ball.y_velocity *= -1;

            // DONE #5 - If the ball hits the same brick 3 times (color == black), remove it from the vector
            if (bricks[i].color == ConsoleColor::Black)
            {  bricks.erase(bricks.begin() + i);
               break;
            }
        }
    }

	// DONE #6 - If no bricks remain, pause ball and display (render) victory text with R to reset
    if (bricks.empty())
    {   ball.moving = false;
    }

	if (paddle.Contains(ball.x_position + ball.x_velocity, ball.y_velocity + ball.y_position))
	{   ball.y_velocity *= -1;
	}

	// DONE #7 - If ball touches bottom of window, pause ball and display (render) defeat text with R to reset
    if (ball.y_position >= WINDOW_HEIGHT - 1) 
    {
        ball.moving = false;
    }
}
