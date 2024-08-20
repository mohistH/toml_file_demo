#include <string>
#include <iostream>
#include <toml>
#include <unordered_map>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/statfs.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#endif /// 


/// @brief executable path
/// @return to get the app's executable path 
static std::string appPath()
{
#ifdef _WIN32
	char path[1024 * 6] = { 0 };
	::GetModuleFileName(NULL, path, 255);
	(strrchr(path, '\\'))[1] = 0;

	return std::string(path);
#else
	char* p = NULL;

	const int len = 1024 * 8;
	/// to keep the absolute path of executable's path
	char arr_tmp[len] = { 0 };

	int n = readlink("/proc/self/exe", arr_tmp, len);
	if (NULL != (p = strrchr(arr_tmp, '/')))
		*p = '\0';
	else
	{
		return std::string("");
	}

	return std::string(arr_tmp);
#endif /// 
}


int main(int argc, char** argv, char** envp)
{
	/// 1. write key-value pairs
	///  key="name", value="oct"
	///  key="is famel", value=false
	{
		toml::table tbl{};
		oct::tableNodeInsert<std::string>(tbl, "name", {"oct"});
		oct::tableNodeInsert<int64_t>(tbl, "age", 16);
		oct::tableNodeInsert<bool>(tbl, "is_famel", false);
		oct::tableNodeInsert<double>(tbl, "score", 99.99);
	
		/// 设置将要生成的文件
		std::string tomlFile = appPath() + "/1.toml";
		std::ofstream file(tomlFile, std::ios::out | std::ios::trunc);
		if (false == file.is_open())
		{
			std::cout << "\n failed to open the file=" << tomlFile << "\n\n";
		}
		else 
		{
			/// 写入文件
			file << tbl;
			file.close();
		}
	}
	/// 2. write hex 、oct and bin
	{
		toml::table tbl{};

		/// 将 10进制的12按照 binary、oct and hex 写入
		int64_t writeValue {12};

		toml::value<int64_t> binaryValue(writeValue);		
		binaryValue.flags(toml::value_flags::format_as_binary);

		toml::value<int64_t> hexValue(writeValue);
		hexValue.flags(toml::value_flags::format_as_hexadecimal);

		toml::value<int64_t> octValue(writeValue);
		octValue.flags(toml::value_flags::format_as_octal);

		tbl.insert_or_assign("binary", binaryValue);
		tbl.insert_or_assign("hex", hexValue);
		tbl.insert_or_assign("oct", octValue);

		/// 设置将要生成的文件
		std::string tomlFile = appPath() + "/2.toml";
		std::ofstream file(tomlFile, std::ios::out | std::ios::trunc);
		if (false == file.is_open())
		{
			std::cout << "\n failed to open the file=" << tomlFile << "\n\n";
		}
		else 
		{
			/// 写入文件
			file << tbl;
			file.close();
		}
	}
	/// 3. 
	{
		/// target toml file content 
		/// 	[TOML_DEMO]
		/// 	bool = false
		/// 	integer = 1234567890
		/// 	string = 'this is a string'
		

		toml::table rootNode{};
		/// 参数1-哪个表，参数2-节点的key, 参数3-将要插入的值
		oct::tableNodeInsert<std::string>(rootNode, "string", "this is a string");
		oct::tableNodeInsert<bool>(rootNode, "bool", false);
		oct::tableNodeInsert<int64_t>(rootNode, "integer", 1234567890);

		toml::table tbl{};
		/// 创建[NET_INTERFACE]
		tbl.insert_or_assign("TOML_DEMO", rootNode);

		/// 设置将要生成的文件
		std::string tomlFile = appPath() + "/3.toml";
		std::ofstream file(tomlFile, std::ios::out | std::ios::trunc);
		if (false == file.is_open())
		{
			std::cout << "\n failed to open the file=" << tomlFile << "\n\n";
		}
		else 
		{
			/// 写入文件
			file << tbl;
			file.close();
		}
	}
	/// 4. 
	{
		/// target toml file content

		/// [TOML_DEMO]
		/// bool = false
		/// integer = 1234567890
		/// string = 'this is a string'

		/// [[fruit]]
		/// kilograms = 2
		/// name = 'banana'

		/// [[fruit]]
		/// kilograms = 1
		/// name = 'apple'

		/// [[fruit]]
		/// kilograms = 3
		/// name = 'blueberry'


		toml::table rootNode{};
		oct::tableNodeInsert<std::string>(rootNode, "string", "this is a string");
		oct::tableNodeInsert<bool>(rootNode, "bool", false);
		oct::tableNodeInsert<int64_t>(rootNode, "integer", 1234567890);

		toml::table tblNode{};
		/// 创建[NET_INTERFACE]
		tblNode.insert_or_assign("TOML_DEMO", rootNode);

		/// 下一是一个数组的写入
		
		{
			/// 定义了水果信息
			struct FruitInfo
			{
				/// 水果的名称
				std::string     m_name{""};
				/// 水果的重量
				int64_t         m_kilograms{0};
			};

			/// <key-水果的名称，value-水果信息>
			using HashFruitInfo = std::unordered_map<std::string, FruitInfo>;

			HashFruitInfo fruitInfoHash{{"apple", {"apple", 1}}, {"banana", {"banana", 2}}, {"blueberry", {"blueberry", 3}}};


			toml::array tmpArr;
			// for(auto& [key, value] : tmpArr)
			for (HashFruitInfo::iterator it = fruitInfoHash.begin(); it != fruitInfoHash.end(); ++ it)
			{
				toml::table tblTmp{};
				oct::tableNodeInsert<std::string>(tblTmp, "name", it->first);
				oct::tableNodeInsert<int64_t>(tblTmp, "kilograms", it->second.m_kilograms);

				tmpArr.insert(tmpArr.begin(), tblTmp);
			}

			/// 将数组中的内容
			tblNode.insert_or_assign("fruit", tmpArr);
		}


		std::string tomlFile = appPath() + "/4.toml";
		std::ofstream file(tomlFile, std::ios::out | std::ios::trunc);
		if (false == file.is_open())
		{
			std::cout << "\n failed to open the file=" << tomlFile << "\n\n";
		}
		else 
		{
			/// 写入文件
			file << tblNode;
			file.close();
		}
	}

	/// 5. 
	{
		/// TOML FILE CONTENTS
		/// [TOML_DEMO]
		/// bool = false
		/// bool_arr = [ false, false ]
		/// complex_arr = [ false, '456', 123 ]
		/// integer = 1234567890
		/// integer_arr = [ 1, 0 ]
		/// str_arr = [ '1', '0' ]
		/// string = 'this is a string'
		/// 
		///     [[NET_INTERFACE.fruit]]
		///     kilograms = 2
		///     name = 'banana'
		/// 
		///     [[NET_INTERFACE.fruit]]
		///     kilograms = 1
		///     name = 'apple'
		/// 
		///     [[NET_INTERFACE.fruit]]
		///     kilograms = 3
		///     name = 'blueberry'

		toml::table rootNode{};
		oct::tableNodeInsert<std::string>(rootNode, "string", "this is a string");
		oct::tableNodeInsert<bool>(rootNode, "bool", false);
		oct::tableNodeInsert<int64_t>(rootNode, "integer", 1234567890);

		toml::table tblNode{};
		/// 创建[NET_INTERFACE]
		tblNode.insert_or_assign("TOML_DEMO", rootNode);

		/// 下一是一个数组的写入
		{
			/// 定义了水果信息
			struct FruitInfo
			{
				/// 水果的名称
				std::string     m_name{""};
				/// 水果的重量
				int64_t         m_kilograms{0};
			};

			/// <key-水果的名称，value-水果信息>
			using HashFruitInfo = std::unordered_map<std::string, FruitInfo>;

			HashFruitInfo fruitInfoHash{{"apple", {"apple", 1}}, {"banana", {"banana", 2}}, {"blueberry", {"blueberry", 3}}};


			toml::array tmpArr;
			// for(auto& [key, value] : tmpArr)
			for (HashFruitInfo::iterator it = fruitInfoHash.begin(); it != fruitInfoHash.end(); ++ it)
			{
				toml::table tblTmp{};
				oct::tableNodeInsert<std::string>(tblTmp, "name", it->first);
				oct::tableNodeInsert<int64_t>(tblTmp, "kilograms", it->second.m_kilograms);

				tmpArr.insert(tmpArr.begin(), tblTmp);
			}

			/// 将数组中的内容 - 关键代码
			tblNode["TOML_DEMO"].as_table()->insert_or_assign("fruit", tmpArr);
		}



		std::string tomlFile = appPath() + "/5.toml";
		std::ofstream file(tomlFile, std::ios::out | std::ios::trunc);
		if (false == file.is_open())
		{
			std::cout << "\n failed to open the file=" << tomlFile << "\n\n";
		}
		else 
		{
			/// 写入文件
			file << tblNode;
			file.close();
		}
	}

	/// 6. 
	{
		/// TOML FILE CONTENTS
		/// [TOML_DEMO]
		/// bool = false
		/// bool_arr = [ false, false ]
		/// complex_arr = [ false, '456', 123 ]
		/// integer = 1234567890
		/// integer_arr = [ 1, 0 ]
		/// str_arr = [ '1', '0' ]
		/// string = 'this is a string'
		/// 
		/// [[fruit]]
		/// kilograms = 2
		/// name = 'banana'
		/// 
		/// [[fruit]]
		/// kilograms = 1
		/// name = 'apple'
		/// 
		/// [[fruit]]
		/// kilograms = 3
		/// name = 'blueberry'
		
		using namespace oct;

		toml::table rootNode{};
		tableNodeInsert<std::string>(rootNode, "string", "this is a string");
		tableNodeInsert<bool>(rootNode, "bool", false);
		tableNodeInsert<int64_t>(rootNode, "integer", 1234567890);

		toml::table tblNode{};
		/// 创建[NET_INTERFACE]
		tblNode.insert_or_assign("TOML_DEMO", rootNode);

		/// 下一是一个数组的写入
		 
		{
			/// 定义了水果信息
			struct FruitInfo
			{
				/// 水果的名称
				std::string     m_name{""};
				/// 水果的重量
				int64_t         m_kilograms{0};
			};

			/// <key-水果的名称，value-水果信息>
			using HashFruitInfo = std::unordered_map<std::string, FruitInfo>;

			HashFruitInfo fruitInfoHash{{"apple", {"apple", 1}}, {"banana", {"banana", 2}}, {"blueberry", {"blueberry", 3}}};


			toml::array tmpArr;
			// for(auto& [key, value] : tmpArr)
			for (HashFruitInfo::iterator it = fruitInfoHash.begin(); it != fruitInfoHash.end(); ++ it)
			{
				toml::table tblTmp{};
				tableNodeInsert<std::string>(tblTmp, "name", it->first);
				tableNodeInsert<int64_t>(tblTmp, "kilograms", it->second.m_kilograms);

				tmpArr.insert(tmpArr.begin(), tblTmp);
			}

			/// 将数组中的内容
			tblNode.insert_or_assign("fruit", tmpArr);
		}

		/// 数组的写入
		{
			/// 整数数组
			toml::array tmpArrInt;
			/// 字符串数组
			toml::array tmpArrStr;
			/// bool数组
			toml::array tmpArrBool;

			for (int index = 0; index < 2; ++ index)
			{
				arrayInsert<int64_t>(tmpArrInt, index);
				arrayInsert<std::string>(tmpArrStr, std::to_string(index));
				arrayInsert<bool>(tmpArrBool, false);
			}

			tblNode["TOML_DEMO"].as_table()->insert_or_assign("integer_arr", tmpArrInt);
			tblNode["TOML_DEMO"].as_table()->insert_or_assign("str_arr", tmpArrStr);
			tblNode["TOML_DEMO"].as_table()->insert_or_assign("bool_arr", tmpArrBool);


			/// 混合数组
			toml::array tmpArrComplex;
			arrayInsert<int64_t>(tmpArrComplex, 123);
			arrayInsert<std::string>(tmpArrComplex, "456");
			arrayInsert<bool>(tmpArrComplex, false);

			tblNode["TOML_DEMO"].as_table()->insert_or_assign("complex_arr", tmpArrComplex);

		}


		std::string tomlFile = appPath() + "/6.toml";
		std::ofstream file(tomlFile, std::ios::out | std::ios::trunc);
		if (false == file.is_open())
		{
			std::cout << "\n failed to open the file=" << tomlFile << "\n\n";
		}
		else 
		{
			/// 写入文件
			file << tblNode;
			file.close();
		}
	}
    
	std::cout << "\n\n";

	return 0;
}