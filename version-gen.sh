#/bin/sh
git log --pretty=format:"#define GIT_REVISION \"%H\\0\" %n#define GIT_REVISION_ABBR \"%h\\0\" %n#define GIT_COMMIT_ADATE \"%ai\\0\" %n#define GIT_COMMIT_AT %at" -1 > ./resources/revision.h
