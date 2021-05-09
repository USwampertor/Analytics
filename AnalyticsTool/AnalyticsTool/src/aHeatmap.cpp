#include "aHeatmap.h"

namespace Analytics {

  bool
  Heatmap::init(int width, int height, float decay, float momentum) {
    m_size.x = width;
    m_size.y = height;

    m_decay = decay;

    m_momentum = momentum;

    return true;
  }

  bool
  Heatmap::startWindow() {
    m_window = new sf::RenderWindow(sf::VideoMode(m_size.x, m_size.y), "Analytics");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    m_window->clear();
    m_window->draw(shape);
    m_window->display();
    
    return m_window->isOpen();
  }

  void
  Heatmap::renderWindow() {

  }

  void
  Heatmap::closeWindow() {
    m_window->close();
  }

  Vector<Vector<float>>
  Heatmap::getInfluenceMap() const {
    return influenceMap;
  }

  float
  Heatmap::getInfluence(sf::Vector2i pos) {
    return influenceMap[pos.x][pos.y];
  }

  float
  Heatmap::getInfluence(float x, float y) {
    return influenceMap[x][y];
  }

  void
  Heatmap::setInfluence(sf::Vector2i pos, float influence) {
    influenceMap[pos.x][pos.y] = influence;
  }

  void
  Heatmap::setInfluence(float x, float y, float influence) {
    influenceMap[x][y] = influence;
  }

  void
  Heatmap::calculateInfluence(sf::Vector2i pos,
                              sf::Vector2u mapSize,
                              float& maxInf,
                              float& minInf) {
    if ((pos.x >= 0) && (pos.x < mapSize.x) && (pos.y >= 0) && (pos.y < mapSize.y)) {
      float inf = getInfluence(pos.x, pos.y) * expf(-1 * m_decay);
      maxInf = std::max(inf, maxInf);
      minInf = std::min(inf, minInf);
    }
  }

  void
  Heatmap::calculateInfluence(int x, int y, sf::Vector2u mapSize, float& maxInf, float& minInf) {

    if ((x >= 0) && (x < mapSize.x) && (y >= 0) && (y < mapSize.y)) {
      float inf = getInfluence(x, y) * expf(-1 * m_decay);
      maxInf = std::max(inf, maxInf);
      minInf = std::min(inf, minInf);
      // 
      // setInfluence(x, y, lerp(getInfluence(x, y), inf, m_momentum));
    }

  }

  void
  Heatmap::propagateInfluence(sf::Vector2i pos, sf::Vector2u mapSize) {
    float maxInf = 0.0f;
    float minInf = 0.0f;

    //Left
    calculateInfluence(pos.x - 1, pos.y, mapSize, maxInf, minInf);
    //UP Left
    calculateInfluence(pos.x - 1, pos.y + 1, mapSize, maxInf, minInf);
    //Up
    calculateInfluence(pos.x, pos.y + 1, mapSize, maxInf, minInf);
    //Up Right
    calculateInfluence(pos.x + 1, pos.y + 1, mapSize, maxInf, minInf);
    //Right
    calculateInfluence(pos.x + 1, pos.y, mapSize, maxInf, minInf);
    //Down right
    calculateInfluence(pos.x + 1, pos.y - 1, mapSize, maxInf, minInf);
    //Down
    calculateInfluence(pos.x, pos.y - 1, mapSize, maxInf, minInf);
    //Down Left
    calculateInfluence(pos.x - 1, pos.y - 1, mapSize, maxInf, minInf);

    float newInfluence = minInf + maxInf;
    setInfluence(pos, lerp(getInfluence(pos), newInfluence, m_momentum));
  }

  void
  Heatmap::bsfPropagate(sf::Vector2i pos, sf::Vector2u mapSize) {
    Vector<sf::Vector2f> openList;
    Vector<sf::Vector2f> closedList;
  }

  void
  Heatmap::setAllInfluence() {

    sf::Vector2u mapSize(influenceMap.size(), influenceMap[0].size());

    //for (auto pos : sortedMap) {
    //  propagateInfluence(pos.first, mapSize);
    //}

    for (int i = 0; i < mapSize.x; ++i) {
      for (int j = 0; j < mapSize.y; ++j) {
        propagateInfluence(sf::Vector2i(i, j), mapSize);
      }
    }
    for (int i = mapSize.x - 1; i > -1; --i) {
      for (int j = mapSize.y - 1; j > -1; --j) {
        propagateInfluence(sf::Vector2i(i, j), mapSize);
      }
    }

  }

