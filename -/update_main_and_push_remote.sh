#!/bin/bash

# Define your feature branch name
FEATURE_BRANCH="execution-branch"

# Define the main branch name
MAIN_BRANCH="main"

# Define the remote repository name
REMOTE="origin"

# Checkout the main branch
git checkout $MAIN_BRANCH

# Merge your feature branch into main
git merge $FEATURE_BRANCH

# Push the changes to the remote repository
git push $REMOTE $MAIN_BRANCH
