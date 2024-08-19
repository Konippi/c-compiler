#!/bin/bash -u

assert() {
  expected="$1"
  input="$2"

  ./sample "$input" > tmp.s
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

echo OK
