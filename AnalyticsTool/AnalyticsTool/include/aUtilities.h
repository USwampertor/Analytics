#pragma once

#include <array>
#include <fstream>
#include <map>
#include <string>
#include <vector>

namespace Analytics
{

  // Using simplifications
  using uint64 = std::uint64_t;
  using uint32 = std::uint32_t;
  //using SIZE_T = size_t;

  template<typename KEY,
    typename T,
    typename B = std::less<KEY>,
    typename A = std::allocator<std::pair<const KEY, T>>>
    using Map = std::map<KEY, T, B, A>;

  template <typename T>
  using Vector = std::vector<T>;

  using Ofstream = std::ofstream;
  using Ifstream = std::ifstream;

  using String = std::string;


  /**
   * @brief Sorts a pair by the second value
   * @param const std::pait<A, T>& a the first value
   * @param const std::pait<A, T>& b the second value
   * @return
   *
   */
  template <typename A, typename T>
  bool sortByVal(const std::pair<A, T>& a,
    const std::pair<A, T>& b) {
    return (a.second < b.second);
  }

  /**
   * @brief Inverse lerp function
   * @param float a start
   * @param float b finish
   * @param float v the value to check on what percentage is between a and b
   * @return the normalized value of v on a and b
   *
   */
  static float
  inverseLerp(float a, float b, float v) { return (v - a) / (b - a); }

  /**
   * @brief Parses a line of a csv and spits out
   *        all the information on a Vector<int>
   *        that is separated by a specified string ("," or maybe "->", etc)
   * @param std::string line the line to parse
   * @param std::string separator the separator for the line
   * @return Vector<int> with the int values
   *
   */
  static Vector<int>
  parseLinetoInt(std::string line, std::string separator) {
    size_t pos = 0;
    String token;
    String csvCopy = line;

    Vector<int> v;

    while ((pos = csvCopy.find(separator)) != String::npos) {
      token = csvCopy.substr(0, pos);

      v.push_back(std::stoi(token));

      csvCopy.erase(0, pos + separator.size());
    }
    v.push_back(std::stoi(csvCopy));
    return v;
  }

  /**
   * @brief Parses a line of a csv and spits out
   *        all the information on a Vector<float>
   *        that is separated by a specified string ("," or maybe "->", etc)
   * @param std::string line the line to parse
   * @param std::string separator the separator for the line
   * @return Vector<float> with the float values
   *
   */
  static Vector<float>
  parseLinetoFloat(std::string line, std::string separator) {
    size_t pos = 0;
    String token;
    String csvCopy = line;

    Vector<float> v;

    while ((pos = csvCopy.find(",")) != String::npos) {
      token = csvCopy.substr(0, pos);

      v.push_back(std::stof(token));

      csvCopy.erase(0, pos + 1);
    }
    v.push_back(std::stof(csvCopy));

    return v;
  }

  /**
   * @brief creates a lerp between to given values and a scale
   * @param first point a
   * @param second point b
   * @param scale of lerp
   * @return lerp value
   *
   */
  template<typename T>
  static T
  lerp(T a, T b, float scale) {
    return static_cast<T>(static_cast<float>(a) +
      (static_cast<float>(b) - static_cast<float>(a)) *
      scale);
  }

  /**
   * @brief clamps the value between two given values
   * @param value to clamp
   * @param top value
   * @param bottom value
   * @return highest value
   *
   */
  template<typename T>
  static T
  clamp(T number, T bot, T top) {
    return std::min(std::max(number, bot), top);
  }

  /**
   * @brief Formats a string like in printf
   * @param String with the printf format
   * @param variables to use in the printf
   * @return String with values formatted
   *
   */
  template<typename ... Args>
  std::string stringFormat(const std::string& format, Args ... args)
  {
    size_t size = snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
    if (size <= 0) { throw std::runtime_error("Error during formatting."); }
    std::unique_ptr<char[]> buf(new char[size]);
    snprintf(buf.get(), size, format.c_str(), args ...);
    return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
  }
}
