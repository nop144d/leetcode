#include <vector>
#include <array>
#include <unordered_map>
#include <set>

std::unordered_map<int, std::vector<int>> UNIQUE_PRIME_FACTORS;
static constexpr uint32_t MILLION{ 1000000 };
std::array<bool, MILLION> PRIMES_TABLE{};
std::vector<uint32_t> PRIMES;

class Solution
{
public:
int findValidSplit(std::vector<int>& nums) {
	// Sieving primes for quick primality check
	uint32_t numberOfPrimes = Sieve(PRIMES_TABLE);

	// Placing primes together without gaps for quick factorization
	PRIMES.reserve(numberOfPrimes);
	for (int i = 0; i < PRIMES_TABLE.size(); i++)
	{
		if (PRIMES_TABLE[i] == true)
		{
			PRIMES.push_back(i);
		}
	}

	Factorize(nums, PRIMES_TABLE, PRIMES, UNIQUE_PRIME_FACTORS);

	std::set<int> collectedFactors{};
	std::set<int> potentialFactors{};
	// collecting unique prime factors of the first element
	for (auto factor : UNIQUE_PRIME_FACTORS[nums[0]])
	{
		collectedFactors.insert(factor);
	}

	int border = 0;
	for (int i = 1; i < nums.size(); i++)
	{
		for (int factor : UNIQUE_PRIME_FACTORS[nums[i]])
		{
			// if (collectedFactors.contains(factor))
			if (collectedFactors.find(factor) != collectedFactors.end())
			{
				// current number shares a factor with previous number(s), moving the index
				border = i;
				// merging factors of all previous numbers
				collectedFactors.insert(potentialFactors.begin(), potentialFactors.end());
				potentialFactors.clear();

				// add all factors of current number and leave
				collectedFactors.insert(UNIQUE_PRIME_FACTORS[nums[i]].begin(), UNIQUE_PRIME_FACTORS[nums[i]].end());
				break;
			}
			else
			{
				// keeping this factors separately to further merge into collected ones in case needed
				potentialFactors.insert(factor);
			}
		}
	}

	UNIQUE_PRIME_FACTORS.clear();

	if (border == nums.size() - 1)
	{
		border = -1;
	}

	return border;
}

private:
template<size_t LIMIT>
uint32_t Sieve(std::array<bool, LIMIT>& primesTable)
{
	std::fill(primesTable.begin(), primesTable.end(), true);
	primesTable[0] = false;
	primesTable[1] = false;

	uint32_t numberOfPrimes{ 0 };
	for (uint32_t i = 2; i < LIMIT; i++)
	{
		if (primesTable[i] != 0)
		{
			numberOfPrimes++;
			for (uint32_t j = 2 * i; j < LIMIT; j += i)
			{
				primesTable[j] = false;
			}
		}
	}

	return numberOfPrimes;
}

template<size_t LIMIT>
void Factorize(const std::vector<int>& nums,
	const std::array<bool, LIMIT>& primesTable,
	const std::vector<uint32_t>& primesVector,
	std::unordered_map<int, std::vector<int>>& uniquePrimeFactors
)
{
	for (auto num : nums)
	{
		// if (!uniquePrimeFactors.contains(num))
		if (uniquePrimeFactors.find(num) == uniquePrimeFactors.end())
		{
			std::vector<int> uniqueFactors{};
			if (primesTable[num] == true)
			{
				uniqueFactors.push_back(num);
			}
			else
			{
				uint32_t temp = num;
				for (uint32_t i = 0; primesVector[i] <= temp; i++)
				{
					if (temp % primesVector[i] == 0)
					{
						uniqueFactors.push_back(primesVector[i]);

						do
						{
							temp /= primesVector[i];
						}
						while (temp % primesVector[i] == 0);

						// Unusual Numbers with few prime factors, typically one small and one large (Semiprimes)
						if (primesTable[temp] == true)
						{
							uniqueFactors.push_back(temp);
							break;
						}
					}
				}
			}
			uniquePrimeFactors.emplace(num, uniqueFactors);
		}
	}
}

};

int main()
{
	Solution sol;
	std::vector<int> nums = { 4,7,8,15,3,5 };
	auto index = sol.findValidSplit(nums);
	return index;
}
