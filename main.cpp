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

  // Initialize array of snake blocks
  std::vector<block*> blocks;
  block firstBlock = { new sf::RectangleShape(sf::Vector2f(blockSize, blockSize)) };
  firstBlock.shape->setPosition(sf::Vector2f(headPos.x * blockSize, headPos.y * blockSize));
  blocks.push_back(&firstBlock);

  // Lambda functions
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
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
      snakeDir = NORTH;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
      snakeDir = EAST;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
      snakeDir = SOUTH;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
      snakeDir = WEST;

    // Move snake accordingly
    

    // Render all the blocks
    for(int i = 0; i < blocks.size(); i++)
      window.draw(*blocks[i]->shape);
  };
  auto displayEnd = [&](){

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
