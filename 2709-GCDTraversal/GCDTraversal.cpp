#include <array>
#include <iterator>
#include <numeric>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <vector>

constexpr int LIMIT{ 100001 };

class Solution {
public:
	Solution()
	{
		PROCESSED.resize(LIMIT);
	}

	bool canTraverseAllPairs(std::vector<int>& nums) {
		if (nums.size() == 1)
		{
			return true;
		}

		// Sieving primes for quick primality check
		Sieve(PRIMES_TABLE);

		// Placing primes together without gaps for quick factorization
		for (int num = 0; num < PRIMES_TABLE.size(); num++)
		{
			if (PRIMES_TABLE[num] == true)
			{
				PRIMES.push_back(num);
			}
		}

		// Calculate unique prime factors of each number and create set of unique combinations
		for (int i = 0; i < nums.size(); i++)
		{
			if (!PROCESSED[nums[i]])
			{
				auto factors = PrimeFactors(nums[i], UNIQUE_PRIMES);
				if (factors.empty())
				{
					return false;
				}
				UPF_SET.insert(factors);
				PROCESSED[nums[i]] = true;
			}
		}

		// Place unique factors into a vector for quick access 
		std::vector<int> allFactors;
		std::copy(UNIQUE_PRIMES.begin(), UNIQUE_PRIMES.end(), std::back_inserter(allFactors));
		if (allFactors.size() == 0) return false;

		// Create adjacency matrix (graph)
		P_MAT.resize(allFactors.size());
		for (auto& vec : P_MAT)
		{
			vec.resize(allFactors.size());
		}

		for (int i = 0; i < allFactors.size(); i++)
		{
			P_MAP[allFactors[i]] = i;
		}

		// Connect vertices with shared prime factors
		for (auto& factors : UPF_SET)
		{
			auto sz = factors.size();
			if (sz > 1)
			{
				for (int j = 0; j < sz - 1; j++)
				{
					for (int k = j + 1; k < sz; k++)
					{
						P_MAT[P_MAP[factors[j]]][P_MAP[factors[k]]] = true;
						P_MAT[P_MAP[factors[k]]][P_MAP[factors[j]]] = true;
					}
					P_MAT[P_MAP[factors[j]]][P_MAP[factors[j]]] = true;
				}
				P_MAT[P_MAP[factors[sz - 1]]][P_MAP[factors[sz - 1]]] = true;
			} else if (sz == 1)
			{
				P_MAT[P_MAP[factors[0]]][P_MAP[factors[0]]] = true;
			}
		}

		return IsGraphConnected(0, P_MAT);
	}

private:
	template<size_t LIMIT>
	void Sieve(std::array<bool, LIMIT>& primesTable)
	{
		std::fill(primesTable.begin(), primesTable.end(), true);
		primesTable[0] = false;
		primesTable[1] = false;

		uint32_t numberOfPrimes{ 0 };
		for (uint32_t i = 2; i < LIMIT; i++)
		{
			if (primesTable[i] != 0)
			{
				for (uint32_t j = 2 * i; j < LIMIT; j += i)
				{
					primesTable[j] = false;
				}
			}
		}
	}

	std::vector<int> PrimeFactors(int num, std::unordered_set<int>& collection)
	{
		std::vector<int> factors{};

		if (PRIMES_TABLE[num])
		{
			factors.push_back(num);
		}
		else
		{
			for (int i = 0; i < PRIMES.size() && PRIMES[i] <= num; i++)
			{
				if (num % PRIMES[i] == 0)
				{
					factors.push_back(PRIMES[i]);
					do {
						num /= PRIMES[i];
					} while (num % PRIMES[i] == 0);
					if (PRIMES_TABLE[num])
					{
						factors.push_back(num);
						break;
					}
				}
			}
		}

		collection.insert(factors.begin(), factors.end());

		return factors;
	}

	bool IsGraphConnected(int start, const std::vector<std::vector<bool>>& adjMatrix)
	{
		// DFS to check if the graph is connected
		size_t numVertices = adjMatrix.size();
		std::vector<bool> visited(numVertices, false);
		std::stack<int> stack;

		stack.push(start);

		while (!stack.empty()) {
			int current = stack.top();
			stack.pop();

			if (!visited[current]) {
				visited[current] = true;

				for (int i = 0; i < numVertices; ++i) {
					if (adjMatrix[current][i] && !visited[i]) {
						stack.push(i);
					}
				}
			}
		}

		int sum = std::accumulate(visited.begin(), visited.end(), 0);
		return (sum == numVertices);
	}

private:
	struct HashFactors
	{
		size_t operator()(const std::vector<int> v) const
		{
			size_t result{ 1 };
			for (auto e : v)
			{
				result *= e;
			}
			return result;
		}
	};

	std::array<bool, LIMIT> PRIMES_TABLE{};
	std::vector<int> PRIMES{};
	std::vector<std::vector<bool>> P_MAT{};
	std::unordered_map<int, int> P_MAP{};
	std::unordered_set<std::vector<int>, HashFactors> UPF_SET{};
	std::unordered_set<int> UNIQUE_PRIMES{};
	std::vector<bool> PROCESSED{};
};

int main()
{
	Solution sol;
	std::vector<int> nums{};
	for (int i = 2; i <= 100000; i++)
	{
		nums.push_back(i);
	}
	nums.push_back(1);
	bool possible = sol.canTraverseAllPairs(nums);
	return !possible;
}
