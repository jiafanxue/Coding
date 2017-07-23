#ifndef SRC_USERCENTERMODEL_H__
#define SRC_USERCENTERMODEL_H__

#include "DatabaseModel.h"
#include <boost/format.hpp>
#include <string>

template <typename T>
auto CheckIfNull(T t)
{
	return t == nullptr ? "" : t;
}

class UserCenterModel
{
public:
	UserCenterModel(const std::string& name_ = "") :
		m_sUsername(name_),
		m_dbMysql("数据库地址", "数据库用户名", "数据库密码", "数据库表"),
		fmt("SQL语句")
	{
		// empty
	}

	void queryUser()
	{
		if (!m_sUsername.empty()) {
			fmt % m_sUsername;

			m_mSql_row = m_dbMysql.query(fmt.str());

			m_sStudentId = CheckIfNull(m_mSql_row[0]);
			m_sName = CheckIfNull(m_mSql_row[1]);
			m_sType = CheckIfNull(m_mSql_row[2]);
			m_sAdmissionTime = CheckIfNull(m_mSql_row[3]);
			m_sIDcard = CheckIfNull(m_mSql_row[4]);
			m_sSex = CheckIfNull(m_mSql_row[5]);
			m_sBirthday = CheckIfNull(m_mSql_row[6]);
			m_sNativeplace = CheckIfNull(m_mSql_row[7]);
			m_sTelephone = CheckIfNull(m_mSql_row[8]);
			m_sBedroom = CheckIfNull(m_mSql_row[9]);
			m_sQQ = CheckIfNull(m_mSql_row[10]);
		}
	}

	void showUser()
	{
		if (!m_sUsername.empty()) {
			std::cout << "m_sStudentId		:" << m_sStudentId << std::endl;
			std::cout << "m_sName         	:" << m_sName << std::endl;
			std::cout << "m_sType         	:" << m_sType << std::endl;
			std::cout << "m_sAdmissionTime	:" << m_sAdmissionTime << std::endl;
			std::cout << "m_sIDcard       	:" << m_sIDcard << std::endl;
			std::cout << "m_sSex          	:" << m_sSex << std::endl;
			std::cout << "m_sBirthday     	:" << m_sBirthday << std::endl;
			std::cout << "m_sNativeplace  	:" << m_sNativeplace << std::endl;
			std::cout << "m_sTelephone    	:" << m_sTelephone << std::endl;
			std::cout << "m_sBedroom      	:" << m_sBedroom << std::endl;
			std::cout << "m_sQQ           	:" << m_sQQ << std::endl;
			std::cout << std::endl;
		}
	}

	void setUsername(const std::string& username_)
	{
		m_sUsername = username_;
	}

	const std::string Id() const
	{
		return m_sStudentId;
	}

	const std::string Name() const
	{
		return m_sName;
	}

	const std::string Type() const
	{
		return m_sType;
	}

	const std::string Admission() const
	{
		return m_sAdmissionTime;
	}

	const std::string Idcard() const
	{
		return m_sIDcard;
	}

	const std::string Sex() const
	{
		return m_sSex;
	}

	const std::string Birthday() const
	{
		return m_sBirthday;
	}

	const std::string Native() const
	{
		return m_sNativeplace;
	}

	const std::string Telephone() const
	{
		return m_sTelephone;
	}

	const std::string Bedroom() const
	{
		return m_sBedroom;
	}

	const std::string QQ() const
	{
		return m_sQQ;
	}

private:
	std::string m_sUsername;

	std::string m_sStudentId;		// 学号
	std::string m_sName;			// 姓名
	std::string m_sType;			// 学生类别
	std::string m_sAdmissionTime;	// 入学年月
	std::string m_sIDcard;			// 身份号
	std::string m_sSex;				// 性别
	std::string m_sBirthday;		// 出生年月
	std::string m_sNativeplace;		// 籍贯
	std::string m_sTelephone;		// 手机号码
	std::string m_sBedroom;			// 寝室号
	std::string m_sQQ;				// qq号
	DataBaseModel m_dbMysql;		// 数据库
	boost::format fmt;				// 搜索匹配字符
	MYSQL_ROW m_mSql_row;

};

#endif // !SRC_USERCENTERMODEL_H__
