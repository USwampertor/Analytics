#include "aHeatmap.h"

namespace Analytics {

  bool
  Heatmap::init(int width, int height, float decay, float momentum) {
    m_size.x = width;
    m_size.y = height;

    m_decay = decay;

    m_momentum = momentum;

    m_normalizedMap.clear();

    m_scale = { 0, 0, 0 };

    m_minPoint = { 0, 0, 0 };
    m_maxPoint = { 0, 0, 0 };

    return true;
  }

  bool
  Heatmap::startWindow() {
    m_window = new sf::RenderWindow(sf::VideoMode(m_size.x, m_size.y), "Analytics");
    
    
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
    return m_influenceMap;
  }

  float
  Heatmap::getInfluence(sf::Vector2i pos) {
    return m_influenceMap[pos.x][pos.y];
  }

  float
  Heatmap::getInfluence(float x, float y) {
    return m_influenceMap[x][y];
  }

  void
  Heatmap::setInfluence(sf::Vector2i pos, float influence) {
    m_influenceMap[pos.x][pos.y] = influence;
  }

  void
  Heatmap::setInfluence(float x, float y, float influence) {
    m_influenceMap[x][y] = influence;
  }

  void
  Heatmap::setInfluenceMap(sf::Vector2u mapSize) {
    m_influenceMap.resize(mapSize.x);
    for (int i = 0; i < m_influenceMap.size(); ++i) {
      m_influenceMap[i].resize(mapSize.y);
    }

    sf::Vector2i pixelPos;
    for (auto pos : m_normalizedMap) {
      pixelPos.x = pos.first.x * (mapSize.x);
      pixelPos.y = pos.first.y * (mapSize.y);
      if ((pixelPos.x >= 0) && (pixelPos.x < mapSize.x) &&
        (pixelPos.y >= 0) && (pixelPos.y < mapSize.y)) {
        m_influenceMap[pixelPos.x][pixelPos.y] = pos.second;
      }
    }
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

    sf::Vector2u mapSize(m_influenceMap.size(), m_influenceMap[0].size());

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

    for (auto column : m_influenceMap) {
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

    for (auto column : m_influenceMap) {
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

    for (auto column : m_influenceMap) {
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
  Heatmap::createHeatmapStats(Vector<sf::Vector3i>& info) {
    m_max = 0;
    m_minPoint = { 0,0,0 };
    m_maxPoint = { 0,0,0 };
    m_normalizedMap.clear();


    Map<sf::Vector3i, int, cmpVector3i> countingMap;
    Map<sf::Vector3f, int, cmpVector3f> normalizedMap;
    // To store the min and max point on the heatmap
    sf::Vector3i minPoint(0, 0, 0);
    sf::Vector3i maxPoint(0, 0, 0);
    sf::Vector3f scale(0.0f, 0.0f, 0.0f);

    int maxCount = 0;

    for(sf::Vector3i pos : info) {
      if (countingMap.find(pos) == countingMap.end()) {
        countingMap.insert(std::make_pair(pos, 1));
      }
      else { ++countingMap[pos]; }

      maxPoint.x = maxPoint.x > pos.x ? maxPoint.x : pos.x;
      maxPoint.y = maxPoint.y > pos.y ? maxPoint.y : pos.y;
      maxPoint.z = maxPoint.z > pos.z ? maxPoint.z : pos.z;


      minPoint.x = minPoint.x < pos.x ? minPoint.x : pos.x;
      minPoint.y = minPoint.y < pos.y ? minPoint.y : pos.y;
      minPoint.z = minPoint.z < pos.z ? minPoint.z : pos.z;
    }

    scale.x = static_cast<int>(std::abs(minPoint.x - maxPoint.x));
    scale.y = static_cast<int>(std::abs(minPoint.y - maxPoint.y));
    scale.z = static_cast<int>(std::abs(minPoint.z - maxPoint.z));

    for (auto pos : countingMap) {
      sf::Vector3f normalized(pos.first.x, pos.first.y, pos.first.z);

      normalized.x = (normalized.x - minPoint.x) / scale.x;
      normalized.y = (normalized.y - minPoint.y) / scale.y;
      normalized.z = (normalized.z - minPoint.z) / scale.z;

      normalizedMap.insert(std::make_pair(normalized, pos.second));

      maxCount = maxCount > pos.second ? maxCount : pos.second;
    }

    m_max = maxCount;
    m_minPoint = minPoint;
    m_maxPoint = maxPoint;
    m_scale = scale;

    m_normalizedMap = normalizedMap;
  }

  void
  Heatmap::processInformation(Vector<sf::Vector3i>& info) {
    createHeatmapStats(info);
  }

  void
  Heatmap::showHeatmap() {
    sf::Vector3i minPoint = m_minPoint;
    sf::Vector3i maxPoint = m_maxPoint;

    sf::Vector3f scale = m_scale;

    sf::Vector2u size(0, 0);

    float maxValue = 100.0f;

    int width = 1280;
    int height = 720;

    if (scale.x > scale.y) {
      size.x = width;
      size.y = width * scale.y / scale.x;
    }
    else {
      size.x = height * scale.x / scale.y;
      size.y = height;
    }

    m_window->setSize(size);
    setInfluenceMap(size);
    setAllInfluence();

    sf::Image heatmapImg;
    heatmapImg.create(size.x, size.y, sf::Color::Transparent);


    int j = 0;
    sf::Vector2i pixelPosition;
    sf::Color c;
    uint32 posX = 0;
    uint32 posY = 0;

    sf::Color hottestC;
    hottestC.r = 255;
    hottestC.g = 71;
    hottestC.b = 71;
    hottestC.a = 0.8f * 255;
    sf::Color hotC;
    hotC.r = 255;
    hotC.g = 0;
    hotC.b = 179;
    hotC.a = 0.6f * 255;
    sf::Color coldC;
    coldC.r = 60;
    coldC.g = 88;
    coldC.b = 191;
    coldC.a = 0.4 * 255;
    sf::Color coldestC;
    coldestC.r = 121;
    coldestC.g = 255;
    coldestC.b = 223;
    coldestC.a = 0.3 * 255;
    sf::Color tranC = sf::Color::Transparent;

    maxValue = 0;

    for (auto column : getInfluenceMap()) {
      posY = 0;
      for (auto pos : column) {
        maxValue = std::max(maxValue, pos);
        ++posY;
      }
      ++posX;
    }

    posX = 0;
    posY = 0;

    for (auto column : getInfluenceMap()) {
      posY = 0;
      for (auto pos : column) {
        float normPos = pos / maxValue;
        // Defines the color that should appear at a given point
        // c = BlendColor(sf::Color::Transparent, sf::Color::Red, pos);
        c = pos > 0.75 ? BlendColor(hotC, hottestC, clamp(inverseLerp(0.75, 1.5, pos), 0.0f, 1.0f)) :
          pos > 0.50 ? BlendColor(coldC, hotC, inverseLerp(0.50, 0.75, pos)) :
          pos > 0.25 ? BlendColor(coldestC, coldC, inverseLerp(0.25, 0.50, pos)) :
          BlendColor(tranC, coldestC, inverseLerp(0.00, 0.25, pos));
        //c.a = 100;

        //c.a = 180;
        heatmapImg.setPixel(posX, posY, c);
        ++posY;
      }
      ++posX;
    }

    sf::Texture heatMapTexture;
    heatMapTexture.create(size.x, size.y);
    heatMapTexture.update(heatmapImg);
    sf::Sprite heatMapSprite;
    heatMapSprite.setTexture(heatMapTexture);


    // Debugging
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    m_window->clear();
    m_window->draw(shape);
    m_window->draw(heatMapSprite);
    m_window->display();
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
