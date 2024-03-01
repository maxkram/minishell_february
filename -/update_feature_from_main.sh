#!/bin/bash

# Define your main and feature branch names
MAIN_BRANCH="main"
FEATURE_BRANCH="execution-branch"

# Ensure you're on the feature branch
git checkout $FEATURE_BRANCH

# Fetch the latest changes from the remote and update the main branch
git fetch origin
git checkout $MAIN_BRANCH
git merge origin/$MAIN_BRANCH

# Merge the latest main into your feature branch
git checkout $FEATURE_BRANCH
git merge $MAIN_BRANCH

echo "$FEATURE_BRANCH is now updated with the latest changes from $MAIN_BRANCH."
