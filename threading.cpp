#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <cstring>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <vector>

void task(int threadID, int input) {
	int key = -1;
	while (key != input) {
		key = rand() % 10000;
	}
	std::string s = "Thread " + std::to_string(threadID);
	s = s + " completed.\n";
	std::cout << s;
	return;
}

int main (int argc, char** argv) {
	if (argc > 2) {
		std::cout << "Input is invalid: too many arguments." << std::endl;
		return 0;
	}
	std::string s(argv[1]);
	for (int i = 0; i < s.length(); i++) {
		if (!isdigit(s[i]) ) {
			std::cout << "Invalid input: argument is not an integer" << std::endl;
			return 0;
		}
	}
	int input = stoi (s);
	if (input < 0 || input > 9999) {
		std::cout << "Invalid input: please select a value between 0 and 9999" << std::endl;
		return 0;
	}
	std::vector<std::thread> threads;
	for (int i = 0; i < 10; i++) {
		threads.push_back(std::thread(task, i, input));
	}
	for (auto& thr : threads) {
		thr.join();
	}
	std::cout << "All threads have finished finding their numbers." << std::endl;
	return 0;
}
