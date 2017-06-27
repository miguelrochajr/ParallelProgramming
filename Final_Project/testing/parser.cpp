#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

void fillData(std::string filename)
{
	std::vector<std::string> testvector;
	std::vector<int> input1,input2;
	std::vector<std::string>::iterator itr,itr1,itr2;
	std::ifstream file(filename.c_str());	
	std::string inputdata;
	std::size_t start=0,end=0;
	while(file.good())
	{
		getline(file,inputdata,'\n');
		testvector.push_back(inputdata);
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////
	for(itr = testvector.begin();itr != testvector.end();itr++)
	{
		end = (*itr).find(",",start);
		input1.push_back((*itr1).substr(start,end));
		(*itr).erase(start,end+1);
		end = (*itr).find(",",start);
		input2.push_back((*itr).substr(start,end));
		(*itr).erase(start,end+1);
	}
	for(itr1 = input1.begin(), itr2 = input2.begin();itr1 != input1.end(),itr2 != input2.end();itr1++,itr2++)
	{
		std::cout << (*itr1) << "|" << (*itr2) << std::endl;
	}
}

int main(int argc, char *argv[])
{
	std::stringstream str;
	str << argv[1];
	fillData(str.str());
	return 0;
}
