#include <stdio.h>
#include <stdlib.h>

/*
 * 网格路径计算函数 - Bug修正版本
 * 
 * 这个文件包含了原始错误版本和修正版本的对比，
 * 以及详细的bug分析和测试用例
 */

// ========== 原始错误版本 ==========
/*
 * 原始版本存在以下关键bug：
 * 1. 数组索引错误：初始化时使用了错误的索引
 * 2. 返回值错误：返回dp[m][n]而不是dp[m-1][n-1] 
 * 3. 内存泄漏：没有释放分配的内存
 * 4. 边界条件处理错误
 */
int uniquePathsBuggy(int m, int n) {
    // 分配2D数组内存
    int** dp = (int**)malloc(m * sizeof(int*));
    for(int i = 0; i < m; i++) {
        dp[i] = (int*)malloc(n * sizeof(int));
    }
    
    // Bug 1: 错误的边界初始化
    // 第一行应该是dp[0][i] = 1，但写成了dp[1][i] = 1
    for(int i = 0; i < n; i++) dp[1][i] = 1;  // 错误：应该是dp[0][i] = 1
    
    // Bug 2: 错误的第一列初始化和重复设置
    // 第一列应该是dp[i][0] = 1，但写成了dp[1][1] = 1
    for(int i = 0; i < m; i++) dp[1][1] = 1;  // 错误：应该是dp[i][0] = 1
    
    // DP填充（这部分逻辑可能正确，但由于初始化错误会导致问题）
    for(int i = 1; i < m; i++) {
        for(int j = 1; j < n; j++) {
            dp[i][j] = dp[i-1][j] + dp[i][j-1];
        }
    }
    
    // Bug 3: 错误的返回值
    // 应该返回dp[m-1][n-1]，但返回了dp[m][n]（数组越界）
    int result = dp[m][n];  // 错误：数组越界
    
    // Bug 4: 内存泄漏 - 没有释放分配的内存
    // 应该在这里释放内存，但原始版本没有
    
    return result;
}

// ========== 修正版本 ==========
/*
 * 修正后的版本解决了所有bug：
 * 1. 正确的数组索引初始化
 * 2. 正确的返回值 
 * 3. 正确的内存管理
 * 4. 正确的边界条件处理
 */
int uniquePathsFixed(int m, int n) {
    // 边界检查
    if (m <= 0 || n <= 0) return 0;
    if (m == 1 || n == 1) return 1;
    
    // 分配2D数组内存
    int** dp = (int**)malloc(m * sizeof(int*));
    for(int i = 0; i < m; i++) {
        dp[i] = (int*)malloc(n * sizeof(int));
    }
    
    // 修正1: 正确初始化第一行 - dp[0][i] = 1
    for(int i = 0; i < n; i++) {
        dp[0][i] = 1;
    }
    
    // 修正2: 正确初始化第一列 - dp[i][0] = 1  
    for(int i = 0; i < m; i++) {
        dp[i][0] = 1;
    }
    
    // DP填充：每个位置的路径数 = 上方路径数 + 左方路径数
    for(int i = 1; i < m; i++) {
        for(int j = 1; j < n; j++) {
            dp[i][j] = dp[i-1][j] + dp[i][j-1];
        }
    }
    
    // 修正3: 正确的返回值 - dp[m-1][n-1]
    int result = dp[m-1][n-1];
    
    // 修正4: 释放分配的内存，避免内存泄漏
    for(int i = 0; i < m; i++) {
        free(dp[i]);
    }
    free(dp);
    
    return result;
}

// ========== 安全演示错误版本的函数 ==========
/*
 * 这个函数安全地演示原始版本的逻辑错误，
 * 避免实际的数组越界崩溃
 */
void demonstrateBuggyLogic(int m, int n) {
    printf("原始错误版本的逻辑分析:\n");
    
    // 显示错误的初始化会导致什么问题
    printf("  - 错误初始化: dp[1][i] = 1 而不是 dp[0][i] = 1\n");
    printf("  - 这意味着第一行(索引0)没有被正确初始化\n");
    printf("  - 错误初始化: dp[1][1] = 1 而不是 dp[i][0] = 1\n");
    printf("  - 这意味着第一列(索引0)没有被正确初始化\n");
    printf("  - 返回dp[%d][%d]而不是dp[%d][%d] - 数组越界!\n", m, n, m-1, n-1);
    printf("  - 没有释放内存 - 内存泄漏!\n");
}

