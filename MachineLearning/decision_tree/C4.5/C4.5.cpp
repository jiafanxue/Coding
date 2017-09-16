/*
* ========================================
* @Author		=	xefvan
* @Remark		=	C4.5
* @Version		=	1.0
* @DateTime		=	2017-09-16 21:31:03
* ========================================
*/

#include "require.h"
#include "decisionstructure.h"
#include <iostream>
#include <string>
#include <vector>

// C4.5决策算法 -- 西瓜决策树
TreeRoot TreeGenerate(TreeRoot pTree,
                      std::vector<Watermelon> datas,					// 训练集
                      std::vector<std::string> attributes,				// 属性集
                      std::map<std::string, std::vector<std::string>> map_attr)
{
	if (belongs_same_label(datas, "yes")) {
		// All samples are positive.
		pTree->attribute = "yes";
		return pTree;
	}
	else if (belongs_same_label(datas, "no")) {
		// All samples are negative.
		pTree->attribute = "no";
		return pTree;
	}
	else {
		if (attributes.empty()) {
			pTree->attribute = majority_of_category(datas);
			return pTree;
		}
		else {
			std::pair<std::string, std::vector<std::string>> optimal_attrs =  \
			        optimal_attribute(datas, attributes, map_attr);
			pTree->attribute = optimal_attrs.first;
			for (auto aptimal_attr : optimal_attrs.second) {
				Node* new_node = new Node();
				new_node->edgeValue = aptimal_attr;

				std::vector<Watermelon> new_datas = remain_watermelon_datas(
				                                        datas, aptimal_attr, optimal_attrs.first);
				if (new_datas.empty()) {
					new_node->attribute = majority_of_category(datas);
					// return pTree;
				}
				else {
					std::vector<std::string> new_attributes;
					for (auto train_attribute : attributes) {
						if (train_attribute.compare(optimal_attrs.first)) {
							new_attributes.push_back(train_attribute);
						}
					}
					TreeGenerate(new_node, new_datas, new_attributes, map_attr);
				}

				pTree->childs.push_back(new_node);
			}
		}
	}

	return pTree;
}

int main(int argc, char const *argv[])
{
	std::vector<Watermelon> datas;
	std::vector<std::string> attributes;
	std::vector<std::string> train_attributes;

	train_attributes.push_back("color");
	train_attributes.push_back("pedicle");
	train_attributes.push_back("sound");
	train_attributes.push_back("texture");
	train_attributes.push_back("umbilical");
	train_attributes.push_back("touch");

	// load the datas and attributes..
	//
	load_file(datas, attributes, "watermelondatas.txt");

	std::map<std::string, std::vector<std::string>> map_attr;

	// match attribute set..
	//
	match_properties(datas, attributes, map_attr);

	TreeRoot pTree = new Node();
	pTree = TreeGenerate(pTree, datas, train_attributes, map_attr);
	print_tree(pTree, 0);


	return 0;
}
