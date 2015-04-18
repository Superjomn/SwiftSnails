set -x -e

root=/Users/apple/project/SwiftSnails
echo "root" $root

cd $root/
rm -f src.tar.gz
rm -rf src/apps/logistic_regression/bin/*
rm -rf src/apps/word2vec/bin/*
tar cvf src.tar src
gzip src.tar
scp src.tar.gz chunwei@info01:~/
