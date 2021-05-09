#pragma once

#include "aUtilities.h"

namespace Analytics
{
class Database
{
public:
	Database() = default;

	~Database() = default;

	bool
	downloadData();

	void
	init();

	int 
	post(String url, String query, String& headerStr, String& responseStr);

	int
	get(String url, String query, String& headerStr, String& responseStr);
};
}

