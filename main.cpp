// Main file
#include "headers/libs.hpp"

// Main function
int main(){
  // Initialize window
  const int WINDOW_WIDTH = 640;
  const int WINDOW_HEIGHT = 480;
  sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Snake");

  // Initialize fonts
  sf::Font defaultFont;
  defaultFont.loadFromFile("fonts/arial.ttf");

  // Initialize game state
  gameState state = GAME_RUN;

  // Initialize gameplay variables
  int blockSize = 40;
  sf::Vector2i headPos(5, 5);
  int score = 0;
  direction snakeDir = NORTH;
  direction previousDir = snakeDir;
  float moveTime = 0.75f;
  sf::Clock clock;

  // Initialize apple
  sf::Vector2i applePos(7, 7);
  block apple = {new sf::RectangleShape(sf::Vector2f(blockSize, blockSize)) };
  apple.shape->setFillColor(sf::Color::Red);
  apple.shape->setPosition(sf::Vector2f(applePos.x * blockSize, applePos.y * blockSize));

  // Initialize array of snake blocks
  std::vector<block> blocks;
  block firstBlock = { new sf::RectangleShape(sf::Vector2f(blockSize, blockSize)) };
  firstBlock.shape->setPosition(sf::Vector2f(headPos.x * blockSize, headPos.y * blockSize));
  blocks.push_back(firstBlock);

  // Initialize random apple seed
  srand(time(NULL));

  // Lambda functions
  auto moveApple = [&](){
    int newX = rand() % (WINDOW_WIDTH / blockSize);
    int newY = rand() % (WINDOW_HEIGHT / blockSize);

    applePos = sf::Vector2i(newX, newY);
    apple.shape->setPosition(sf::Vector2f(applePos.x * blockSize, applePos.y * blockSize));
  };
  auto addSnakeBlock = [&](){
    block temp = { new sf::RectangleShape(sf::Vector2f(blockSize, blockSize)) };
    temp.shape->setPosition(sf::Vector2f(-blockSize * score, -blockSize * score));
    blocks.push_back(temp);
  };
  auto displayMenu = [&](){
    // Title
    sf::Text text("Snake", defaultFont);
    text.setCharacterSize(40);
    text.setPosition((WINDOW_WIDTH / 2) - (text.getLocalBounds().width / 2), 60);

    // Start button
    sf::RectangleShape startButton(sf::Vector2f(300, 40));
    startButton.setPosition((WINDOW_WIDTH / 2) - (startButton.getLocalBounds().width / 2), 200);
    sf::Text startText("START GAME", defaultFont);
    startText.setFillColor(sf::Color::Black);
    startText.setPosition(sf::Vector2f((startButton.getPosition().x) + (startText.getLocalBounds().width / 3),
                                        startButton.getPosition().y));

    // Quit button
    sf::RectangleShape quitButton(sf::Vector2f(300, 40));
    quitButton.setPosition((WINDOW_WIDTH / 2) - (quitButton.getLocalBounds().width / 2), 250);
    sf::Text quitText("QUIT GAME", defaultFont);
    quitText.setFillColor(sf::Color::Black);
    quitText.setPosition(sf::Vector2f((quitButton.getPosition().x) + (quitText.getLocalBounds().width / 2.25),
                                        quitButton.getPosition().y));

    // Handle controls
    if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
      if(inside(sf::Vector2f(sf::Mouse::getPosition(window)), startButton)){
        std::cout << "Starting game" << std::endl;
        state = GAME_RUN;
      } else if(inside(sf::Vector2f(sf::Mouse::getPosition(window)), quitButton)){
        std::cout << "Quitting game" << std::endl;
        window.close();
      }
    }

    // Draw it all
    window.draw(text);
    window.draw(startButton);
    window.draw(startText);
    window.draw(quitButton);
    window.draw(quitText);
  };
  auto displayGame = [&](){
    // Handle controls
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W) && previousDir != SOUTH)
      snakeDir = NORTH;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A) && previousDir != WEST)
      snakeDir = EAST;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S) && previousDir != NORTH)
      snakeDir = SOUTH;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D) && previousDir != EAST)
      snakeDir = WEST;
      
    // Only move if the time has been reached
    if(clock.getElapsedTime().asSeconds() > moveTime){
      previousDir = snakeDir;

      if(snakeDir == NORTH)
        headPos.y -= 1;
      else if(snakeDir == EAST)
        headPos.x -= 1;
      else if(snakeDir == SOUTH)
        headPos.y += 1;
      else if(snakeDir == WEST)
        headPos.x += 1;

      // Update the snakes position
      for(int i = blocks.size() - 1; i >= 0; i--){
        if(i == 0)
          blocks[i].shape->setPosition(sf::Vector2f(headPos.x * blockSize, headPos.y * blockSize));
        else
          blocks[i].shape->setPosition(blocks[i - 1].shape->getPosition());
      }

      // Check walls for collisions
      if(headPos.x < 0 || headPos.x > WINDOW_WIDTH / blockSize || headPos.y < 0 || headPos.y > WINDOW_HEIGHT / blockSize){
        std::cout << "Game ended" << std::endl;
        state = GAME_END;
      }

      // Check for self-snake collisions
      for(int i = blocks.size() - 1; i >= 0; i--){
        if(i != 0){
          float snakeX = blocks[i].shape->getPosition().x / blockSize;
          float snakeY = blocks[i].shape->getPosition().y / blockSize;
          if(snakeX == headPos.x && snakeY == headPos.y){
            std::cout << "You hit yourself!" << std::endl;
            state = GAME_END;
          }
        }
      }

      // Check if the snake ate the apple
      if(headPos.x == applePos.x && headPos.y == applePos.y){
        moveApple();
        addSnakeBlock();

        // Slowly increase movement speed
        moveTime -= 0.1f;
      }

      // Restart once moved
      clock.restart().asSeconds();
    }

    // Set the score as the length of the snake
    score = blocks.size();

    // Render all the blocks and apple
    window.draw(*apple.shape);
    for(int i = 0; i < blocks.size(); i++)
      window.draw(*blocks[i].shape);
  };
  auto displayEnd = [&](){
    // Title
    sf::Text text("Snake", defaultFont);
    text.setCharacterSize(40);
    text.setPosition((WINDOW_WIDTH / 2) - (text.getLocalBounds().width / 2), 40);

    // Score
    sf::Text scoreText("Score: 0", defaultFont);
    scoreText.setString("Score: " + std::to_string(score));
    scoreText.setPosition((WINDOW_WIDTH / 2) - (text.getLocalBounds().width / 2), 100);

    // Start button
    sf::RectangleShape startButton(sf::Vector2f(300, 40));
    startButton.setPosition((WINDOW_WIDTH / 2) - (startButton.getLocalBounds().width / 2), 200);
    sf::Text startText("START GAME", defaultFont);
    startText.setFillColor(sf::Color::Black);
    startText.setPosition(sf::Vector2f((startButton.getPosition().x) + (startText.getLocalBounds().width / 3),
                                        startButton.getPosition().y));

    // Quit button
    sf::RectangleShape quitButton(sf::Vector2f(300, 40));
    quitButton.setPosition((WINDOW_WIDTH / 2) - (quitButton.getLocalBounds().width / 2), 250);
    sf::Text quitText("QUIT GAME", defaultFont);
    quitText.setFillColor(sf::Color::Black);
    quitText.setPosition(sf::Vector2f((quitButton.getPosition().x) + (quitText.getLocalBounds().width / 2.25),
                                        quitButton.getPosition().y));

    // Handle controls
    if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
      if(inside(sf::Vector2f(sf::Mouse::getPosition(window)), startButton)){
        std::cout << "Starting game" << std::endl;

        // Reset game variables
        score = 0;
        headPos = sf::Vector2i(5, 5);
        snakeDir = NORTH;
        blocks.erase(blocks.begin() + 1, blocks.end());
        moveTime = 0.75f;

        state = GAME_RUN;
      } else if(inside(sf::Vector2f(sf::Mouse::getPosition(window)), quitButton)){
        std::cout << "Quitting game" << std::endl;
        window.close();
      }
    }

    // Draw it all
    window.draw(text);
    window.draw(scoreText);
    window.draw(startButton);
    window.draw(startText);
    window.draw(quitButton);
    window.draw(quitText);
  };

  // Window loop
  while(window.isOpen()){
    // Handle events
    sf::Event event;
    while(window.pollEvent(event)){
      if(event.type == sf::Event::Closed)
        window.close();
    }

    // Rendering
    window.clear();

    // Render based on game type
    if(state == GAME_MENU){
      displayMenu();
    } else if(state == GAME_RUN){
      displayGame();
    } else if(state == GAME_END){
      displayEnd();
    }

    // Display to the window
    window.display();
  }
}
