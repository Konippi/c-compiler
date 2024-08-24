#!/bin/bash -u

assert() {
  expected="$1"
  input="$2"

  ./c_compiler "$input" > tmp.s
  cc -o tmp tmp.s
  ./tmp
  actual="$?"

  if [ "$actual" = "$expected" ]; then
    echo "$input => $actual"
  else
    echo "$input => $expected expected, but got $actual"
    exit 1
  fi
}

assert 0 "0"
assert 3 "1+2"
assert 1 "2-1"
assert 42 "25+25-8"
assert 42 "25 + 25 - 8"
assert 42 " 25+25-8 "
assert 42 "6*7"
assert 42 "84/2"
assert 42 "(2 + (41 * 2)) / 2"
assert 42 "21 - (-21)"
assert 0 "0 == 1"
assert 1 "1 == 1"
assert 1 "1 != 0"
assert 0 "1 != 1"
assert 1 "1 < 2"
assert 0 "1 < 1"
assert 1 "1 <= 1"
assert 1 "1 > 0"
assert 0 "1 > 1"
assert 1 "1 >= 1"

echo OK
