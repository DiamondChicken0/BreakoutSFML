#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <unordered_map>
#include <iostream>
#include <valarray>

using namespace sf;
using namespace std;

enum Controls
{
    MOVE_LEFT,
    MOVE_RIGHT,
    PAUSE
};

unordered_map<Controls, Keyboard::Key> ControlMap =
        {
            {MOVE_LEFT, Keyboard::A},
            {MOVE_RIGHT, Keyboard::D},
            {PAUSE, Keyboard::Escape}
        };

class gameLogic
{
    public:
    RectangleShape gameBG;
    vector<RectangleShape> blockList;
    RectangleShape paddle;
    CircleShape ball;
    bool movingR = false;
    bool movingL = false;
    float ballVelocityY = 0;
    float ballVelocityX = 0;
    float ballVelocityH = 3.5; // speed

    void createMap(RenderWindow* window, int columns, int rows)
    {
        columns++;
        int xOffsetGlobal = 100;
        int yOffsetGlobal = 50;

        int xOffsetBlocks = xOffsetGlobal;
        int topOffsetBlocks = yOffsetGlobal + 50;
        int bottomOffsetBlocks = yOffsetGlobal + 400;

        int colorIncrement = -1;
        vector<Color> blockColorPatterns =
        {
                Color{0xff3636ff}, // Red
                Color{0xff8336ff}, // Orange
                Color{0xffde36ff}, // Yellow
                Color{0x94ff36ff}, // Green
                Color{0x36ffa8ff}, // Teal (Aquamarine)
                Color{0x36b2ffff}, // Blue
                Color{0x5136ffff}, // Violet
                Color{0xbd34ebff}, // Purple
                Color{0xeb34baff}  // Pink
        };

        gameBG.setSize(Vector2f(int(window->getSize().x - xOffsetGlobal*2 - 2),
                                       int(window->getSize().y - yOffsetGlobal * 2)));
        gameBG.setPosition(xOffsetGlobal-4,yOffsetGlobal);
        gameBG.setFillColor(Color::Black);
        gameBG.setOutlineThickness(4);
        gameBG.setOutlineColor(Color::White);
        //window->draw(gameBG);

        paddle.setSize((Vector2f(150,20)));
        paddle.setFillColor(Color::White);
        paddle.setOrigin(paddle.getSize().x/2, paddle.getSize().y/2);
        paddle.setPosition(window->getSize().x/2, window->getSize().y * 0.92);

        ball.setRadius(10);
        ball.setPosition(paddle.getPosition().x, paddle.getPosition().y + 30);
        ball.setFillColor(Color::White);




        for (int i = xOffsetBlocks; i < window->getSize().x - (xOffsetBlocks*2);
        i += int(window->getSize().x - xOffsetBlocks)/columns)
        {
            colorIncrement = (colorIncrement < blockColorPatterns.size()-1) ? colorIncrement + 1 : colorIncrement = 0;
            for (int j = topOffsetBlocks; j < window->getSize().y - bottomOffsetBlocks;
            j += int((window->getSize().y - topOffsetBlocks)/ rows))
            {


                RectangleShape block(Vector2f(int(window->getSize().x - xOffsetBlocks)/columns,
                               int((window->getSize().y - topOffsetBlocks)/rows)));
                block.setOutlineColor(Color::Black);
                block.setOutlineThickness(4);
                block.setPosition(i,j);
                block.setFillColor(blockColorPatterns[colorIncrement]);

                blockList.push_back(block);
                //window->draw(block);
            }
        }

        while (ballVelocityX < 0.5 && ballVelocityX > -0.5)
        {
            ballVelocityX = rand() % 3 - 2;
        }

        ballVelocityY = -1 * sqrt(pow(ballVelocityH, 2) - pow(ballVelocityX,2));

    }

