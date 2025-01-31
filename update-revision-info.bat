@echo off
git --git-dir %1/.git log --pretty=format:"#define GIT_REVISION \"%%H\" %%n#define GIT_REVISION_ABBR \"%%h\" %%n#define GIT_COMMIT_ADATE \"%%ai\" %%n#define GIT_COMMIT_AT %%at" -1 > %1/resources/revision.h