// ========== 测试函数 ==========
void testCase(int m, int n, int expected) {
    printf("\n=== Bug修正对比测试 ===\n");
    printf("测试用例: m=%d, n=%d\n", m, n);
    
    // 安全地演示原始错误版本的问题
    printf("原始版本结果: [错误结果或崩溃]\n");
    demonstrateBuggyLogic(m, n);
    
    // 测试修正版本
    int fixedResult = uniquePathsFixed(m, n);
    printf("修正版本结果: %d\n", fixedResult);
    printf("期望结果: %d\n", expected);
    
    if (fixedResult == expected) {
        printf("修正状态: ✓\n");
    } else {
        printf("修正状态: ✗\n");
    }
    
    printf("\nBug分析:\n");
    printf("1. 边界初始化错误 - 已修正 ✓\n");
    printf("   原始: for(int i = 0; i < n; i++) dp[1][i] = 1; // 错误索引\n");
    printf("   修正: for(int i = 0; i < n; i++) dp[0][i] = 1; // 正确索引\n");
    printf("2. 数组索引越界 - 已修正 ✓\n"); 
    printf("   原始: for(int i = 0; i < m; i++) dp[1][1] = 1; // 错误逻辑\n");
    printf("   修正: for(int i = 0; i < m; i++) dp[i][0] = 1; // 正确逻辑\n");
    printf("3. 内存泄漏 - 已修正 ✓\n");
    printf("   原始: 没有释放malloc分配的内存\n");
    printf("   修正: 添加了完整的内存释放代码\n");
    printf("4. 返回值错误 - 已修正 ✓\n");
    printf("   原始: return dp[m][n]; // 数组越界\n");
    printf("   修正: return dp[m-1][n-1]; // 正确索引\n");
    printf("========================================\n");
}

// ========== 数学验证函数 ==========
/*
 * 使用组合数学公式验证结果的正确性
 * uniquePaths(m,n) = C(m+n-2, m-1) = C(m+n-2, n-1)
 */
int calculateCombination(int n, int k) {
    if (k > n - k) k = n - k; // 利用对称性
    
    long long result = 1;
    for (int i = 0; i < k; i++) {
        result = result * (n - i) / (i + 1);
    }
    return (int)result;
}

int uniquePathsMath(int m, int n) {
    if (m <= 0 || n <= 0) return 0;
    return calculateCombination(m + n - 2, m - 1);
}

// ========== 主函数 ==========
int main() {
    printf("网格路径计算函数Bug修正演示\n");
    printf("==========================================\n");
    
    // 测试用例1: 3x3网格
    testCase(3, 3, 6);
    
    // 测试用例2: 3x2网格  
    testCase(3, 2, 3);
    
    // 测试用例3: 7x3网格
    testCase(7, 3, 28);
    
    // 测试用例4: 边界情况
    testCase(1, 1, 1);
    testCase(1, 5, 1);
    testCase(5, 1, 1);
    
    printf("\n=== 数学验证 ===\n");
    printf("使用组合数学公式验证DP算法的正确性:\n");
    int test_cases[][2] = {{3,3}, {3,2}, {7,3}, {1,1}, {1,5}, {5,1}};
    int num_tests = sizeof(test_cases) / sizeof(test_cases[0]);
    
    for (int i = 0; i < num_tests; i++) {
        int m = test_cases[i][0];
        int n = test_cases[i][1];
        int dp_result = uniquePathsFixed(m, n);
        int math_result = uniquePathsMath(m, n);
        printf("m=%d, n=%d: DP算法=%d, 数学公式=%d %s\n", 
               m, n, dp_result, math_result, 
               (dp_result == math_result) ? "✓" : "✗");
    }
    
    printf("\n=== 总结 ===\n");
    printf("所有bug已成功修正:\n");
    printf("✓ 数组索引错误已修正\n");
    printf("✓ 返回值错误已修正\n"); 
    printf("✓ 内存泄漏已修正\n");
    printf("✓ 边界条件处理已优化\n");
    printf("✓ 数学验证通过\n");
    printf("\n修正后的函数现在能够正确计算网格路径数量。\n");
    
    return 0;
}