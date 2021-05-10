
#pragma once

#include <SFML/System/Vector3.hpp>
#include <SFML/Graphics.hpp>

#include "aUtilities.h"

namespace Analytics {

struct cmpVector3i {
  bool operator()(const sf::Vector3i& lhs, const sf::Vector3i& rhs) const {
    return lhs.x < rhs.x
           || (lhs.x == rhs.x && (lhs.y < rhs.y
           || (lhs.y == rhs.y && lhs.z < rhs.z)));
  }
};

struct cmpVector3f {
  bool operator()(const sf::Vector3f& lhs, const sf::Vector3f& rhs) const {
    return lhs.x < rhs.x
           || (lhs.x == rhs.x && (lhs.y < rhs.y
           || (lhs.y == rhs.y && lhs.z < rhs.z)));
  }
};

class Heatmap
{
 public:
  Heatmap() = default;

  ~Heatmap() = default;

  bool 
  init(int width, int height, float decay = 0.035f, float momentum = 0.80f);

  bool
  startWindow();

  void
  renderWindow();

  void
  closeWindow();

  Vector<Vector<float>>
  getInfluenceMap() const;

  float
  getInfluence(sf::Vector2i pos);

  float
  getInfluence(float x, float y);

  void
  setInfluence(sf::Vector2i pos, float influence);

  void
  setInfluence(float x, float y, float influence);

  void
  setInfluenceMap(sf::Vector2u mapSize);

  void
  calculateInfluence(sf::Vector2i pos, sf::Vector2u mapSize, float& maxInf, float& minInf);

  void
  calculateInfluence(int x, int y, sf::Vector2u mapSize, float& maxInf, float& minInf);

  void
  propagateInfluence(sf::Vector2i pos, sf::Vector2u mapSize);

  void
  bsfPropagate(sf::Vector2i pos, sf::Vector2u mapSize);

  void
  setAllInfluence();

  sf::Sprite
  getHeatmapSprite(sf::Vector2u mapSize);

  Vector<sf::CircleShape>
  getCircleMap();

  void
  setMomentum(float momentum);

  void
  setDecay(float decay);

  void
  processInformation(Vector<sf::Vector3i>& info);

  void
  createHeatmapStats(Vector<sf::Vector3i>& info);

  void
  showHeatmap();

  sf::Color
  BlendColor(sf::Color color1, sf::Color color2, float fraction);

 private:

  //Database section

  sf::Vector3f m_scale;

  int m_max;

  sf::Vector3i m_minPoint;

  sf::Vector3i m_maxPoint;
  
  Map<sf::Vector3f, int, cmpVector3f> m_normalizedMap;

  // Heatmap section

  Vector<Vector<float>> m_influenceMap;

  float m_decay;

  float m_momentum;

  sf::Vector2i m_size;

  sf::RenderWindow* m_window;
};
}
