#/bin/sh
git log --pretty=format:"git-commit-info %h %ad" -1 > ./resources/version-included.txt
