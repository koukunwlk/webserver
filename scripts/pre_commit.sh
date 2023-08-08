#!/bin/sh

echo -e "\033[0;32m[==========]\033[0m Validating formatting"

STAGED_FILES=$(git diff --cached --name-only --diff-filter=ACM | grep .cpp)

clang-format -style=Google --dry-run -Werror $STAGED_FILES --ferror-limit=0
if [ $? -ne 0 ]; then
  echo -e "\033[0;31m[  FAILED  ]\033[0m Fix formatting before commit!"
  exit 1
fi
echo -e "\033[0;32m[  PASSED  ]\033[0m Formatting check passed!"

exec make test_all

if [ $? -ne 0 ]; then
 echo "Tests must pass before commit!"
 exit 1
fi
