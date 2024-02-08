#!/bin/bash

# Set the main branch name
MAIN_BRANCH="main"

# Function to display the help menu
display_help() {
    echo "Usage: $0 [option]"
    echo "Options:"
    echo "  -h                Display this help message."
    echo "  1                 Pull the latest changes from the current branch."
    echo "  2                 Push the current branch to the remote repository."
    echo "  3                 Update $MAIN_BRANCH branch with the latest changes from the remote."
    echo "  4                 Start a new feature branch based on the latest $MAIN_BRANCH."
    echo "  5                 Update your current feature branch with the latest changes from $MAIN_BRANCH."
    echo "  6                 Push your feature branch to the remote repository."
    echo "  7                 Stash any work-in-progress changes."
    echo "  8                 Check out a remote branch."
    echo "  9                 Create a pull request."
    echo "  10                Merge your feature branch into $MAIN_BRANCH locally."
    echo "  11                Delete a branch."
    echo ""
    echo "Regular situations to use each option:"
    echo "  1) When you want to update your current branch with the latest changes from the remote repository."
    echo "  2) When you want to push your local changes to the remote repository."
    echo "  3) Before starting new work or to regularly sync your main branch."
    echo "  4) When starting work on a new feature or task."
    echo "  5) Periodically during development, or before merging to ensure compatibility."
    echo "  6) When your feature is ready for review or needs to be shared with others."
    echo "  7) Before updating your branch (main or feature), it might be useful to stash any work-in-progress changes to prevent merge conflicts."
    echo "  8) This could be handy if you need to switch to a branch that someone else has started without creating a new local branch from scratch."
    echo "  9) When you're ready to merge your feature branch into the main branch, you can create a pull request to start the review process."
    echo "  10) Before pushing changes, you might want to merge your feature branch into your local main branch to test if the merge would be successful."
    echo "  11) After a feature branch has been successfully merged and is no longer needed, you might want to delete it to keep the repository clean."
    exit 0
}

pull_current_branch() {
    echo "Pulling the latest changes from the current branch..."
    git pull
    echo "The current branch has been updated with the latest changes from the remote repository."
}



push_current_branch() {
    current_branch=$(git rev-parse --abbrev-ref HEAD)

    # Check for uncommitted changes
    if git diff-index --quiet HEAD --; then
        echo "No changes to commit on $current_branch branch."
    else
        # Prompt for a commit message
        echo "Enter your commit message:"
        read commit_message

        # Add all changes to staging
        git add .

        # Commit the changes
        git commit -m "$commit_message"
    fi

    # Push the current branch to the remote repository
    echo "Pushing local changes in the $current_branch branch to the remote repository..."
    git push
    echo "Local changes in $current_branch have been pushed to the remote."
}


update_main_branch() {
    echo "Switching to the $MAIN_BRANCH branch..."
    git checkout $MAIN_BRANCH
    echo "Pulling the latest changes from the remote repository..."
    git pull origin $MAIN_BRANCH
    echo "$MAIN_BRANCH is now up to date."
}


start_new_feature() {
    echo "Enter the name for your new feature branch:"
    read feature_branch
    echo "Creating a new feature branch: $feature_branch from $MAIN_BRANCH..."
    git checkout -b "$feature_branch" $MAIN_BRANCH
    echo "New feature branch $feature_branch has been created and checked out."
}


update_feature_branch() {
    echo "Enter the name of your feature branch to update it with $MAIN_BRANCH:"
    read feature_branch
    echo "Checking out $feature_branch..."
    git checkout "$feature_branch"
    echo "Merging $MAIN_BRANCH into $feature_branch..."
    git merge $MAIN_BRANCH
    echo "$feature_branch is now up to date with $MAIN_BRANCH."
}


push_feature_branch() {
    echo "Enter the name of your feature branch to push it to the remote repository:"
    read feature_branch
    echo "Pushing $feature_branch to the remote repository..."
    git push origin "$feature_branch"
    echo "$feature_branch has been successfully pushed to the remote."
}

stash_changes() {
    echo "Stashing any work-in-progress changes..."
    git stash push -m "Stashed by Git script"
    echo "Changes stashed."
}

checkout_remote_branch() {
    echo "Enter the name of the remote branch you want to check out:"
    read remote_branch
    echo "Checking out remote branch $remote_branch..."
    git fetch
    git checkout "$remote_branch"
    echo "Switched to $remote_branch."
}

create_pull_request() {
    echo "This functionality requires external CLI tools like 'hub' for GitHub."
    # Example using GitHub's 'hub' tool
    # hub pull-request -m "Your PR message"
}

merge_feature_into_main() {
    echo "Enter the name of your feature branch to merge it into $MAIN_BRANCH locally:"
    read feature_branch
    git checkout $MAIN_BRANCH
    git merge "$feature_branch"
    echo "$feature_branch has been merged into $MAIN_BRANCH locally."
}

delete_branch() {
    echo "Enter the name of the branch you want to delete:"
    read branch_name
    echo "Deleting local branch $branch_name..."
    git branch -d "$branch_name"
    echo "Deleting remote branch $branch_name..."
    git push origin --delete "$branch_name"
    echo "$branch_name has been deleted both locally and remotely."
}

# Check for command-line arguments
if [[ $# -gt 0 ]]; then
    case $1 in
        -h) display_help ;;
        1) pull_current_branch ;;
        2) push_current_branch ;;
        3) update_main_branch ;;
        4) start_new_feature ;;
        5) update_feature_branch ;;
        6) push_feature_branch ;;
        7) stash_changes ;;
        8) checkout_remote_branch ;;
        9) create_pull_request ;;
        10) merge_feature_into_main ;;
        11) delete_branch ;;
        *) echo "Invalid option selected." ;;
    esac
    exit 0
fi

# Main menu for interactive use (no arguments provided)
echo "Select an action:"
echo "1) Pull the latest changes from the current branch"
echo "2) Push the current branch to the remote repository"
echo "3) Update $MAIN_BRANCH branch"
echo "4) Start a new feature branch"
echo "5) Update feature branch with $MAIN_BRANCH"
echo "6) Push feature branch to remote"
echo "7) Stash changes"
echo "8) Check out a remote branch"
echo "9) Create a pull request"
echo "10) Merge feature branch into $MAIN_BRANCH"
echo "11) Delete a branch"
read action

case $action in
    1) pull_current_branch ;;
    2) push_current_branch ;;
    3) update_main_branch ;;
    4) start_new_feature ;;
    5) update_feature_branch ;;
    6) push_feature_branch ;;
    7) stash_changes ;;
    8) checkout_remote_branch ;;
    9) create_pull_request ;;
    10) merge_feature_into_main ;;
    11) delete_branch ;;
    *) echo "Invalid option selected." ;;
esac
