#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

bool isIsogram(const std::string& word)
{
	std::unordered_set<char> hash;
	for (auto c : word)
		if (!hash.insert(c).second || c == ' ')
			return false;
	return true;
}

bool compareString(const std::string& lhs, const std::string& rhs)
{
	if (lhs.size() < rhs.size()) return true;
	if (rhs.size() < lhs.size()) return false;
	if (lhs < rhs) return true;
	if (rhs < lhs) return false;
	return false;
}

std::vector<std::string> extractFile(const std::string& fileName, const unsigned length)
{
	std::vector<std::string> list;
	std::ifstream file(fileName);
	if (file.is_open())
	{
		for (std::string line; file >> line;)
		{
			std::transform(line.begin(), line.end(), line.begin(), tolower);
			if (isIsogram(line) && line.size() >= length)
				list.push_back(line);
		}
	}
	else
		throw 2;
	file.close();
	return list;
}

void outputFile(const std::vector<std::string>& vec, const std::string& fileName)
{
	std::ofstream file(fileName);
	if (file.is_open())
	{
		for (auto str : vec)
			file << str << std::endl;
	}
	else
		throw 3;
	file.close();
}

void process(const std::string& inFileName, const unsigned wordLength = 4U, const std::string& outFileName = "isograms.txt")
{
	std::cout << "Extracting isograms from the word list..." << std::endl;
	std::vector<std::string>vec = extractFile(inFileName, wordLength);
	std::cout << "Sorting the list..." << std::endl;
	std::sort(vec.begin(), vec.end(), compareString);
	std::cout << "Writing to file..." << std::endl;
	outputFile(vec, outFileName);
	std::cout << "Finished!" << std::endl;
}

int main(int argc, char** argv)
{
	try
	{
		if (argc < 2)
		{
			std::cerr << "Too few arguments\n";
			std::cerr << "Useage: Isogrammer.exe [inputfile] OPTIONAL: [wordlength] [outputfile]\n";
			std::cerr << "Example: Isogrammer.exe wordlist.txt 4 isograms.txt" << std::endl;
			return EXIT_FAILURE;
		}
		if (argc > 4)
		{
			std::cerr << "Too many arguments\n";
			std::cerr << "Useage: Isogrammer.exe [inputfile] OPTIONAL: [wordlength] [outputfile]\n";
			std::cerr << "Example: Isogrammer.exe wordlist.txt 4 isograms.txt" << std::endl;
			return EXIT_FAILURE;
		}
		std::ifstream inFile(argv[1]);
		if (!inFile.is_open())
		{
			std::cerr << "Error opening file " << argv[1] << std::endl;
			return EXIT_FAILURE;
		}
		if (argc == 2)
			process(argv[1]);
		if (argc == 3)
			process(argv[1], std::stoi(argv[2]));
		if (argc == 4)
			process(argv[1], std::stoi(argv[2]), argv[3]);
	}
	catch (std::exception& e)
	{
		std::cerr << "Process failed: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}
}
