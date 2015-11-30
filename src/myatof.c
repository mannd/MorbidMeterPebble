/* modified from https://code.google.com/p/unladen-swallow/source/browse/branches/release-2009Q1-maint/Python/atof.c */

/* Just in case you haven't got an atof() around...
   This one doesn't check for bad syntax or overflow,
   and is slow and inaccurate.
   But it's good enough for the occasional string literal... */

#include <pebble.h>
#include "myatof.h"

int isdigit(int c)
{
  char cc = (char)c;
  if(cc>='0' && cc<='9')
    return 1;    
  return 0;    
}


double myatof(char *s)
{
  int negation = 1;
  if (*s == '-') {
    negation = -1;
    s++;
  }
  double a = 0.0;
  int e = 0;
  int c;
  while ((c = *s++) != '\0' && isdigit(c)) {
    a = a*10.0 + (c - '0');
  }
  if (c == '.') {
    while ((c = *s++) != '\0' && isdigit(c)) {
      a = a*10.0 + (c - '0');
      e = e-1;
    }
  }
  if (c == 'e' || c == 'E') {
    int sign = 1;
    int i = 0;
    c = *s++;
    if (c == '+')
      c = *s++;
    else if (c == '-') {
      c = *s++;
      sign = -1;
    }
    while (isdigit(c)) {
      i = i*10 + (c - '0');
      c = *s++;
    }
    e += i*sign;
  }
  while (e > 0) {
    a *= 10.0;
    e--;
  }
  while (e < 0) {
    a *= 0.1;
    e++;
  }
  return a * negation;
}
