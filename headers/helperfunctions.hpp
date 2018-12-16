// Contains functions to help the program
#pragma once

// This function checks if the position is inside the rectangle provided
bool inside(sf::Vector2f pos, sf::RectangleShape shape){
  sf::Vector2f minPos = shape.getPosition();
  sf::Vector2f maxPos(minPos.x + shape.getGlobalBounds().width, minPos.y + shape.getGlobalBounds().height);

  if(pos.x >= minPos.x && pos.x <= maxPos.x)
    if(pos.y >= minPos.y && pos.y <= maxPos.y)
      return true;

  return false;
}
