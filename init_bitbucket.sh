#!/bin/sh
# copybitbucket - creates remote bitbucket repo and adds it as git remote to cwd

    echo 'Username?'
    read username
    echo 'Password?'
    read -s password  # -s flag hides password text
    echo 'Repo name?'
    read reponame

    curl --user $username:$password https://api.bitbucket.org/1.0/repositories/ --data name=$reponame --data is_private='true'
    
    git remote rename origin upstream
    git remote add origin git@bitbucket.org:$username/$reponame.git
    git push -u origin --all
    git push -u origin --tags
