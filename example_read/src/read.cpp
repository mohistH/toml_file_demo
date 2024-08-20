#include <string>
#include <iostream>
#include <toml>

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


/// @brief to ouput the whole file's content
/// @param examFile 
void outputFile(const std::string& examFile)
{
	using namespace toml;
	/// 2. to parse the toml file
	parse_result config = parse_file(examFile);

	/// 3. to output the file's content
	std::cout << config << "\n";
}


int main(int argc, char** argv, char** envp)
{
	/// 1. to get the toml example file
    std::string tomlFile{appPath() + std::string{"/example.toml"}};

	/// 2. to output the whole file's content 
	// outputFile(tomlFile);

    using namespace toml;
	/// 2. to parse the toml file
	parse_result config = parse_file(tomlFile);

    /// 3. to read string: title
    {
        /// title= TOML Example
		const std::string str = config["title"].value<std::string>().value();
        std::cout << "title= " << str << std::endl << "\n";
    }
    
    /// read integer
    {
        /// int1= -9223372036854775808
        /// int2= 9223372036854775807

        int64_t int1 = config["int1"].value<int64_t>().value();
        int64_t int2 = config["int2"].value<int64_t>().value();
        std::cout << "int1= " << int1 << std::endl;
        std::cout << "int2= " << int2 << std::endl << "\n";
    }
	/// read float point 
	{
		/// # floats
        /// flt1 = 0.00000000001
        /// flt2 = 1e-11
        /// flt3 = 11.0
        /// flt4 = +1.0
		double flt1 = config["flt1"].value<double>().value();
		double flt2 = config["flt2"].value<double>().value();
		double flt3 = config["flt3"].value<double>().value();
		double flt4 = config["flt4"].value<double>().value();
		std::cout << "flt1=" << flt1 << std::endl;
        std::cout << "flt2=" << flt2 << std::endl;
        std::cout << "flt3=" << flt3 << std::endl;
        std::cout << "flt4=" << flt4 << std::endl << "\n";
	}
    /// read the date and time
    {
		/// tim1 = 07:32:00
		toml::time tim1 = config["tim1"].value<toml::time>().value();
		std::cout << "tim1=" << tim1 << ", hour=" << (int)tim1.hour << ", minute=" << (int)tim1.minute << ", second=" << (int)tim1.second << "\n";

		/// tim2 = 00:32:00.100000000
		toml::time tim2 = config["tim2"].value<toml::time>().value();
		std::cout	<< "tim1=" << tim1 << ", hour=" << (int)tim1.hour << ", minute=" << (int)tim1.minute << ", second=" 
					<< (int64_t)tim1.second << ", nanosecond=" << (int64_t)tim2.nanosecond << "\n";
		/// dat1 = 1979-05-27
		toml::date dat1 = config["dat1"].value<toml::date>().value();
		std::cout << "dat1=" << dat1 << ", year=" << (int)dat1.year << ", month=" << (int)dat1.month << ", day=" << (int)dat1.day << "\n" << "\n";
	}

	/// read date time
	{
		/// odt1 = 1979-05-27T07:32:00Z
		toml::date_time odt1 = config["odt1"].value<toml::date_time>().value();
		std::cout << "odt1=" << odt1 << ", year=" << (int)odt1.date.year << ", month=" << (int)odt1.date.month << ", day=" << (int)odt1.date.day
			<< " hour=" << (int)odt1.time.hour << ", minute=" << (int)odt1.time.minute << ", second=" << (int)odt1.time.second << ", offset=" << (int)odt1.offset->minutes << "\n\n";

	}
	/// read string of multiple lines  
	{
		///# strings
		///	str1 = """
		///	This is a
		///	multi - line
		///	string.
		///	"""
		std::string str1 = config["str1"].value<std::string>().value();
		std::cout << "str1=" << str1 << "\n\n";
	}

	/// read array
	{
		/// arr = ['this', 'is', 'a', 'long', 'array', 'with', 16, 'elements.', 'it', 'should', 'be', 'printed', 'as', 'a', 'multiline', 'array.']
		toml::array* parr = config["arr"].as_array();

		size_t arrSize = parr->size();
		std::cout << "arr count=" << arrSize << "\n";
		for (size_t index = 0; index < arrSize; ++index)
		{
			toml::node* pNode = parr->get(index);
			toml::node_view<toml::node> tmpIndexNode{ *pNode };
			if (true == tmpIndexNode.is_integer())
			{
				std::cout << " " << tmpIndexNode.value<int64_t>().value();
			}
			
			if (true == tmpIndexNode.is_string())
			{
				std::cout << " " << tmpIndexNode.value<std::string>().value();
			}
		}

		std::cout << "\n\n";
	}
	/// read table
	{
		/// tab = { this = 'is', an = 'inline', table = 'yay'}
		toml::node_view<toml::node> tabNode = config["tab"];
		/// key=this,	value=is
		/// key=an,		value = inline
		/// key=table,	value='yay'
		std::cout << "this " << tabNode.at_path("this").value<std::string>().value();
		std::cout << " an " << tabNode.at_path("an").value<std::string>().value();
		std::cout << " table " << tabNode.at_path("table").value<std::string>().value() << "\n\n";
	}
	
	/// read:  dotted.keys
	{
		/// dotted.keys.are = "supported"
		std::cout << "dotted.keys.are " << config.at_path("dotted.keys.are").value<std::string>().value() << "\n";
		std::cout << "dotted.keys.are " << config["dotted"]["keys"]["are"].value<std::string>().value() << "\n\n";
	}

	/// read owner table
	{
		/// [owner]
		/// name = "Mark Gillard"
		/// dob = 1987 - 03 - 16 10:20 : 00 + 09 : 30
		std::cout << "name=" << config["owner"]["name"].value<std::string>().value() << "\n";
		std::cout << "dob=" << config["owner"]["dob"].value<toml::date_time>().value() << "\n";
		
		/// another way to read
		toml::node_view<toml::node> ownerNode = config["owner"];
		std::cout << "name=" << oct::toStdString(ownerNode, "name") << "\n";
		std::cout << "dob=" << ownerNode["dob"].value<toml::date_time>().value() << "\n\n";
	}

	/// read the sub table of owner
	{
		///	[owner]
		///	name = "Mark Gillard"
		///	dob = 1987 - 03 - 16 10:20 : 00 + 09 : 30
		///
		///	[[owner.pets]]
		///	name = "Brian"
		///	species = "cat"
		///
		///	[[owner.pets]]
		///	name = "Skippy"
		///	species = "kangaroo"
		toml::node_view<toml::node> ownerPetsNode = config["owner"]["pets"];
		toml::array* petsArr = ownerPetsNode.as_array();
		std::cout << "pets arr's count =" << petsArr->size() << "\n";
		for (size_t index = 0; index < petsArr->size(); ++index)
		{
			toml::node* pNode = petsArr->get(index);
			toml::node_view<toml::node> tmpIndexNode{ *pNode };
			std::cout << "index=" << index << ", name=" << oct::toStdString(tmpIndexNode, "name") << ", species=" << oct::toStdString(tmpIndexNode, "species") << "\n";
		}
	}
    
	std::cout << "\n\n";

	return 0;
}