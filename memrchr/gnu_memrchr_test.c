#include "stddef.h"
#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"
#include "sys/mman.h"

#include "string.h"

size_t iterations;
size_t page_size;

unsigned char* buf1;

char* memrchr_riscv(const char*, int, size_t);

typedef char* (*impl_t) (const char *, int, size_t);

impl_t impl = memrchr_riscv;

int ret = 0;

/* Naive implementation to verify results.  */
char *
simple_memrchr (const char *s, int c, size_t n)
{
  s = s + n;
  while (n--)
    if (*--s == (char) c)
      return (char *) s;
  return NULL;
}

static void
do_one_test (const char *s, int c, size_t n, char *exp_res)
{
  char *res = impl(s, c, n);
  if (res != exp_res)
    {
      printf("Wrong result in function %p %p",
	     res, exp_res);
      ret = 1;
      return;
    }
}

static void
do_test (size_t align, size_t pos, size_t len, int seek_char)
{
  size_t i;
  char *result;

  align &= 7;
  if (align + len >= page_size)
    return;

  for (i = 0; i < len; ++i)
    {
      buf1[align + i] = 1 + 23 * i % 127;
      if (buf1[align + i] == seek_char)
        buf1[align + i] = seek_char + 1;
    }
  buf1[align + len] = 0;

  if (pos < len)
    {
      buf1[align + pos] = seek_char;
      buf1[align + len] = -seek_char;
      result = (char *) (buf1 + align + pos);
    }
  else
    {
      result = NULL;
      buf1[align + len] = seek_char;
    }

    do_one_test ((char *) (buf1 + align), seek_char, len, result);
}


static void
do_random_tests (void)
{
  size_t i, j, n, align, pos, len;
  int seek_char;
  char *result;
  unsigned char *p = buf1 + page_size - 512;

  for (n = 0; n < iterations; n++)
    {
      align = random () & 15;
      pos = random () & 511;
      if (pos + align >= 512)
	pos = 511 - align - (random () & 7);
      len = random () & 511;
      if (pos >= len)
	len = pos + (random () & 7);
      if (len + align >= 512)
        len = 512 - align - (random () & 7);
      seek_char = random () & 255;
      j = len + align + 64;
      if (j > 512)
        j = 512;

      for (i = 0; i < j; i++)
	{
	  if (i == pos + align)
	    p[i] = seek_char;
	  else
	    {
	      p[i] = random () & 255;
	      if (p[i] == seek_char)
		p[i] = seek_char + 13;
	    }
	}

      if (pos < len)
	result = (char *) (p + pos + align);
      else
	result = NULL;

	if (impl((char *) (p + align), seek_char, len) != result)
	  {
	    printf("Iteration %zd - wrong result in function (%zd, %d, %zd, %zd) %p != %p, p %p",
		   n, align, seek_char, len, pos,
		   impl((char *) (p + align), seek_char, len),
		   result, p);
	    ret = 1;
	  }
    }
}

int
main (void)
{
  size_t i;
	
  iterations = 10000000;
  page_size = 2 * getpagesize();

  buf1 = mmap (0, (1 + 1) * page_size, PROT_READ | PROT_WRITE,
	       MAP_PRIVATE | MAP_ANON, -1, 0);

  for (i = 1; i < 8; ++i)
    {
      /* Test len == 0.  */
      do_test (i, i, 0, 0);
      do_test (i, i, 0, 23);

      do_test (0, 16 << i, 2048, 23);
      do_test (i, 64, 256, 23);
      do_test (0, 16 << i, 2048, 0);
      do_test (i, 64, 256, 0);

      do_test (0, i, 256, 23);
      do_test (0, i, 256, 0);
      do_test (i, i, 256, 23);
      do_test (i, i, 256, 0);

    }
  for (i = 1; i < 32; ++i)
    {
      do_test (0, i, i + 1, 23);
      do_test (0, i, i + 1, 0);
      do_test (i, i, i + 1, 23);
      do_test (i, i, i + 1, 0);

      do_test (0, 1, i + 1, 23);
      do_test (0, 2, i + 1, 0);
      do_test (i, 1, i + 1, 23);
      do_test (i, 2, i + 1, 0);
    }

  do_random_tests ();
  return ret;
}

