#include <iostream>
#include <filesystem>
#include <string>
#include <ctime>
#include <fstream>
#include <chrono>
#include <vector>
#include <sstream>
#include <iomanip>
#include <map>
#include <algorithm>

int hash_by_crc(const std::string &str);
int hash_by_pjw(const std::string &str);
int hash_by_buz(const std::string &str);
int hash_by_cpp(const std::string &str);
int find_duplicates(const std::vector<std::string>& filenames, int(*hashfunc)(const std::string&));
void get_filenames(const std::string& path, std::vector<std::string> &filenames);
void fill_map();


int(*hashes[])(const std::string&) = { hash_by_crc, hash_by_pjw, hash_by_buz, hash_by_cpp };
static std::map<char, int> r;

int main() {
	const std::string func_names[4] = {"Hash by CRC", "Hash by PJW", "Hash by BUZ", "Hash by CPP"};
	const std::string path = "out";
	
	std::vector<std::string> filenames;
	get_filenames(path, filenames);
	fill_map();

	for (int i = 0; i < (int) (sizeof(hashes) / sizeof(hashes[0])) && 
			i < (int) (sizeof(func_names) / sizeof(func_names[0])); i++) {
		auto start = std::chrono::high_resolution_clock::now();
		int counter_files = find_duplicates(filenames, hashes[i]);
		auto finish = std::chrono::high_resolution_clock::now();
		std::chrono::duration time = finish - start;
		std::cout << func_names[i] << " " << std::setw(5) <<
			std::chrono::duration_cast<std::chrono::milliseconds>(time).count() << " ms" << 
			"\tRepeat files: " << counter_files << std::endl;
	}
	return 0;
}

void fill_map() {
	srand(time(NULL));
	for (int i = 'A'; i < 'z'; i++)
		r[i] = rand();
}

void get_filenames(const std::string& path, std::vector<std::string>& filenames) {
	for (const auto& entry : std::filesystem::directory_iterator(path))
		if (entry.is_regular_file())
			filenames.push_back(entry.path().string());
}

int find_duplicates(const std::vector<std::string>& filenames, int(*hashfunc)(const std::string&)) {
	std::string path = "out";
	std::vector<int> hashes;
	int counter_repeats = 0;
	for (auto filename : filenames) {
		std::ifstream file(filename);
		if (!file.is_open())
			return -1;
		std::stringstream buf;
		buf << file.rdbuf();
		int h = hashfunc(buf.str());
		if (std::find(hashes.begin(), hashes.end(), h) != hashes.end())
			counter_repeats++;
		else
			hashes.push_back(h);
		file.close();
	}
	return counter_repeats;
}

int hash_by_crc(const std::string &str) {
	int h = 0;
	for (int i = 0; i < (int) str.length(); i++) {
		int highorder = h & 0xf8000000;
		h = h << 5;
		h = h ^ (highorder >> 27);
		h = h ^ str[i];
	}
	return h;
}

int hash_by_pjw(const std::string &str) {
	int h = 0;
	for (int i = 0; i < (int) str.length(); i++) {
		h = (h << 4) + str[i];
		int g = h & 0xf0000000;
		if (g != 0) {
			h = h ^ (g >> 24);
			h = h ^ g;
		}
	}
	return h;
}

int hash_by_buz(const std::string &str) {
	int h = 0;
	for (int i = 0; i < (int) str.length(); i++) {
		int highorder = h & 0x80000000;
		h = h << 1;
		h = h ^ (highorder >> 31);
		h = h ^ r[str[i]];
	}
	return h;
}

int hash_by_cpp(const std::string &str) {
	return std::hash<std::string>{}(str);
}