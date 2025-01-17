# 语义解析要求

### 一些假设
1) 假设1：整型（int）变量不能与浮点型（float）变量相互赋值或者相互运算。
2) 假设2：仅有int型变量才能进行逻辑运算或者作为if和while语句的条件；仅有int型和float型变量才能参与算术运算。
3) 假设3：任何函数只进行一次定义，无法进行函数声明。
4) 假设4：所有变量（包括函数的形参）的作用域都是全局的，即程序中所有变量均不能重名。
5) 假设5：结构体间的类型等价机制采用名等价（Name Equivalence）的方式。
6) 假设6：函数无法进行嵌套定义。
7) 假设7：结构体中的域不与变量重名，并且不同结构体中的域互不重名。

### 一些要求识别出的语义错误
1) 错误类型1：变量在使用时未经定义。
2) 错误类型2：函数在调用时未经定义。
3) 错误类型3：变量出现重复定义，或变量与前面定义过的结构体名字重复。
4) 错误类型4：函数出现重复定义（即同样的函数名出现了不止一次定义）。
5) 错误类型5：赋值号两边的表达式类型不匹配。
6) 错误类型6：赋值号左边出现一个只有右值的表达式。
7) 错误类型7：操作数类型不匹配或操作数类型与操作符不匹配（例如整型变量与数组变量相加减，或数组（或结构体）变量与数组（或结构体）变量相加减）。
8) 错误类型8：return语句的返回类型与函数定义的返回类型不匹配。
9) 错误类型9：函数调用时实参与形参的数目或类型不匹配。
10) 错误类型10：对非数组型变量使用“[…]”（数组访问）操作符。
11) 错误类型11：对普通变量使用“(…)”或“()”（函数调用）操作符。
12) 错误类型12：数组访问操作符“[…]”中出现非整数（例如a[1.5]）。
13) 错误类型13：对非结构体型变量使用“.”操作符。
14) 错误类型14：访问结构体中未定义过的域。
15) 错误类型15：结构体中域名重复定义（指同一结构体中），或在定义时对域进行初始化（例如struct A { int a = 0; }）。
16) 错误类型16：结构体的名字与前面定义过的结构体或变量的名字重复。
17) 错误类型17：直接使用未定义过的结构体来定义变量。