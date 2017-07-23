#ifndef SRC_DATABASE_DATABASEMODEL_H__
#define SRC_DATABASE_DATABASEMODEL_H__

#include <string>
#include <iostream>
#include <mysql.h>
#include <boost/noncopyable.hpp>

class DataBaseModel : boost::noncopyable
{
public:
	explicit DataBaseModel(
		const std::string& host_,
		const std::string& user_,
		const std::string& pwd_,
		const std::string& database_,
		unsigned int port_ = 3306)
		: m_sHost(host_), m_sUser(user_), m_sPasswd(pwd_), m_sDatabase(database_), m_uiPort(port_)
	{
		initConnect();
	}

	// 由于专门针对的数据库，所以这里做了特别处理
	MYSQL_ROW query(const std::string& query_string)
	{
		MYSQL_RES* result;
		MYSQL_ROW sql_row;

		unsigned int num_fields;
		unsigned int num_rows;

		if (!mysql_query(&m_Mysql, query_string.c_str())) {
			// Call mysql_store_result() and gotten back a result that is not a null pointer
			//
			result = mysql_store_result(&m_Mysql);
			num_rows = mysql_num_rows(result);

			if (result && num_rows >= 1) {
				// Find out how many rows are in the result set.
				//
				num_fields = mysql_num_fields(result);

				if ((sql_row = mysql_fetch_row(result))) {
					mysql_free_result(result);
					return sql_row;
				}
			}
			else {
				if (mysql_errno(&m_Mysql)) {
					std::cerr << "Error: " << mysql_error(&m_Mysql) << std::endl;
				}
				else if (mysql_field_count(&m_Mysql) == 0) {
					// query does not return data
					// (it was not a SELECT)
					num_rows = mysql_affected_rows(&m_Mysql);
					// ...
				}
				else if (num_rows == 0) {
					std::cout << "No data!" << std::endl;
				}
			}
		}
		else {
			std::cout << "query sql failed!" << std::endl;
			return NULL;
		}
	}


private:
	void initConnect()
	{
		mysql_init(&m_Mysql);

		//mysql_options()

		if (!mysql_real_connect(
			&m_Mysql,
			m_sHost.c_str(), m_sUser.c_str(), m_sPasswd.c_str(), m_sDatabase.c_str(),
			m_uiPort, nullptr, 0))
		{
			std::cerr << "Failed to connect to database: Error:	" << mysql_error(&m_Mysql) << std::endl;
			// exception handler
		}
		else {
			// zero for success. Nonzero if an error eccurred.
			if (mysql_query(&m_Mysql, "SET NAMES GBK")) {
				std::cerr << "Failed to query: Error:	" << mysql_error(&m_Mysql) << std::endl;
				// exception handler
			}
		}
	}

private:
	MYSQL m_Mysql;
	std::string m_sHost;
	std::string m_sUser;
	std::string m_sPasswd;
	std::string m_sDatabase;
	unsigned int m_uiPort;
};

#endif // !SRC_DATABASE_DATABASEMODEL_H__
