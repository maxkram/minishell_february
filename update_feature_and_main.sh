#!/bin/bash

# Define variables for branch names and remote
FEATURE_BRANCH="execution-branch"
MAIN_BRANCH="main"
REMOTE="origin"

# Step 1: Update local main branch with the latest changes from remote
echo "Updating $MAIN_BRANCH branch..."
git checkout $MAIN_BRANCH
git pull $REMOTE $MAIN_BRANCH

# Step 2: Switch to your feature branch
echo "Switching to $FEATURE_BRANCH branch..."
git checkout $FEATURE_BRANCH

# Step 3: Merge the latest changes from main into your feature branch
echo "Merging changes from $MAIN_BRANCH into $FEATURE_BRANCH..."
git merge $MAIN_BRANCH

# Step 4: Push the updated feature branch to the remote repository
echo "Pushing updated $FEATURE_BRANCH to $REMOTE..."
git push $REMOTE $FEATURE_BRANCH

echo "Your $FEATURE_BRANCH is now up to date with $MAIN_BRANCH, and the changes have been pushed to $REMOTE."
