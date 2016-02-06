#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <unordered_set>
#include <vector>

bool isIsogram(const std::string& word)
{
	std::unordered_set<char> hash;
	for (auto c : word)
		if (!hash.insert(c).second || c == ' ') // add c == '-' || c == '\'' if you don't want apostrophes or hypens
			return false;
	return true;
}

struct compare
{
	bool operator()(const std::string& lhs, const std::string& rhs) const
	{
		if (lhs.size() < rhs.size()) return true;
		if (rhs.size() < lhs.size()) return false;
		if (lhs < rhs) return true;
		if (rhs < lhs) return false;
		return false;
	}
};

std::set<std::string, compare> extractFile(const std::string& fileName, const unsigned min, const unsigned max)
{
	std::set<std::string, compare> list;
	std::ifstream file(fileName);
	if (file.is_open())
	{
		for (std::string line; file >> line;)
		{
			std::transform(line.begin(), line.end(), line.begin(), tolower);
			line.erase(std::remove_if(line.begin(), line.end(), [](auto c) { return !isalpha(c) && c != '\'' && c!= '-'; }), line.end()); //keeps hypens and apostrophes
			if (!line.empty() && line.back() == '-')
				line.pop_back(); //remove trailing hyphens
			if (isIsogram(line) && min <= line.size() && line.size() <= max)
				list.insert(line);
		}
	}
	else
		throw std::runtime_error("Couldn't read from file");
	file.close();
	return list;
}

void outputFile(const std::set<std::string, compare>& vec, const std::string& fileName)
{
	std::ofstream file(fileName);
	if (file.is_open())
	{
		for (auto str : vec)
			file << str << std::endl;
	}
	else
		throw std::runtime_error("Couldn't write to file");
	file.close();
}

void process(const std::string& inFileName, const unsigned min = 4U, const unsigned max = 26U, const std::string& outFileName = "isograms.txt")
{
	std::cout << "Extracting isograms from the word list..." << std::endl;
	std::set<std::string, compare>vec = extractFile(inFileName, min, max);
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
			std::cerr << "Useage: Isogrammer.exe [inputfile] OPTIONAL: [minlength] [maxlength] [outputfile]\n";
			std::cerr << "Example: Isogrammer.exe wordlist.txt 4 7 isograms.txt" << std::endl;
			return EXIT_FAILURE;
		}
		if (argc > 5)
		{
			std::cerr << "Too many arguments\n";
			std::cerr << "Useage: Isogrammer.exe [inputfile] OPTIONAL: [minlength] [maxlength] [outputfile]\n";
			std::cerr << "Example: Isogrammer.exe wordlist.txt 4 7 isograms.txt" << std::endl;
			return EXIT_FAILURE;
		}
		if (argc == 2)
			process(argv[1]);
		if (argc == 3)
			process(argv[1], std::stoi(argv[2]));
		if (argc == 4)
			process(argv[1], std::stoi(argv[2]), std::stoi(argv[3]));
		if (argc == 5)
			process(argv[1], std::stoi(argv[2]), std::stoi(argv[3]), argv[4]);
	}
	catch (std::exception& e)
	{
		std::cerr << "Process failed: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
