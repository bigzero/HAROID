You could checkout directly the remote branch after fetching it 
git fetch origin
git branch -f remote_branch_name origin/remote_branch_name
git checkout remote_branch name

 or shorter:
git checkout -b production origin/production

 You would be directly working from the fetched copy of origin/production branch (no conflict there).

By doing
git co -b production
git pull origin production

 You are trying to merge the remote production branch into your master (in a local 'production' branch, which means potentially conflicts if your master has locally a different history than the remote origin/production branch.
