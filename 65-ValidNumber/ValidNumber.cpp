#include <iostream>
#include <string>
#include <fstream>

class Solution {
public:
	bool isNumber(std::string s) {
		uint8_t i = 0;
		bool e = false, m = false, p = false, d = false;
		bool unc = true;
		bool nodigit = true;
		while (i < s.size())
		{
			auto c = s[i];
			if (c - '0' >= 0 && c - '0' <= 9)
			{
				unc = false;
				nodigit = false;
				i++;
				continue;
			}
			switch (c)
			{
			case '.':
				if (d || e)
				{
					return false;
				}
				else
				{
					d = true;
				}
				break;
			case '-':
			case '+':
				if (m || p || (!e && i != 0) || (e && s[i - 1] != 'e' && s[i - 1] != 'E'))
				{
					return false;
				}
				else
				{
					unc = true;
					m = true;
				}
				break;
			case 'e':
			case 'E':
				if (e || nodigit)
				{
					return false;
				}
				else
				{
					m = false;
					p = false;
					e = true;
					unc = true;
				}
				break;
			default:
				return false;
			}
			i++;
		}

		if (unc)
		{
			return false;
		}

		return true;
	}
};

int main(int arc, char** argv)
{
	std::ifstream valid("valid.txt");
	std::ifstream invalid("invalid.txt");
	std::string str;

	Solution sol;

	std::cout << "VALIDS\n";
	while (valid >> str)
	{
		std::cout << str << " - ";
		if (sol.isNumber(str))
		{
			std::cout << "valid\n";
		}
		else
		{
			std::cout << "invalid\n";
		}
	}

	std::cout << "INVALIDS\n";
	while (invalid >> str)
	{
		std::cout << str << " - ";
		if (sol.isNumber(str))
		{
			std::cout << "valid\n";
		}
		else
		{
			std::cout << "invalid\n";
		}
	}

	return 0;
}
