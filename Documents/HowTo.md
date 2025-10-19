
# git configuration
git config --global user.name "<username>"
git config --global user.email "<email address>
git config --global init.default branch main

# git create branch

# git commit

# git push

# git pull



# How to build 

For the test
cmake -DTARGET_GROUP=test .. 
make

For the debug
cmake -DTARGET_GROUP=debug .. 
make

For the release
cmake -DTARGET_GROUP=release .. 
make

# How to run the test 

1. build the test binaries

cmake -DTARGET_GROUP=test .. 
make

2. run the test suite/sequence

ctest --verbose    


