# Git Cheat Sheet

## 1. Git Configuration

```bash
git config --global user.name "Your Name"    # Set the name
git config --global user.email "your.email@example.com"   # Set the email
git config --global core.editor "nano"    # Set the default editor
git config --global color.ui auto         # Enable color in output
```

## 2. Initializing a Git Repository

```bash
git init                                # Initialize a new Git repository
git clone <repository-url>               # Clone an existing repository
```

## 3. Working with Branches

```bash
git branch                              # List local branches
git branch <branch-name>                # Create a new branch
git checkout <branch-name>              # Switch to a branch
git checkout -b <branch-name>           # Create and switch to a new branch
git merge <branch-name>                 # Merge a branch into the current branch
git branch -d <branch-name>             # Delete a local branch
git branch -a                          # List all branches (local and remote)
```

## 4. Managing Files

```bash
git status                             # View the status of modified files
git add <file>                          # Add a file to the staging area
git add .                               # Add all modified files
git rm <file>                           # Remove a tracked file
git commit -m "message"                # Commit changes with a message
git commit -a                          # Commit all tracked changes
git diff                               # Show local changes not yet committed
git diff --staged                      # Show changes added to the staging area
```

## 5. Managing Commits

```bash
git log                                # View commit history
git log --oneline                       # View commit history in one line
git log --graph --oneline --all         # View history with a graph
git reset <commit-id>                  # Reset to a specific commit (unstage files)
git reset --hard <commit-id>           # Hard reset to a commit (discard local changes)
git revert <commit-id>                 # Revert a commit without losing history
```	
	
## 6. Managing Remote Repositories

```bash
git remote -v                          # View configured remotes
git remote add <name> <url>            # Add a new remote
git remote remove <name>               # Remove a remote
git fetch <remote-name>                # Fetch changes from a remote without merging
git pull <remote-name> <branch>        # Fetch and merge changes
git push <remote-name> <branch>        # Push commits to a remote branch
git push origin --delete <branch>     # Delete a remote branch
```	
	
## 7. Managing Tags

```bash
git tag                                # List tags
git tag <tag-name>                     # Create a tag
git tag -a <tag-name> -m "message"     # Create an annotated tag
git push origin <tag-name>             # Push a tag to the remote repository
git push --tags                        # Push all tags
git tag -d <tag-name>                  # Delete a local tag
git push origin --delete <tag-name>    # Delete a remote tag
```
	
## 8. Stashing (Saving Changes Temporarily)

```bash
git stash                              # Save local changes to a stash
git stash list                         # View the list of stashes
git stash apply                        # Apply the most recent stash
git stash pop                          # Apply and remove the most recent stash
git stash drop                         # Drop a specific stash
```

## 9. Collaborating on Pull Requests

```bash
git fetch                              # Fetch latest changes from a pull request
git checkout -b <new-branch> origin/<pr-branch>  # Create a branch for a PR
git push origin <new-branch>           # Push the PR branch
```

## 	10. Rewriting History (Advanced)

```bash
git rebase -i <commit-id>              # Interactive rebase (modify history)
git rebase --abort                     # Abort an ongoing rebase
git cherry-pick <commit-id>            # Apply a specific commit from another branch
```

## 11. Viewing the Tree and Differences

```bash
git log --oneline --graph              # Display commit history as a graph
git log --oneline --graph --all        # Display history of all branches
git diff <file>                        # Compare file with the last committed version
git show <commit-id>                   # View the details of a specific commit
```

## 12. Other Useful Commands

```bash
git clean -fd                          # Remove untracked files
git ls-files                           # List all tracked files
git mv <old-name> <new-name>           # Rename a file
git blame <file>                       # See who modified each line of a file
git archive --format=tar <archive-name> # Create an archive of a commit
```
