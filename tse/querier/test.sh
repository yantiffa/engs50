#!/bin/bash

OUTPUT=testing.out
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
