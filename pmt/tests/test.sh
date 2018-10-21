#!/bin/bash

AHO="../bin/./pmt -a aho -c -p"
BM="../bin/./pmt -a bm -c"
SEL="../bin/./pmt -a sel -c"
WU="../bin/./pmt -a wu -c"

#for other

#TESTE4="pour ../data/fr1.txt"
TESTE2="../data/pateng2.txt ../data/eng1.txt" #8
TESTE4="../data/pateng4.txt ../data/eng1.txt" #13
TESTE5="../data/pateng5.txt ../data/eng1.txt" #22
TESTE6="../data/pateng6.txt ../data/eng1.txt" #33
TESTE7="../data/pateng7.txt ../data/eng1.txt" #43
TESTE0="../data/pateng0.txt ../data/eng1.txt" #54
TESTE8="../data/pateng8.txt ../data/eng1.txt" #111
TESTE9="../data/pateng9.txt ../data/eng1.txt" #193



#for aho
TESTE1="../data/pateng1.txt ../data/eng1.txt"
TESTE3="../data/pateng3.txt ../data/eng3.txt"



TIME="/usr/bin/time -f "%e""
TIME2="ts=$(date +%s%N)"
# TIME3=";tt=$((($(date +%s%N) - $ts)/1000000))"
# TIME4=";echo "Time taken: $tt milliseconds""




#BM
    #BM
    for i in 2 4 5 6 7 0 8 9
    do

        PAT='../data/pateng'
        END='.txt'
        START=$(date +%s.%N)

        NOW=$PAT$i$END

        ($BM -p $NOW ../data/eng1.txt)

        END=$(date +%s.%N)
        DIFF=$(echo "$END - $START" | bc)
        (wc -c $NOW)
        echo $DIFF
    done


    (echo "grep")
    for i in 2 4 5 6 7 0 8 9
    do

        PAT='../data/pateng'
        END='.txt'
        START=$(date +%s.%N)

        NOW=$PAT$i$END

        (grep -c -f $NOW ../data/eng1.txt)

        END=$(date +%s.%N)
        DIFF=$(echo "$END - $START" | bc)
        (wc -c $NOW)
        echo $DIFF
    done



    (echo "agrep")
    #($TIME agrep -c -f $TESTE2)
    for i in 2 4 5 6 7 0 8 9
    do

        PAT='../data/pateng'
        END='.txt'
        START=$(date +%s.%N)

        NOW=$PAT$i$END

        (agrep -c -f $NOW ../data/eng1.txt)

        END=$(date +%s.%N)
        DIFF=$(echo "$END - $START" | bc)
        (wc -c $NOW)
        echo $DIFF
    done


#AHO
    for i in $(seq 1 8)
    do

        PAT='../data/dic'
        END='.txt'
        START=$(date +%s.%N)

        NOW=$PAT$i$END

        ($AHO $NOW ../data/eng1.txt)

        END=$(date +%s.%N)
        DIFF=$(echo "$END - $START" | bc)
        (wc -l $NOW) #lines
        echo $DIFF
    done


 #SEL
     for i in 2 4 5 6 7 0 8 9
     do

         PAT='../data/pateng'
         END='.txt'
         START=$(date +%s.%N)

         NOW=$PAT$i$END

         ($SEL -p $NOW ../data/eng1.txt)

         END=$(date +%s.%N)
         DIFF=$(echo "$END - $START" | bc)
         (wc -c $NOW)
         echo $DIFF
     done
# ($TIME agrep -1 -c $TESTE1)
#
# #wu
#WU
    for i in 2 4 5 6 7 0 8 9
    do

        PAT='../data/pateng'
        END='.txt'
        START=$(date +%s.%N)

        NOW=$PAT$i$END

        ($WU -p $NOW ../data/eng1.txt)

        END=$(date +%s.%N)
        DIFF=$(echo "$END - $START" | bc)
        (wc -c $NOW)
        echo $DIFF
    done
