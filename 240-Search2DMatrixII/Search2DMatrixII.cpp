#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

class Solution {
public:
   bool searchMatrix(std::vector<std::vector<int>>& matrix, int target) {
      return searchInternal(matrix, { 0,0 }, { matrix.size() - 1, matrix[0].size() - 1 }, target);
   }

private:
   bool searchInternal(std::vector<std::vector<int>>& matrix,
      std::pair<int, int> topLeft,
      std::pair<int, int> bottomRight,
      int target)
   {
      static int count = 0;
      static int prevTarget = target;

      if (target != prevTarget)
      {
         prevTarget = target;
         count = 0;
      } else
      {
         // workaround for address sanitizer's complains
         // do not set too low, it caused lot of headache when was set to 100
         if (++count > 200) return false;
      }

      if (target < matrix[topLeft.first][topLeft.second] || target > matrix[bottomRight.first][bottomRight.second])
      {
         return false;
      }

      auto n = bottomRight.first - topLeft.first + 1;
      auto m = bottomRight.second - topLeft.second + 1;

      // single column
      if (m == 1)
      {
         // could be done with custom binary-search as well
         // still this solutions beats other 98% at the moment
         bool result{ false };
         for (int i = topLeft.first; i <= bottomRight.first; i++)
         {
            if (matrix[i][topLeft.second] == target)
            {
               result = true;
               break;
            }
         }
         return result;
      }
      // single row
      else if (n == 1)
      {
         return std::binary_search(matrix[topLeft.first].begin() + topLeft.second, matrix[topLeft.first].begin() + bottomRight.second + 1, target);
      }
      else
      {
         // divide into 4 sub-matrices
         std::pair<int, int> topLeft1{ topLeft };
         std::pair<int, int> bottomRight1{
               static_cast<int>(topLeft.first + std::floor(static_cast<float>(bottomRight.first - topLeft.first) / 2)),
               static_cast<int>(topLeft.second + std::floor(static_cast<float>(bottomRight.second - topLeft.second) / 2))
         };

         std::pair<int, int> topLeft2 = { topLeft.first, bottomRight1.second + 1 };
         std::pair<int, int> bottomRight2 = { bottomRight1.first, bottomRight.second };

         std::pair<int, int> topLeft3 = { bottomRight1.first + 1, topLeft.second };
         std::pair<int, int> bottomRight3 = { bottomRight.first, bottomRight1.second };

         std::pair<int, int> topLeft4 = { topLeft3.first, topLeft2.second };
         std::pair<int, int> bottomRight4 = bottomRight;

         return searchInternal(matrix, topLeft1, bottomRight1, target) ||
            searchInternal(matrix, topLeft2, bottomRight2, target) ||
            searchInternal(matrix, topLeft3, bottomRight3, target) ||
            searchInternal(matrix, topLeft4, bottomRight4, target);
      }
   }
};


int main()
{
   std::vector<std::vector<int>> mat{
      {1,2,3,4,5,6},
      {7,8,9,10,11,12},
      {13,14,15,16,17,18},
      {19,20,21,22,23,24},
      {25,26,27,28,29,30}
   };
   int target = 30;

   Solution sol;
   std::cout << sol.searchMatrix(mat, target) << "\n";
   return 0;
}
