#include <vector>
#include <algorithm>


constexpr int MAX_SUM{ 1000000007 };

class Solution {
public:
   int sumOfFlooredPairs(std::vector<int>& nums) {
      std::sort(nums.begin(), nums.end());

      int globalSum{ 0 };
      int localSum{ 0 };
      int prevNum{ 0 };
      int elemsLeft{ 0 };

      for (int i = 0; i < nums.size(); i++)
      {
         if (nums[i] == prevNum)
         {
            globalSum = (globalSum + localSum) % MAX_SUM;
            continue;
         }
         std::vector<int>::iterator it{ nums.begin() + i }, prevIt{ nums.end()};

         prevNum = nums[i];
         localSum = 0;

         auto maxRatio = nums.back() / nums[i];
         for (int j = maxRatio; j >= 1; j--)
         {
            it = std::lower_bound(nums.begin() + i, prevIt, nums[i] * j);
            localSum = (localSum + j * (prevIt - it)) % MAX_SUM;

            elemsLeft = it - nums.begin();
            if (elemsLeft <= j)
            {
               for (auto newIt = nums.begin(); newIt < it; newIt++)
               {
                  localSum = (localSum + (*newIt) / nums[i]) % MAX_SUM;
               }
               break;
            }

            prevIt = it;
         }

         globalSum = (globalSum + localSum) % MAX_SUM;
      }

      return globalSum;
   }
};

int main()
{
   std::vector<int> nums{ 4,3,4,3,5 };
   Solution sol;
   auto sum = sol.sumOfFlooredPairs(nums);

   return 0;
}
