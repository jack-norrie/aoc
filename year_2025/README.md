## Advent of Code

This year I decided to use [Advent of Code](https://adventofcode.com/2024) as an opportunity to practice and learn C.

This follows my AOC journey last year with C++, which I originally picked so that I could get a better understanding of low level programming concepts as a Python developer. However, I was left a little disappointed since C++ introduces many higher level abstractions, such as smart pointers, that make some of the low level tasks I was trying to learn/practice trivial. That is why this year I am stripping away the abstractions and going straight to C. The learning curve won't be so bad given my syntactic knowledge, but I anticipate that I will quickly have to to start developing my own data structures, which I could take for granted when using C++.

My main resource for learning the syntax of the language will be The C Programming Language. I have heard the exercises for this book are excellent, and as such I will be doing them all over the course of this month. However, one downside of this book is that it is very terse, as such I will supplement it with C Programming: A Modern Approach when I require more exposition.

## Problems

### Day 1

#### Description

The setup for this problem is that you occupy some position on a ring with 100 spaces. At each iteration you can either move left or right by some amount. The first part of the problem has you count the number of times you land on zero throughout the traversal as your final square, while the last part has you count how many times you land on zero throughout any part of the traversal.

#### Solution

To solve the first part you need to write logic to open a file and iterate over its lines. You then need to use the information on each line to update your position. This requires usage of modular arithmetic. Unfortunately, C does not implement mathematical modulo, instead it rounds to zero, i.e. the modulo operation finds `r` such that `x = round_to_zero(x / m) * m + r`, while mathematical modulo finds `r` for `x = floor(x / m) * m + r`. To overcome this problem you can use the pattern `((x % m) + m) % m`. This is a no-op if `x` is positive, but if `x` is negative then this has the effect of undoing the off by one error on the quotient. Finally, you increment your result whenever you land on a zero.

The next part of the problem can be solved by dividing the raw position update, pre modulo, by the modulo, since this will give an indication of how many times zero was passed in the traversal. However, yet again we must be mindful of the rounding towards zero. For example, a naive handling of this, would cause `50 -> -1` to not be counted as a zero passing. We notice that additional crossings past the original positive to negative crossing does get counted, e.g. from `50 -> -101` will miss 1 -> 0 but will include -99 -> 100, so we need to consider the edge case of the first positive to negative crossing. As such we add a `+1` increment check for the condition `new_pos <= 0 && pos > 0`. We then apply the same logic for all paths `res += abs(new_pos / m)`.

#### Lessons

- How to open a file
- How to slice a string
- How to cast between types
- Division rounds towards zero

## Day 2

#### Description

The challenge posed by this problem was detecting if a string was composed on k copies of a substring.

#### Solution

The function I can up with to check this was to simply check every length `m` segment against the first length `m` segment. Since equality is an equivalence relation, this would then suffice to ensure that all the substrings were equal, i.e. the whole string was a tiling of repeated substrings.

```C
int is_k_copies(const char *s, int k) {
  int n = strlen(s);
  int m = n / k;

  int i;
  for (i = 0; i < strlen(s); i++) {
    if (s[i] != s[i % m]) {
      return 0;
    }
  }
  return 1;
}

```

#### Lessons

- How to split on a delimiter using strtok
- By understanding `strtok` I found how C handles internal state with `static` variables. This then lead me to learn about reentrant functions, specifically `strtok_r` which allowed me to perform nested tokenisation.
- `strcspan` (string compliment span) returns a pointer to the first occurrence of some target character, this can be used to remove newline characters.
- You cannot return an array.
- You can use structs to return multiple outputs for a function

## Day 3

#### Description

#### Solution

#### Lessons

## Day N

#### Description

#### Solution

#### Lessons