    void nextFrame(RenderWindow* window)
    {
        //cout << "old : " << ballVelocityX << ", " << ballVelocityY << "\n";
        //cout << "pos : " << ball.getPosition().x << ", " << ball.getPosition().y << "\n";

        if (movingL && (paddle.getGlobalBounds().left > gameBG.getGlobalBounds().left + 4))
        {
            paddle.move(-5,0);
        }

        if (movingR && (paddle.getGlobalBounds().left + paddle.getGlobalBounds().width <
        gameBG.getGlobalBounds().left + gameBG.getGlobalBounds().width - 4))
        {
            paddle.move(5,0);
        }

        Vector2f predictedPos(ball.getPosition().x + ballVelocityX, ball.getPosition().y + ballVelocityY);
        bool winCond = true;
        for (RectangleShape &blocks: blockList)
        {
            //at this point the ball is going to hit and will be inside.
            if ((blocks.getFillColor() != Color::Transparent &&
            predictedPos.x >= blocks.getGlobalBounds().left &&
            predictedPos.x <= blocks.getGlobalBounds().left + blocks.getGlobalBounds().width) &&
            predictedPos.y >= blocks.getGlobalBounds().top &&
            predictedPos.y <= blocks.getGlobalBounds().top + blocks.getGlobalBounds().height)
            {
                cout << "old : " << ballVelocityX << ", " << ballVelocityY << "\n";
                cout << "new : " << ballVelocityX << ", " << ballVelocityY << "\n";

                float ballAngle = 360 + atan(ballVelocityY / ballVelocityX) * -1;
                ballVelocityX = sin(ballAngle) * ballVelocityH * -1;
                ballVelocityY = cos(ballAngle) * ballVelocityH;

                blocks.setFillColor(Color::Transparent);
                break;
            }
            if (blocks.getFillColor() != Color::Transparent)
                winCond = false;
        }

        if (((predictedPos.x >= paddle.getGlobalBounds().left &&
            predictedPos.x <= paddle.getGlobalBounds().left + paddle.getGlobalBounds().width &&
            predictedPos.y >= paddle.getGlobalBounds().top &&
            predictedPos.y <= paddle.getGlobalBounds().top + paddle.getGlobalBounds().height) ||
            (predictedPos.x <= gameBG.getGlobalBounds().left &&
            predictedPos.x >= gameBG.getGlobalBounds().left + gameBG.getGlobalBounds().width &&
            predictedPos.y <= gameBG.getGlobalBounds().top &&
            predictedPos.y >= gameBG.getGlobalBounds().top + gameBG.getGlobalBounds().height)))
        {
            float ballAngle = 360 + atan(ballVelocityY / ballVelocityX) * -1;
            ballVelocityX = sin(ballAngle) * ballVelocityH * -1;
            ballVelocityY = cos(ballAngle) * ballVelocityH;
        }

        ball.move(ballVelocityX,ballVelocityY);


        window->draw(gameBG);
        window->draw(paddle);
        for (RectangleShape blocks: blockList)
        {
            window->draw(blocks);

        }
        window->draw(ball);

    }


};

int main() {

    Clock clock;


    const int fps = 144;
    const Time timePerFrame = seconds(1.0/fps);

    ContextSettings settings;
    settings.antialiasingLevel = 8;
    RenderWindow window(VideoMode(1920,1080), "Breakout", Style::Default, settings);

    window.setVerticalSyncEnabled(true);

    gameLogic logic;
    logic.createMap(&window, 18, 16);

    while (window.isOpen())
    {
        //clock.restart();
        Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case (Event::Closed):
                {
                    window.close();
                    break;
                }

                case (Event::KeyPressed):
                {
                    if(Keyboard::isKeyPressed(ControlMap[MOVE_LEFT]))
                    {
                        logic.movingL = true;
                    }
                    if(Keyboard::isKeyPressed(ControlMap[MOVE_RIGHT]))
                    {
                        logic.movingR = true;
                    }
                    if(Keyboard::isKeyPressed(ControlMap[PAUSE]))
                    {

                    }
                    break;
                }

                case (Event::KeyReleased):
                {
                    if(event.key.code == ControlMap[MOVE_LEFT])
                    {
                        logic.movingL = false;
                    }
                    if(event.key.code == ControlMap[MOVE_RIGHT])
                    {
                        logic.movingR = false;
                    }
                    if(event.key.code == ControlMap[PAUSE])
                    {

                    }
                    break;
                }
            }
        }
        window.clear();
        logic.nextFrame(&window);
        window.display();
        sleep(timePerFrame - clock.getElapsedTime());
    }
    return 0;
}
