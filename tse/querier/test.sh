#!/bin/bash

OUFILE=testing.out
rm -f testing.out

tests() {
		echo "-------------------------------------------------------------" | tee -a $OUTFILE
		echo "$1" | tee -a $OUTFILE
		echo "-------------------------------------------------------------" | tee -a $OUTFILE
		echo "" | tee -a $OUTFILE
}

#command line argument tests
#test 1:invalid number of parameters
tests "test 1:invalid number of parameters (should fail)"
querier 2>&1 | tee -a $OUTFILE
echo "" | tee -a $OUTFILE

#test 2: directory that has not been crawled
tests "test 2: directory that has not been crawled"
querier ../fake ../indexer/indexnum 2>&1 | tee -a $OUTFILE
echo "" | tee -a $OUTFILE

#test3: directory that is not accessible
tests "test 3: directory that is not accessible "
mkdir -p ../notaccessible
chmod 000 ../notaccessible
querier ../notaccessible ../indexer/indexnum 2>&1 | tee -a $OUTFILE
chmod 755 ../notaccessible
rmdir ../notaccessible
echo "" | tee -a $OUTFILE

#test 4: an index file that is not readable
tests "test 4:an index file that is not readable"
querier ../pages ../indexer/indexnusss 2>&1 | tee -a $OUTFILE
echo "" | tee -a $OUTFILE

#test 5: a large indexfile
tests "test 5: a large indexfile"
echo "dartmouth" | querier ../pages ../indexer/indexnum 2>&1 | tee -a $OUTFILE
echo "" | tee -a $OUTFILE

# Testing stdin (invalid inputs)
#test 6: Empty
tests "#test 6: Empty"
cat > test6.txt << 'EOF'


EOF
querier ../pages ../indexer/indexnum < test6.txt 2>&1 | tee -a $OUTFILE
echo "" | tee -a $OUTFILE

#test 7: tab or enter then EOF
tests "test 7: tab or enter then EOF"
printf"\t\n" | querier ../pages ../indexer/indexnum 2>&1 | tee -a $OUTFILE
echo "" | tee -a $OUTFILE

#test 8: a single word that does not exist
tests "test 8: a single word that does not exist"
echo "jiskald" | querier ../pages ../indexer/indexnum 2>&1 | tee -a $OUTFILE
echo "" | tee -a $OUTFILE

#test 9: a valid word or'd with an invalid word
tests "test 9: a valid word or'd with an invalid word"
echo "dartmouth or jiskald" | querier ../pages ../indexer/indexnum 2>&1 | tee -a $OUTFILE
echo "" | tee -a $OUTFILE

#test 10: a valud word and'd with an invalid word
tests "test 10: a valud word and'd with an invalid word"
echo "dartmouth and jiskald" | querier ../pages ../indexer/indexnum 2>&1 | tee -a $OUTFILE
echo "" | tee -a $OUTFILE

#test 11: a combination of or's and and's and spaces and tabs separating valid and invalid words
tests "test 11: a combination of or's and and's and spaces and tabs separating valid and invalid words"
cat > test11.txt << 'EOF'
dartmouth and college or jiskald
  computer and    science
EOF
querier ../pages ../indexer/indexnum < test11.txt 2>&1 | tee -a $OUTFILE
echo "" | tee -a $OUTFILE

#test 12: and at the beginning
tests "test 12: and at the beginning"
echo "and dartmouth" | querier ../pages ../indexer/indexnum 2>&1 | tee -a $OUTFILE
echo "" | tee -a $OUTFILE

#test 13: or at the beginning
tests "test 13: or at the beginning"
echo "or dartmouth" | querier ../pages ../indexer/indexnum 2>&1 | tee -a $OUTFILE
echo "" | tee -a $OUTFILE

#test 14: tab then and at the beginning
tests "test 14: tab then and at the beginning"
printf "\tand dartmouth" |  querier ../pages ../indexer/indexnum 2>&1 | tee -a $OUTFILE
echo "" | tee -a $OUTFILE 
