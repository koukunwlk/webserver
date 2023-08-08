#!/bin/sh

GIT_DIR=$(git rev-parse --git-dir)

echo "Installing hooks..."

cp pre_commit.sh $GIT_DIR/hooks/pre-commit

chmod +x $GIT_DIR/hooks/pre-commit

echo "Done"!