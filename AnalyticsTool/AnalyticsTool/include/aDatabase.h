#pragma once

#include "aUtilities.h"

#include <SFML/System/Vector3.hpp>

namespace Analytics
{
class Database
{
public:
	Database() = default;

	~Database() = default;

  Vector<sf::Vector3i>
	downloadData();

	void
	init();

	int 
	post(String url, String query, String& headerStr, String& responseStr);

	int
	get(String url, String query, String& headerStr, String& responseStr);
};
}

