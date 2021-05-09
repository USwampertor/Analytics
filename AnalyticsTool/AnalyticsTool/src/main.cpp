/**
 * 
 */
#include <chrono>
#include <conio.h>
#include <ctime>
#include <iostream>
#include <thread>

#include "aDatabase.h"
#include "aHeatmap.h"

using namespace Analytics;

int main()
{
	Database* d = new Database();

	Heatmap* h = new Heatmap();
	h->init(1280, 720);
	h->startWindow();

  int timesPerformed = 1;

	while (true) {

		if (_kbhit()) { break; }
    system("cls");
    std::cout << "Times performed: " << timesPerformed << std::endl;
    std::cout << "Downloading..." << std::endl;
		d->downloadData();
		std::cout << "Downloaded" << std::endl;
    std::cout << "Waiting 30 seconds for next update" << std::endl;

		std::this_thread::sleep_for(std::chrono::seconds(30));
		++timesPerformed;
	}

	h->closeWindow();
	delete(d);
	delete(h);

	return 0;
}
