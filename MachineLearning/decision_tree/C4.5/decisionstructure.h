#ifndef DECISIONSTRUCTURE_H__
#define DECISIONSTRUCTURE_H__

#include <iostream>
#include <string>
#include <vector>

// 西瓜个体定义
class Watermelon
{
public:
	std::string id;							// 编号
	std::string color;						// 颜色
	std::string pedicle;					// 根蒂
	std::string sound;						// 声音
	std::string texture;					// 纹理
	std::string umbilical;					// 脐部
	std::string touch;						// 触感
	std::string good;						// 好瓜？

	friend
	std::ostream& operator<<(std::ostream& os, const Watermelon& wm)
	{
		os << wm.id << " " << wm.color << " " << wm.pedicle << " "
		   << wm.sound << " " << wm.texture << " " << wm.umbilical << " "
		   << wm.touch << " " << wm.good;
		os << std::endl;
		return os;
	}
};

struct Node
{
	std::string attribute;
	std::string edgeValue;
	std::vector<Node*> childs;
	Node() : attribute(""), edgeValue("") { }
};

template <typename T>
using Ptr = T*;
using TreeRoot = Ptr<Node>;

#endif // DECISIONSTRUCTURE_H__
