#include "MarkovNames.h"
#include <cassert>
#include <ctime>
#include <sstream>

const int MarkovNames::MAX_NAME_ITERATION = 100;

MarkovNames::MarkovNames(void) {
	prev_start_index = 0;
	order = 0;
}

MarkovNames::~MarkovNames(void) {
	v_keys.clear();
	for each (pair<string, vector<char> > p in m_chains) {
		p.second.clear();
	}
	m_chains.clear();
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
	v_keys = mn.v_keys;
	m_chains = mn.m_chains;
	prev_start_index = mn.prev_start_index;
}

MarkovNames::MarkovNames(vector<string> original_names, int order, int length) {
	assert(order > 0);
	assert(original_names.size() > 0);	
	srand(time(NULL));
	this->order = order;
	this->min_name_length = length;

	for each (string name in original_names) {
		ProcessName(name);
	}
}

MarkovNames::MarkovNames(string original_names, int order, int length) {
	assert(order > 0);	
	srand(time(NULL));
	this->order = order;
	this->min_name_length = length;

	for each (string s in split(original_names, ',')) {
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
	int keys_map_size = v_keys.size();
	int first_key_index = rand() % keys_map_size;
	if(keys_map_size > 1){
		while (first_key_index == prev_start_index)	{
			first_key_index = rand() % keys_map_size;
		}
	}
	prev_start_index = first_key_index;

	string name = v_keys[rand() % v_keys.size()];

	ChainsIter it = m_chains.find(name);
	char next_char;
	int iterations = 0;

	do {
		iterations++;
		next_char = it->second[rand() % it->second.size()];

		if(next_char != '\n'){
			name.append(1, next_char);

			string segment = name.substr(name.length() - order);
			it = m_chains.find(segment);
		}else{
			if(iterations < MAX_NAME_ITERATION){
				next_char = 'a';
			}
		}
	} while (next_char != '\n');
	
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
			v_keys.push_back(segment);
		}
	}
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
