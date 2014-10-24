from git import *
import os
import sys
import argparse

parser = argparse.ArgumentParser(description='Helper for multiple Git Repository')
parser.add_argument('-r', '--repo', metavar='N', nargs='+',
                   help='Select repositories')
parser.add_argument('-d', '--diff', help='Verifies if there are changes in the repo',
                    action='store_true')

args = parser.parse_args()

repos= [
  "service.website",
  "module.base",
  "module.core",
  "module.database",
  "module.inventory",
  "module.user",
  "module.utils",
  "module.logger",
];

if args.repo:
  repos=args.repo
cwd=os.getcwd();

gitRepos=[]
for i in repos:
  repoPath=cwd+"\\"+i
  try:
    repo=Repo(repoPath)
  except Exception, e:
    print('error: ',e)
    continue
  gitRepos.append(repo.git)

if args.diff:
  for git in gitRepos:
    diff=git.diff()
    if len(diff) > 0:
      print(i);
