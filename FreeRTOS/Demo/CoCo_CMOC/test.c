#include <cmoc.h>

struct s {
char a;
};

void main() {
struct s x;
x.a = 'A';
printf("&x = %X\n", &x);
char *y = (char *)&x;
printf("y[0] = %c\n", *y);
}