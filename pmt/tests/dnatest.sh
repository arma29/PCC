#!/bin/bash
#chomd +x
# sed -i -e 's/\r$//' test.sh; ./test.sh
AHO="../bin/./pmt -a aho -c -p"
BM="../bin/./pmt -a bm -c"
SHO="../bin/./pmt -a sho -c"
SEL="../bin/./pmt -a sel -c"
WU="../bin/./pmt -a wu -c"

#BM
    #BM
    for i in $(seq 1 7)
    do

        PAT='../data/patdna'
        END='.txt'
        START=$(date +%s.%N)

        NOW=$PAT$i$END

        ($BM -p $NOW ../data/dna.txt)

        END=$(date +%s.%N)
        DIFF=$(echo "$END - $START" | bc)
        (wc -c $NOW)
        echo $DIFF
    done

    #SHO
    for i in $(seq 1 7)
    do

        PAT='../data/patdna'
        END='.txt'
        START=$(date +%s.%N)

        NOW=$PAT$i$END

        ($SHO -p $NOW ../data/dna.txt)

        END=$(date +%s.%N)
        DIFF=$(echo "$END - $START" | bc)
        (wc -c $NOW)
        echo $DIFF
    done

    (echo "grep results for exact")
    for i in $(seq 1 7)
    do

        PAT='../data/patdna'
        END='.txt'
        START=$(date +%s.%N)

        NOW=$PAT$i$END

        (grep -c -f $NOW ../data/dna.txt)

        END=$(date +%s.%N)
        DIFF=$(echo "$END - $START" | bc)
        (wc -c $NOW)
        echo $DIFF
    done



    (echo "agrep results for exact")
    #($TIME agrep -c -f $TESTE2)
    for i in $(seq 1 7)
    do

        PAT='../data/patdna'
        END='.txt'
        START=$(date +%s.%N)

        NOW=$PAT$i$END

        (agrep -c -f $NOW ../data/dna.txt)

        END=$(date +%s.%N)
        DIFF=$(echo "$END - $START" | bc)
        (wc -c $NOW)
        echo $DIFF
    done




#AHO
    for i in $(seq 1 7)
    do

        PAT='../data/dicdna'
        END='.txt'
        START=$(date +%s.%N)

        NOW=$PAT$i$END

        ($AHO $NOW ../data/dna.txt)

        END=$(date +%s.%N)
        DIFF=$(echo "$END - $START" | bc)
        (wc -l $NOW) #lines
        echo $DIFF
    done

    (echo "grep results for paralel")
    for i in $(seq 1 7)
    do

        PAT='../data/dicdna'
        END='.txt'
        START=$(date +%s.%N)

        NOW=$PAT$i$END

        (grep -c -f $NOW ../data/dna.txt)

        END=$(date +%s.%N)
        DIFF=$(echo "$END - $START" | bc)
        (wc -l $NOW) #lines
        echo $DIFF
    done




 #SEL
     for i in $(seq 1 7)
     do

         PAT='../data/patdna'
         END='.txt'
         START=$(date +%s.%N)

         NOW=$PAT$i$END

         ($SEL -p $NOW ../data/dna.txt)

         END=$(date +%s.%N)
         DIFF=$(echo "$END - $START" | bc)
         (wc -c $NOW)
         echo $DIFF
     done
# ($TIME agrep -1 -c $TESTE1)
#
# #wu
#WU
    for i in $(seq 1 7)
    do

        PAT='../data/patdna'
        END='.txt'
        START=$(date +%s.%N)

        NOW=$PAT$i$END

        ($WU -p $NOW ../data/dna.txt)

        END=$(date +%s.%N)
        DIFF=$(echo "$END - $START" | bc)
        (wc -c $NOW)
        echo $DIFF
    done

    (echo "agrep results for aprox")
    for i in $(seq 1 7)
    do

        PAT='../data/patdna'
        END='.txt'
        START=$(date +%s.%N)

        NOW=$PAT$i$END

        (agrep -c -f $NOW ../data/dna.txt)

        END=$(date +%s.%N)
        DIFF=$(echo "$END - $START" | bc)
        (wc -c $NOW)
        echo $DIFF
    done
