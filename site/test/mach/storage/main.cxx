//#include <Poco/any.h>
#include <string>
#include <iostream>
//#include <sstream>
//#include <fstream>
//#include <algorithm>	// For for_each ()
//#include <omp.h>
//#include <unordered_map>
//#include <typeinfo>
//#include <list>
//#include <string>
//#include <stack>

// #include <cln/cln.h>

//
//// Boost:
//#include <boost/any.hpp>
//#include <boost/thread.hpp>
//// Boost serialization:
//#include <boost/serialization/serialization.hpp>
//#include <boost/serialization/split_member.hpp>	// For separating reading and writing.
//#include <boost/serialization/list.hpp>
//#include <boost/archive/text_oarchive.hpp>
//#include <boost/archive/text_iarchive.hpp>
////#include <boost/archive/binary_oarchive.hpp>
////#include <boost/archive/binary_iarchive.hpp>

#include <noware/var>
#include <noware/machine>
//#include <noware/container/entity.h++>

//////#include <noware/containers/entity.h++>
////#include <noware/containers/variable-undefine.h++>
//#include <noware/math.h++>
//#include <noware/serialization.h++>

//#include <noware/containers/text.h++>
//#include <noware/containers/any.hpp>
//#include <NoWare/Container/Variable.h>
//#include <noware/containers/array.hpp>
//#include <NoWare/Containers/Dynamic.h>
//#include <NoWare/Containers/Dynamic.cpp>
//#include "MPI.h"
//#include <noware/cluster.h++>
//#include "Memory.h"
//#include "HPC.cpp"
//#include "Numerical Strings.h"
//#include <vector>
//struct My
//{
//	int x, y;
//};

int main (int argc, char * argv [])
{
	//using boost::any_cast;
	using namespace std;
	//using boost;
	using namespace noware;
	
	for (/*unsigned long long*/ int i = 0; i < argc; ++ i)
		std::cout << argv [i] << ' ';
	std::cout << std::endl;
	std::cout << "Argument Count: " << argc << std::endl;
	
	
	noware::var selection, group, name, value;
	noware::machine::storage dam;	// Direct Access Memory.
	
	do
	{
		std::cout << std::endl;
		std::cout << "Storage Space:" << std::endl;
		std::cout << std::endl;
		std::cout << "Options:" << std::endl;
		std::cout << std::endl;
		std::cout << "  " << "0. Exit" << std::endl;
		std::cout << "  " << "1. Exist" << std::endl;
		std::cout << "  " << "2. Get" << std::endl;
		std::cout << "  " << "3. Set" << std::endl;
		std::cout << "  " << "4. Remove" << std::endl;
		std::cout << "  " << "5. Size" << std::endl;
		std::cout << "  " << "6. Clear" << std::endl;
		
		std::cout << std::endl;
		std::cout << "Selection: ";
		getline (std::cin, selection);
		//std::cout << std::endl;
		
		if (selection == 0)
		{
			//return EXIT_SUCCESS;
			break;
		}
		else if (selection == 1)
		{
			std::cout << "  " << "Group: ";
			getline (std::cin, group);
			
			std::cout << "  " << "Name: ";
			getline (std::cin, name);
			
			std::cout << "  " << "Exists : [" << group << "][" << name << "] == " << (dam.exist (group, name) ? "True" : "False") << std::endl;
		}
		else if (selection == 2)
		{
			std::cout << "  " << "Group: ";
			getline (std::cin, group);
			
			std::cout << "  " << "Name: ";
			getline (std::cin, name);
			
			std::cout << "  " << "Getting : [" << group << "][" << name << "] == [" << dam.get (group, name) << ']' << std::endl;
		}
		else if (selection == 3)
		{
			std::cout << "  " << "Group: ";
			getline (std::cin, group);
			
			std::cout << "  " << "Name: ";
			getline (std::cin, name);
			
			std::cout << "  " << "Value: ";
			getline (std::cin, value);
			
			std::cout << "  " << "Setting : [" << group << "][" << name << "] = [" << value << "] ... " << (dam.set (group, name, value) ? "Success" : "Failure") << std::endl;
		}
		else if (selection == 4)
		{
			std::cout << "  " << "Group: ";
			getline (std::cin, group);
			
			std::cout << "  " << "Name: ";
			getline (std::cin, name);
			
			std::cout << "  " << "Removing : [" << group << "][" << name << "] ... " << (dam.remove (group, name) ? "Success" : "Failure") << std::endl;
		}
		else if (selection == 5)
		{
			std::cout << "  " << "Size == [" << dam.size () << ']' << std::endl;
		}
		else if (selection == 6)
		{
			std::cout << "  " << "Clearing ... " << (dam.clear () ? "Success" : "Failure") << std::endl;
		}
		else //if (selection != 0)
		{
			std::cout << "  " << "Unrecognized Selection" << std::endl;
		}
	}
	//while (selection != 0);
	while (true);
	
	return EXIT_SUCCESS;
}
