#!/bin/bash

# Fetch the latest branches information
git fetch origin

echo "Available branches:"
# List all remote branches and remove 'origin/' prefix
git branch -r | sed 's/origin\///' | nl

echo "Enter the number of the feature branch you want to select:"
read branch_num

# Select the branch based on user input
FEATURE_BRANCH=$(git branch -r | sed 's/origin\///' | sed -n "${branch_num}p")

echo "You have selected the branch: $FEATURE_BRANCH"

MAIN_BRANCH="main"

# Ensure the selected branch is checked out and up to date
git checkout $FEATURE_BRANCH
git pull origin $FEATURE_BRANCH

# Update feature branch with latest changes from main branch
git checkout $MAIN_BRANCH
git pull origin $MAIN_BRANCH
git checkout $FEATURE_BRANCH
git merge $MAIN_BRANCH

# If there are merge conflicts, prompt the user to resolve them before proceeding
# Note: Script does not automatically handle merge conflicts

# Add, commit, and push changes in feature branch (optional, based on need)
git add .
echo "Enter your commit message for the feature branch:"
read FEATURE_COMMIT_MESSAGE
git commit -m "$FEATURE_COMMIT_MESSAGE"
git push origin $FEATURE_BRANCH

# Switch to main branch and pull latest changes
git checkout $MAIN_BRANCH
git pull origin $MAIN_BRANCH

# Merge feature branch into main with a default message
MERGE_MESSAGE="Merge branch '$FEATURE_BRANCH' into '$MAIN_BRANCH'"
git merge $FEATURE_BRANCH --message "$MERGE_MESSAGE"

# Push main branch to remote
git push origin $MAIN_BRANCH

# Switch back to feature branch
git checkout $FEATURE_BRANCH

echo "Feature branch '$FEATURE_BRANCH' has been merged into '$MAIN_BRANCH' and pushed to remote with the message: $MERGE_MESSAGE"
