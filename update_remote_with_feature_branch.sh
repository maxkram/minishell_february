#!/bin/bash

# Define branches
FEATURE_BRANCH="execution-branch"
MAIN_BRANCH="main"

# Ensure current branch is $FEATURE_BRANCH and it's up to date
git checkout $FEATURE_BRANCH
git pull origin $FEATURE_BRANCH

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

echo "Feature branch '$FEATURE_BRANCH' has been merged into '$MAIN_BRANCH' and pushed to remote with the message: $MERGE_MESSAGE"
