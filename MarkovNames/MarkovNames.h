#pragma once

#include <vector>
#include <map>
#include <string>

using namespace std;

class MarkovNames {
public:
	MarkovNames(void);
	~MarkovNames(void);
	MarkovNames(const MarkovNames &mn);
	MarkovNames & operator=(const MarkovNames &mn);

	MarkovNames(vector<string> original_names, int order);
	MarkovNames(string names, int order);

	void ResetGenerator(vector<string> original_names, int order);

	string GetName();
	vector<string> GetNames(int count);

private:
	
	vector<string> v_keys;
	map<string, vector<char> > m_chains;
	typedef map<string, vector<char> >::iterator ChainsIter;

	int order;
	int prev_start_index;

	void ProcessName(string name);
	void Copy(const MarkovNames &mn);
	std::vector<std::string> & split(const std::string &s, char delim, std::vector<std::string> &elems);
	std::vector<std::string> split(const std::string &s, char delim);
};


