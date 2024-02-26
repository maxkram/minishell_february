#!/bin/bash

# Function to check for uncommitted changes
check_uncommitted_changes() {
    if ! git diff-index --quiet HEAD --; then
        echo "You have uncommitted changes in your current branch."
        echo "Would you like to stash them? (y/n)"
        read stash_choice
        if [ "$stash_choice" = "y" ]; then
            git stash push -m "Stashed changes before operation"
            echo "Changes stashed."
        else
            echo "Please commit or stash your changes manually before proceeding."
            exit 1
        fi
    fi
}

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

echo "Select the operation you want to perform:"
echo "1) Merge feature_branch -> local_main"
echo "2) Merge feature_branch -> remote_main"
echo "3) Update feature_branch with changes from remote_main"
echo "4) Update feature_branch with changes from local_main"
read operation

case $operation in
  1|2|3|4)
    check_uncommitted_changes
    ;;
esac

case $operation in
  1)
    # Merge feature branch into local main branch
    git checkout $MAIN_BRANCH
    git merge $FEATURE_BRANCH
    echo "Feature branch '$FEATURE_BRANCH' has been merged into local '$MAIN_BRANCH'."
    ;;
  2)
    # Merge feature branch into remote main branch (via local merge and push)
    git checkout $MAIN_BRANCH
    git pull origin $MAIN_BRANCH
    git merge $FEATURE_BRANCH
    git push origin $MAIN_BRANCH
    echo "Feature branch '$FEATURE_BRANCH' has been merged into remote '$MAIN_BRANCH' and changes have been pushed."
    ;;
  3)
    # Update feature branch with latest changes from remote main branch
    git checkout $FEATURE_BRANCH
    git pull origin $MAIN_BRANCH
    echo "Feature branch '$FEATURE_BRANCH' has been updated with changes from remote '$MAIN_BRANCH'."
    ;;
  4)
    # Update feature branch with latest changes from local main branch
    git checkout $FEATURE_BRANCH
    git merge $MAIN_BRANCH
    echo "Feature branch '$FEATURE_BRANCH' has been updated with changes from local '$MAIN_BRANCH'."
    ;;
  *)
    echo "Invalid operation selected."
    ;;
esac

# Optionally, add handling for merge conflicts and further actions after the operation.
