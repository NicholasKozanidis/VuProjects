/**
 * RPN calculator
 * Only works with single digit inputs. 
 * Example input: 12+
 * Output: 3
 */

#include <stdio.h> 
#include <stdlib.h>
#include <assert.h>

#define STACK_SIZE 1024
char stack[STACK_SIZE];
int sp;

int pop()
{
  assert(sp > 0);
  return stack[sp--];
}

int top()
{
  assert(sp > 0);
  return stack[sp];
}

void push(int n)
{
  assert(sp < STACK_SIZE -1);
  stack[++sp] = n;
}


int main(int argc, char **argv)
{
  sp = -1;
  while (1) {
    char c = getc(stdin);
    int a,b;

    switch (c) {
      case '+':
        a = pop();
        b = pop();
        push(a + b);
        break;
      case '-':
        a = pop();
        b = pop();
        push(b - a);
        break;
      case '*':
        a = pop();
        b = pop();
        push(a * b);
        break;
      case EOF:
        // Terminate on CTRL-D
        printf("= %d\n", top());
        exit(0);
        break;
      case '\n':
        printf("= %d\n", top());
        break;
      default:
        push(c - '0'); // Push ASCII character as integer
    }
  }
  return 0; 
}
