#!/bin/bash

# Define the remote repository name, usually 'origin'
REMOTE="origin"

# Define the main branch name, usually 'main' or 'master'
MAIN_BRANCH="main"

# Fetch the latest changes from the remote repository
git fetch $REMOTE

# Update the local main branch
echo "Updating the local $MAIN_BRANCH branch..."
git checkout $MAIN_BRANCH
git pull $REMOTE $MAIN_BRANCH

# Get a list of all local branches except for 'main'
BRANCHES=$(git branch | grep -v "\*" | grep -v "$MAIN_BRANCH")

# Update each branch with changes from main
for BRANCH in $BRANCHES; do
    echo "Updating branch $BRANCH with changes from $MAIN_BRANCH..."
    git checkout $BRANCH
    git merge $MAIN_BRANCH
done

# Switch back to the main branch
git checkout $MAIN_BRANCH

echo "All branches have been updated with the latest changes from $MAIN_BRANCH."
