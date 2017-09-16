#ifndef REQUIRE_H__
#define REQUIRE_H__

#include "decisionstructure.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <stack>
#include <string>
#include <map>
#include <vector>

inline void assure(std::ifstream& in,
                   const std::string& filename = "")
{
	using namespace std;
	if (!in) {
		fprintf(stderr, "Could not open file: %s\n", filename.c_str());
		exit(1);
	}
}

inline void assure(std::ofstream& out,
                   const std::string& filename = "")
{
	using namespace std;
	if (!out) {
		fprintf(stderr, "Could not open file: %s\n", filename.c_str());
	}
}

void load_file(std::vector<Watermelon>& datas,
               std::vector<std::string>& attributes,
               const std::string& filename)
{
	std::ifstream istrm(filename);
	assure(istrm, filename);

	if (istrm.is_open()) {
		char buffer[128];
		bool firstline = true;
		while (!istrm.eof()) {
			istrm.getline(buffer, 128);
			const char* id = strtok(buffer, " ");
			const char* color = strtok(NULL, " ");
			const char* pedicle = strtok(NULL, " ");
			const char* sound = strtok(NULL, " ");
			const char* texture = strtok(NULL, " ");
			const char* umbilical = strtok(NULL, " ");
			const char* touch = strtok(NULL, " ");
			const char* good = strtok(NULL, " ");

			// Check if the first line.
			// The first line contains attributes of datas.
			//
			if (firstline) {
				attributes.push_back(std::string(id));
				attributes.push_back(std::string(color));
				attributes.push_back(std::string(pedicle));
				attributes.push_back(std::string(sound));
				attributes.push_back(std::string(texture));
				attributes.push_back(std::string(umbilical));
				attributes.push_back(std::string(touch));
				attributes.push_back(std::string(good));
				firstline = false;
			}
			else {
				Watermelon data;
				data.id = std::string(id);
				data.color = std::string(color);
				data.pedicle = std::string(pedicle);
				data.sound = std::string(sound);
				data.texture = std::string(texture);
				data.umbilical = std::string(umbilical);
				data.touch = std::string(touch);
				data.good = std::string(good);
				datas.push_back(data);
			}
		}
	}
}

void match_properties(std::vector<Watermelon> datas,
                      std::vector<std::string> attributes,
                      std::map<std::string, std::vector<std::string>>& map_attr)
{
	int index = 0;
	for (auto attribute : attributes) {
		std::vector<std::string> attrTmp;
		for (auto data : datas) {
			switch (index) {
			case 0:
				if (!attrTmp.empty() &&
				        std::find(attrTmp.begin(), attrTmp.end(), data.id) == attrTmp.end()) {
					attrTmp.push_back(data.id);
				}
				else if (attrTmp.empty()) {
					attrTmp.push_back(data.id);
				}
				break;
			case 1:
				if (!attrTmp.empty() &&
				        std::find(attrTmp.begin(), attrTmp.end(), data.color) == attrTmp.end()) {
					attrTmp.push_back(data.color);
				}
				else if (attrTmp.empty()) {
					attrTmp.push_back(data.color);
				}
				break;
			case 2:
				if (!attrTmp.empty() &&
				        std::find(attrTmp.begin(), attrTmp.end(), data.pedicle) == attrTmp.end()) {
					attrTmp.push_back(data.pedicle);
				}
				else if (attrTmp.empty()) {
					attrTmp.push_back(data.pedicle);
				}
				break;
			case 3:
				if (!attrTmp.empty() &&
				        std::find(attrTmp.begin(), attrTmp.end(), data.sound) == attrTmp.end()) {
					attrTmp.push_back(data.sound);
				}
				else if (attrTmp.empty()) {
					attrTmp.push_back(data.sound);
				}
				break;
			case 4:
				if (!attrTmp.empty() &&
				        std::find(attrTmp.begin(), attrTmp.end(), data.texture) == attrTmp.end()) {
					attrTmp.push_back(data.texture);
				}
				else if (attrTmp.empty()) {
					attrTmp.push_back(data.texture);
				}
				break;
			case 5:
				if (!attrTmp.empty() &&
				        std::find(attrTmp.begin(), attrTmp.end(), data.umbilical) == attrTmp.end()) {
					attrTmp.push_back(data.umbilical);
				}
				else if (attrTmp.empty()) {
					attrTmp.push_back(data.umbilical);
				}
				break;
			case 6:
				if (!attrTmp.empty() &&
				        std::find(attrTmp.begin(), attrTmp.end(), data.touch) == attrTmp.end()) {
					attrTmp.push_back(data.touch);
				}
				else if (attrTmp.empty()) {
					attrTmp.push_back(data.touch);
				}
				break;
			case 7:
				if (!attrTmp.empty() &&
				        std::find(attrTmp.begin(), attrTmp.end(), data.good) == attrTmp.end()) {
					attrTmp.push_back(data.good);
				}
				else if (attrTmp.empty()) {
					attrTmp.push_back(data.good);
				}
				break;
			default:
				break;
			}
		}
		index++;
		map_attr[attribute] = attrTmp;
	}
}