  sf::Sprite
  Heatmap::getHeatmapSprite(sf::Vector2u mapSize) {
    // Creates the heatmap image to start editing the pixel colors
    sf::Image heatmapImg;
    heatmapImg.create(mapSize.x, mapSize.y, sf::Color::Transparent);

    // Draw the information
    // TODO: CHANGE THIS WITH THE INFLUENCE MAP
    int j = 0;
    sf::Vector2i pixelPosition;
    sf::Color c;
    uint32 posX = 0;
    uint32 posY = 0;

    sf::Color redC = sf::Color::Red;
    sf::Color yellowC = sf::Color::Yellow;
    // yellowC.a = 0.9f;
    sf::Color greenC = sf::Color::Green;
    // greenC.a = 0.8f;
    sf::Color blueC = sf::Color::Blue;
    // blueC.a = 0.7f;

    for (auto column : influenceMap) {
      posY = 0;
      for (auto pos : column) {
        // Defines the color that should appear at a given point
        // c = BlendColor(sf::Color::Transparent, sf::Color::Red, pos);
        c = pos > 0.75 ? BlendColor(yellowC, redC, pos) :
          pos > 0.50 ? BlendColor(greenC, yellowC, pos) :
          pos > 0.25 ? BlendColor(blueC, greenC, pos) :
          pos <= 1.0f ?
          BlendColor(sf::Color::Transparent, blueC, pos) :
          BlendColor(sf::Color::Transparent, blueC, 1.0f);
        heatmapImg.setPixel(posX, posY, c);
        ++posY;
      }
      ++posX;
    }

    // makes the texture transparent from a specific key color and sets it into a
    // Renderable sprite
    // heatmapImg.createMaskFromColor(sf::Color::Transparent);
    sf::Texture heatMapTexture;
    heatMapTexture.create(mapSize.x, mapSize.y);
    heatMapTexture.update(heatmapImg);
    sf::Sprite heatMapSprite;
    heatMapSprite.setTexture(heatMapTexture);

    return heatMapSprite;
  }

  Vector<sf::CircleShape>
  Heatmap::getCircleMap() {

    sf::Color c;
    uint32 posX = 0;
    uint32 posY = 0;

    float maxSize = 100.0f;
    Vector<sf::CircleShape> circleMap;

    float maxValue = 0;

    for (auto column : influenceMap) {
      posY = 0;
      for (auto pos : column) {
        maxValue = std::max(maxValue, pos);
        ++posY;
      }
      ++posX;
    }

    posX = 0;
    posY = 0;

    sf::Color blueC = sf::Color::Blue;

    for (auto column : influenceMap) {
      posY = 0;
      for (auto pos : column) {

        float normPos = pos / maxValue;

        if (normPos > 0.01f) {
          sf::CircleShape circle(normPos * maxSize > 1.0f ? normPos * maxSize : 1.0f);
          sf::Color clearBlue = BlendColor(sf::Color::Transparent, blueC, normPos);
          circle.setFillColor(clearBlue);
          circle.setOutlineColor(sf::Color::Cyan);
          circle.setOutlineThickness(1.0f);
          circle.setPosition(sf::Vector2f(posX - circle.getRadius(), posY - circle.getRadius()));
          circleMap.push_back(circle);
        }

        ++posY;
      }
      ++posX;
    }

    return circleMap;
  }

  void
  Heatmap::setMomentum(float momentum) { m_momentum = momentum; }

  void
  Heatmap::setDecay(float decay) { m_decay = decay; }

  void
  Heatmap::createHeatmapStats() {

  }

  void
  Heatmap::showHeatmap() {
    Map<sf::Vector3f*, int> countingMap;









  }

  sf::Color
  Heatmap::BlendColor(sf::Color color1, sf::Color color2, float fraction) {
    sf::Color toReturn;

    toReturn.r = static_cast<sf::Uint8>((color2.r - color1.r) * fraction + color1.r);
    toReturn.g = static_cast<sf::Uint8>((color2.g - color1.g) * fraction + color1.g);
    toReturn.b = static_cast<sf::Uint8>((color2.b - color1.b) * fraction + color1.b);
    toReturn.a = static_cast<sf::Uint8>((color2.a - color1.a) * fraction + color1.a);

    return toReturn;
  }

}
