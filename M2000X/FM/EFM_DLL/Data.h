#pragma once


#include <string>
#include <fstream> 
#include <vector> 
#include <unordered_map>

#include "Log.h"
#include "Utils.h"

class Data
{
public:
	virtual ~Data() {};
	std::unordered_map<std::string, std::vector<double>> map;

	bool loadData(std::string sFilename)
	{
		CLog::trace("loading file %s", sFilename);
		std::ifstream fs;
		fs.open(sFilename);
		
		if (fs.is_open())
		{
			CLog::trace("file %s loaded", sFilename);
			map.clear();
			std::string line;
			std::string coff;
			std::vector<double> coffs;
			bool bRes = false;
			bool bEnd = false;
			double val = 0;
			while (!fs.eof())
			{
				std::getline(fs, line);
				bRes = false;
				bEnd = false;
				coffs.clear();
				val = 0;
				size_t w = 0;
				size_t i = 0;//(int)line.find_first_of("=");
				if (i != -1)
				{
					while (i < line.length() && !bEnd)
					{
						switch (line[i])
						{
						case '=':
							coff = line.substr(0, i);
							coff.erase(std::remove_if(coff.begin(), coff.end(), std::isspace), coff.end());
							coff.shrink_to_fit();
							w = i;
							break;
						case '/':
							bEnd = true;//the rest is a remark line
							break;
						case '-':
							if (line[i + 1] == '-')
								bEnd = true;//the rest is a remark line
							break;
						case '{':
							w = i;
							break; // skip
						case '}':
						case ';':
							bEnd = true;
						case ',':
						{
							bRes = true;
							std::string s = line.substr(w + 1, i - w - 1);
							w = i;

							val = round(atof(s.c_str()), 8);
							coffs.push_back(val);
							break; // 
						}
						default:
							break;
						}
						i++;
					}
				}
				if (bRes)
					map.insert(std::pair<std::string, std::vector<double>>(coff, coffs));
			}
		}
		else
			CLog::trace("file %s - not loaded", sFilename);


		fs.close();

		return map.size() > 0 ? true : false;
	}

	void getFirstFromMap(double& val, std::string key )
	{
		auto list = map[key];
		if (list.begin() != list.end())
		{
			if (list.size() > 0)
				val = list[0];
			else
				val = 0;

			CLog::cotrace("coff,%s,%f\n", key.c_str(), val);
		}
		else
			CLog::cotrace("err,%s\n", key.c_str());

	}

};