bool belongs_same_label(std::vector<Watermelon> datas,
                        std::string label = "yes")
{
	if (datas.empty()) {
		std::cout << "the datas is empty" << std::endl;
		exit(1);
	}

	for (auto iter = datas.begin(); iter != datas.end(); ++iter) {
		if (iter->good != label) {
			return false;
		}
	}

	return true;
}

std::string majority_of_category(std::vector<Watermelon> datas)
{
	int positivecategory = 0;
	int negativecategory = 0;

	for (auto data : datas) {
		if (data.good == "yes") {
			++positivecategory;
		}
		else if (data.good == "no") {
			++negativecategory;
		}
	}
	return (positivecategory > negativecategory ? "yes" : "no");
}

double calculate_information_entropy(std::vector<Watermelon> datas,
                                     std::string mapAttr = "",
                                     std::string attribute = "good")
{
	// Ent(D) = -∑(k=1, |Y|) p_k * log2(p_k)
	//
	int size = 0;
	double entropy = 0;
	int positive = 0;
	int negative = 0;

	// Beacuse of the datas only have two label.
	// So entropy = positiveSample + negativeSample
	//
	if (attribute == "color") {
		for (auto data : datas) {
			if (data.color == mapAttr) {
				if (data.good == "yes") {
					++positive;
				}
				else {
					++negative;
				}
				++size;
			}
		}
	}
	else if (attribute == "pedicle") {
		for (auto data : datas) {
			if (data.pedicle == mapAttr) {
				if (data.good == "yes") {
					++positive;
				}
				else {
					++negative;
				}
				++size;
			}
		}
	}
	else if (attribute == "sound") {
		for (auto data : datas) {
			if (data.sound == mapAttr) {
				if (data.good == "yes") {
					++positive;
				}
				else {
					++negative;
				}
				++size;
			}
		}
	}
	else if (attribute == "texture") {
		for (auto data : datas) {
			if (data.texture == mapAttr) {
				if (data.good == "yes") {
					++positive;
				}
				else {
					++negative;
				}
				++size;
			}
		}
	}
	else if (attribute == "umbilical") {
		for (auto data : datas) {
			if (data.umbilical == mapAttr) {
				if (data.good == "yes") {
					++positive;
				}
				else {
					++negative;
				}
				++size;
			}
		}
	}
	else if (attribute == "touch") {
		for (auto data : datas) {
			if (data.touch == mapAttr) {
				if (data.good == "yes") {
					++positive;
				}
				else {
					++negative;
				}
				++size;
			}
		}
	}
	else if (attribute == "good") {
		size = datas.size();
		auto judget = [&](Watermelon wm) { if (wm.good == "yes") { ++positive; } else { ++negative; }};
		for_each(datas.begin(), datas.end(), judget);
	}

	if (positive == 0 || negative == 0) {
		return 0;
	}
	else {
		entropy = -(((double)positive / size) * log2((double)positive / size) + ((double)negative / size) * log2((double)negative / size));
	}

	return entropy;
}

double proportion(std::vector<Watermelon>& datas,
                  std::string mapAttr = "",
                  std::string attribute = "")
{
	int size = datas.size();
	double proportion = 0;

	if (attribute == "color") {
		for (auto data : datas) {
			if (data.color == mapAttr) {
				++proportion;
			}
		}
	}
	else if (attribute == "pedicle") {
		for (auto data : datas) {
			if (data.pedicle == mapAttr) {
				++proportion;
			}
		}
	}
	else if (attribute == "sound") {
		for (auto data : datas) {
			if (data.sound == mapAttr) {
				++proportion;
			}
		}
	}
	else if (attribute == "texture") {
		for (auto data : datas) {
			if (data.texture == mapAttr) {
				++proportion;
			}
		}
	}
	else if (attribute == "umbilical") {
		for (auto data : datas) {
			if (data.umbilical == mapAttr) {
				++proportion;
			}
		}
	}
	else if (attribute == "touch") {
		for (auto data : datas) {
			if (data.touch == mapAttr) {
				++proportion;
			}
		}
	}

	proportion /= size;

	return proportion;
}

double calculate_information_gain(std::vector<Watermelon>& datas,
                                  std::string attribute,
                                  std::map<std::string, std::vector<std::string>> map_attr)
{
	// Gain(D, a) = Ent(D) - ∑(v=1, V) |D^|/|D| * Ent(D)
	//
	double gain = calculate_information_entropy(datas);
	std::vector<std::string> attrs = map_attr[attribute];

	for (auto attr : attrs) {
		gain -= proportion(datas, attr, attribute) * calculate_information_entropy(datas, attr, attribute);
	}

	return gain;
}

