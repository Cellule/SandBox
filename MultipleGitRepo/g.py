from git import *
import os
import sys
import argparse

parser = argparse.ArgumentParser(description='Helper for multiple Git Repository')
parser.add_argument('-r', '--repo', metavar='N', nargs='+',
                   help='Select repositories')
parser.add_argument('-d', '--diff', help='Verifies if there are changes in the repo',
                    action='store_true')
parser.add_argument('-p', '--pull', help='Pulls every repo in their current branch',
                    action='store_true')
parser.add_argument(
  '-c', '--checkout',
  help='checkouts every repo to this specified branch',
  metavar='branch'
)

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
  gitRepos.append((repo.git,i))

for (git,repo) in gitRepos:
  print('Executing repo :'+repo)
  if args.diff:
    diff=git.diff()
    if len(diff) > 0:
      print(repo+" has changes");
  if args.checkout:
    try:
      print(git.checkout(args.checkout))
    except Exception, e:
      print(repo,e)
  if args.pull:
    try:
      print(git.pull())
    except Exception, e:
      print(repo,e)


