#include <iostream>
#include <filesystem>
#include <string>
#include <ctime>
#include <fstream>
#include <chrono>
#include <vector>

#include <map>
#include <conio.h>

int R(char ki);
int hash_by_crc(std::string str);
int hash_by_pjw(std::string str);
int hash_by_buz(std::string str);
int find_duplicates(int(*hashfunc)(std::string));

int(*hashes[])(std::string) = { hash_by_crc, hash_by_pjw, hash_by_buz };

int main() {
	for (int i = 0; i < sizeof(hashes) / sizeof(hashes[0]); i++) {
		auto start = std::chrono::high_resolution_clock::now();
		find_duplicates(hashes[i]);
		auto finish = std::chrono::high_resolution_clock::now();
		std::chrono::duration time = finish - start;
		std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(time).count() << " ms" << std::endl;
	}
	return 1;
}

int find_duplicates(int(*hashfunc)(std::string)) {
	std::string path = "out";
	std::vector<int> hashes;
	int counter_repeats = 0;
	for (const auto& entry : std::filesystem::directory_iterator(path))
	{
		if (entry.is_regular_file()) {
			std::ifstream file(entry.path());
			if (!file.is_open())
				return -1;
			while (!file.fail()) {
				std::string word;
				file >> word;
				int h = hashfunc(word);
				if (std::find(hashes.begin(), hashes.end(), h) == hashes.end())
					counter_repeats++;
				else
					hashes.push_back(h);
			}
			file.close();
		}
	}
	return counter_repeats;
}

int R(char ki) {
	srand(time(NULL));
	return rand() * (int) ki;
}

int hash_by_crc(std::string str) {
	int h = 0;
	for (int i = 0; i < str.length(); i++) {
		int highorder = h & 0xf8000000;
		h = h << 5;
		h = h ^ (highorder >> 27);
		h = h ^ str[i];
	}
	return h;
}

int hash_by_pjw(std::string str) {
	int h = 0;
	for (int i = 0; i < str.length(); i++) {
		h = (h << 4) + str[i];
		int g = h & 0xf0000000;
		if (g != 0) {
			h = h ^ (g >> 24);
			h = h ^ g;
		}
	}
	return h;
}

int hash_by_buz(std::string str) {
	int h = 0;
	for (int i = 0; i < str.length(); i++) {
		int highorder = h & 0x80000000;
		h = h << 1;
		h = h ^ (highorder >> 31);
		h = h ^ R(str[i]);
	}
	return h;
}
