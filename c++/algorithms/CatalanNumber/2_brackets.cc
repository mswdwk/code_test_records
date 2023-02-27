#include<stdio.h>

//l表示剩余的左括号数量，r表示剩余的右括号数量，第三个参数记录合法的括号组合数
void findParens(int l, int r, int& validCnt)
{
    if(l > r){return;} //剩余的左括号大于了右括号，非法case
    if(l ==0 ){ //剩余的左括号肯定先被减少到0，剩余全是右括号
        validCnt++;
        return;
    }

    findParens(l-1,r,validCnt); //将当前位置赋值为左括号
    findParens(l, r-1,validCnt);//将当前位置赋值为右括号
}
void testGenerateParens(int pair)
{
    int validCnt=0;
    findParens(pair,pair,validCnt);
    printf("%d\n",validCnt);
}

int main()
{
	testGenerateParens(3);
	testGenerateParens(4);
	testGenerateParens(5);
	return 0;
}
