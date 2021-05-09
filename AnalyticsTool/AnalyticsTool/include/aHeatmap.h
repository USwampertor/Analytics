
#pragma once

#include <SFML/Graphics.hpp>

#include "aUtilities.h"

namespace Analytics {
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
  createHeatmapStats();

  void
  showHeatmap();

  /**
   * @brief Blends a color based on a value of influence (as a LERP but for colors)
   * @param sf::Color color1 the start color
   * @param sf::Color color2 the end color
   * @param float fraction the influence in the final color
   * @return sf::Color the blend of both colors
   *
   */
  sf::Color
  BlendColor(sf::Color color1, sf::Color color2, float fraction);

 private:

  Vector<Vector<float>> influenceMap;

  float m_decay;

  float m_momentum;

  sf::Vector2i m_size;

  sf::RenderWindow* m_window;
};
}