double calculate_information_gain_ratio(std::vector<Watermelon>& datas,
                                        std::string attribute,
                                        std::map<std::string, std::vector<std::string>> map_attr)
{
	// Gain_ratio(D, a) = Gain(D, a) / IV(a)
	//
	double gain = calculate_information_gain(datas, attribute, map_attr);

	double iv = 0;
	std::vector<std::string> attrs = map_attr[attribute];
	for (auto attr : attrs) {
		iv -= proportion(datas, attr, attribute) * log2(proportion(datas, attr, attribute));
	}

	double gain_ratio = gain / iv;

	return gain_ratio;
}

std::pair<std::string, std::vector<std::string>> optimal_attribute(std::vector<Watermelon>& datas,
        std::vector<std::string>& attributes,
        std::map<std::string, std::vector<std::string>> map_attr)
{
	std::map<std::string, double> map_gains;
	std::map<std::string, double> map_gains_ratio;
	for (auto attribute : attributes) {
		map_gains[attribute] = calculate_information_gain(datas, attribute, map_attr);
		map_gains_ratio[attribute] = calculate_information_gain_ratio(datas, attribute, map_attr);
	}

	// Sort the information gain and select the attribute of the maximum
	// information gain.The biggest value is in the first.
	//
	std::vector<std::pair<std::string, double>> vec_map_gains(map_gains.begin(), map_gains.end());
	std::vector<std::pair<std::string, double>> vec_map_gains_ratios(map_gains_ratio.begin(), map_gains_ratio.end());
	auto compare_x_y = [](const std::pair<std::string, double> x, const std::pair<std::string, double> y) {
		return x.second > y.second;
	};
	std::sort(vec_map_gains.begin(), vec_map_gains.end(), compare_x_y);
	std::sort(vec_map_gains_ratios.begin(), vec_map_gains_ratios.end(), compare_x_y);

	// Find information gain greater than average.
	//
	std::vector<std::string> vec_map_gains_name;
	int vec_map_gains_size = vec_map_gains.size() / 2;
	for (int i = 0; i < vec_map_gains_size; ++i) {
		vec_map_gains_name.push_back(vec_map_gains[i].first);
	}

	std::string best_attribute;
	for (auto vec_map_gains_ratio : vec_map_gains_ratios) {
		if (std::find(vec_map_gains_name.begin(), vec_map_gains_name.end(), vec_map_gains_ratio.first)
		        != vec_map_gains_name.end()) {
			best_attribute = vec_map_gains_ratio.first;
			break;
		}
	}

	if (!best_attribute.empty()) {
		auto search = map_attr.find(best_attribute);
		if (search != map_attr.end()) {
			return std::make_pair(search->first, search->second);
		}
		else {
			return std::make_pair(std::string(""), std::vector<std::string>());
		}
	}
	else {
		return std::make_pair(std::string(""), std::vector<std::string>());
	}
}

std::vector<Watermelon> remain_watermelon_datas(std::vector<Watermelon> datas,
        std::string mapAttr,
        std::string attribute)
{
	std::vector<Watermelon> tmp;

	if (attribute == "color") {
		for (auto data : datas) {
			if (data.color == mapAttr) {
				tmp.push_back(data);
			}
		}
	}
	else if (attribute == "pedicle") {
		for (auto data : datas) {
			if (data.pedicle == mapAttr) {
				tmp.push_back(data);
			}
		}
	}
	else if (attribute == "sound") {
		for (auto data : datas) {
			if (data.sound == mapAttr) {
				tmp.push_back(data);
			}
		}
	}
	else if (attribute == "texture") {
		for (auto data : datas) {
			if (data.texture == mapAttr) {
				tmp.push_back(data);
			}
		}
	}
	else if (attribute == "umbilical") {
		for (auto data : datas) {
			if (data.umbilical == mapAttr) {
				tmp.push_back(data);
			}
		}
	}
	else if (attribute == "touch") {
		for (auto data : datas) {
			if (data.touch == mapAttr) {
				tmp.push_back(data);
			}
		}
	}

	return tmp;
}

void print_tree(TreeRoot pTree, int depth)
{
	for (int i = 0; i < depth; ++i) {
		std::cout << '\t';
	}

	if (!pTree->edgeValue.empty()) {
		std::cout << "--" << pTree->edgeValue << "--" << std::endl;
		for (int i = 0 ; i < depth; ++i) {
			std::cout << '\t';
		}
	}

	std::cout << pTree->attribute << std::endl;
	for (auto child : pTree->childs) {
		print_tree(child, depth + 1);
	}
}

#endif // REQUIRE_H__
