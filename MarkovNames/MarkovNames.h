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

	MarkovNames(vector<string> original_names, int order, int length);
	MarkovNames(string names, int order, int length);

	void ResetGenerator(vector<string> original_names, int order, int length);

	string GetName();
	vector<string> GetNames(int count);

private:
	
	vector<string> v_generated;
	vector<string> v_samples;
	map<string, vector<char> > m_chains;
	typedef map<string, vector<char> >::iterator ChainsIter;

	int order;
	int min_name_length;

	void ProcessName(string name);
	void Copy(const MarkovNames &mn);
	std::vector<std::string> & split(const std::string &s, char delim, std::vector<std::string> &elems);
	std::vector<std::string> split(const std::string &s, char delim);

	static const int MAX_NAME_ITERATION;
	void ClearVectors();
};


