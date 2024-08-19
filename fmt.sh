#!/bin/sh -eu

CHECK_MODE=false

if [ "$#" -gt 0 ]; then
  if [ "$1" = "--check" ]; then
    CHECK_MODE=true
  else
    echo "Error: Invalid argument '$1'. Only '--check' is supported."
    exit 1
  fi
fi

find src -type f \( -name "*.c" -o -name "*.h" \) | while read -r file_name; do
  if $CHECK_MODE; then
    printf "Checking format for %s\n" "$file_name"
    clang-format --dry-run --Werror "$file_name"
  else
    printf "Formatting %s\n" "$file_name"
    clang-format -i "$file_name"
  fi
done
