#!/bin/sh
 
git filter-branch -f --env-filter '
 
an="$GIT_AUTHOR_NAME"
am="$GIT_AUTHOR_EMAIL"
cn="$GIT_COMMITTER_NAME"
cm="$GIT_COMMITTER_EMAIL"
 
if [ "$GIT_COMMITTER_NAME" = "Marcus Nasarek" ]
then
    cn="Rheik van Eyck"
    cm="rheikvaneyck@yahoo.de"
fi
if [ "$GIT_AUTHOR_NAME" = "Marcus Nasarek" ]
then
    an="Rheik van Eyck"
    am="rheikvaneyck@yahoo.de"
fi
 
export GIT_AUTHOR_NAME="$an"
export GIT_AUTHOR_EMAIL="$am"
export GIT_COMMITTER_NAME="$cn"
export GIT_COMMITTER_EMAIL="$cm"
'
