#include "MarkovNames.h"
#include <cassert>
#include <ctime>
#include <sstream>
#include <algorithm>

const int MarkovNames::MAX_NAME_ITERATION = 100;

MarkovNames::MarkovNames(void) {
	order = 0;
}

MarkovNames::~MarkovNames(void) {
	ClearVectors();
}

MarkovNames::MarkovNames(const MarkovNames &mn) {
	this->Copy(mn);
}

MarkovNames & MarkovNames::operator=(const MarkovNames &mn) {
	if(this != &mn){
		this->~MarkovNames();

		this->Copy(mn);
	}
	return *this;
}

void MarkovNames::Copy(const MarkovNames &mn){
	order = mn.order;
	v_samples = mn.v_samples;
	m_chains = mn.m_chains;
}

MarkovNames::MarkovNames(vector<string> original_names, int order, int length) {
	assert(order > 0);
	assert(original_names.size() > 0);	
	srand(time(NULL));
	this->order = order;
	this->min_name_length = length;

	for each (string s in original_names) {
		transform(s.begin(), s.end(), s.begin(), ::tolower);
		ProcessName(s);
	}
}

MarkovNames::MarkovNames(string original_names, int order, int length) {
	assert(order > 0);	
	srand(time(NULL));
	this->order = order;
	this->min_name_length = length;

	for each (string s in split(original_names, ',')) {
		transform(s.begin(), s.end(), s.begin(), ::tolower);
		ProcessName(s);
	}
}

void MarkovNames::ResetGenerator(vector<string> original_names, int order, int length) {
	assert(order > 0);
	assert(original_names.size() > 0);

	srand(time(NULL));
	this->order = order;
	this->min_name_length = length;

	for each (string name in original_names) {
		ProcessName(name);
	}
}

string MarkovNames::GetName() {
	assert(m_chains.size() > 0);
	string name;
	do{
		int n = rand() % v_samples.size();
		int name_length = v_samples[n].size();

		int substring_start = v_samples[n].length() == order ? 0 : rand() % (v_samples[n].length() - order);
		
		name = v_samples[n].substr(substring_start, order);
		if(name[0] == ' ')
			continue;

		ChainsIter it = m_chains.find(name);
		char next_char;
		int iterations = 0;

		while (name.length() < name_length) {
			iterations++;
			next_char = it->second[rand() % it->second.size()];

			if(next_char != '\n'){
				name.append(1, next_char);

				string segment = name.substr(name.length() - order);
				it = m_chains.find(segment);
			}else{
				break;
			}
		}
	}while(find(v_generated.begin(), v_generated.end(), name) != v_generated.end() || name.length() < min_name_length);
	v_generated.push_back(name);

	transform(name.begin(), name.begin() + 1, name.begin(), ::toupper);

	return name;
}

vector<string> MarkovNames::GetNames(int count) {
	return vector<string>();
}

void MarkovNames::ProcessName(string name){
	if(name.length() < order)
		return;

	for(int i = 0; i <= name.length() - order; i++){
		string segment = name.substr(i, order);
		char next_char = i == name.length() - order ? '\n' : name[i+order];

		ChainsIter it = m_chains.find(segment);
		if(it != m_chains.end()){
			it->second.push_back(next_char);
		}else{
			vector<char> v(1, next_char);
			m_chains[segment] = v;
		}
	}
	v_samples.push_back(name);
}

std::vector<std::string> & MarkovNames::split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while(std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

std::vector<std::string> MarkovNames::split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

void MarkovNames::ClearVectors(){
	v_samples.clear();
	for each (pair<string, vector<char> > p in m_chains) {
		p.second.clear();
	}
	m_chains.clear();
	v_generated.clear();
}