# include <stdio.h>
# include <stdlib.h>

// 字符串转换成 int 值的函数
int string_to_int(const char *str) {
    char *endptr;
    long int value = strtol(str, &endptr, 0);
    
    // 如果 endptr 等于 str，则转换失败
    if (endptr == str) {
        fprintf(stderr, "转换失败: 输入不是有效的整数字符串\n");
        return 0;
    }
    
    return (int)value;
}

// 将字符串转换为 float 值
double parse_float(const char *string) {
    // 使用标准库函数strtod将字符串转换为浮点数值
    return strtod(string, NULL);
}

int main(){
    printf("%f", parse_float("3.14e13"));
}