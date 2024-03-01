#!/bin/bash

# Define variables
FEATURE_BRANCH="your-feature-branch-name"
MAIN_BRANCH="main"
REMOTE="origin"

# Update Feature Branch through Main and Update Remote through Main

# Step 1: Checkout and update the main branch
echo "Checking out and updating $MAIN_BRANCH..."
git checkout $MAIN_BRANCH
git pull $REMOTE $MAIN_BRANCH

# Step 2: Merge feature branch into main locally
echo "Merging $FEATURE_BRANCH into $MAIN_BRANCH..."
git merge $FEATURE_BRANCH
# Handle potential merge conflicts here manually or through a script

# Step 3: Push updated main to remote
echo "Pushing updated $MAIN_BRANCH to $REMOTE..."
git push $REMOTE $MAIN_BRANCH

# Step 4: Checkout feature branch and merge main into it
echo "Checking out and updating $FEATURE_BRANCH with changes from $MAIN_BRANCH..."
git checkout $FEATURE_BRANCH
git merge $MAIN_BRANCH
# Handle potential merge conflicts here manually or through a script

# Step 5: Optionally push feature branch updates to remote
echo "Pushing updated $FEATURE_BRANCH to $REMOTE..."
git push $REMOTE $FEATURE_BRANCH

echo "Process completed."
