#!/bin/bash

COUNTER=0

# if there is a grading.txt or similar, remove it.
# The -f flag means that the file doesn't exist we don't get an error
rm -f ./grading.txt
rm -f ./tmp_test.txt

# We use sed to remove all lines that are empty ($) or start with # from the test file
# The result is stored in the file "tmp_test.txt" with the '>' command
# In the end of the while loop you can see that this file is used as input with '<'
sed '/^#/d' <tests.txt >tmp_test.txt
# Here I use -i, which means sed works in place. The result is that the first sed operation
# creates the tmp_test.txt with no lines starting with #, while the second command removes
# all empty lines
sed -i '/^$/d' tmp_test.txt

# If we wanted to do it in a single sed command we could do
# sed -e 's/#.*$//' -e '/^$/d' <tests.txt >tmp_test.txt
# but really, why not keep it simple? Focus on making the C code fast, not the shellscript!

# We could also use tools like awk or grep, none of them are very pleasent though.

while read p; do
    # While array indices start at 0, tests are numbered 1 and onwards...
    let COUNTER=COUNTER+1

    # We can use both echo and printf. printf is preferred, don't use echo like me
    printf "[TEST %s]\n" "$COUNTER"

    # awk, like sed, is a program used for handling text. Here I use awk to get the text before
    # and after " should be "
    programTestInput=$(awk -F" should be " '{print $1}' <<< $p )
    programExpectedAnswer=$(awk -F" should be " '{print $2}' <<< $p )

    # printf "\n"
    # echo $programTestInput
    # echo $programExpectedAnswer

    # The argument $1 is the first command line argument. In the makefile we call the test with our
    # output, "myProgram", thus in this case $1 = myProgram
    programOutput=$(./$1 ${programTestInput})
    # $? is the return value of the last run program. With exit code 1 progRet will be 1
    progRet=$?

    # We run our program with valgrind. We can, and should, add more args to valgrind.
    # Check out more arguments on the intertubes
    memOutput=$(valgrind --leak-check=full --error-exitcode=123 --log-file=log.txt ./$1 $programTestInput)
    memRet=$?

    # As with everything else, bash has obtuse syntax for if else
    if [ "$programOutput" == "$programExpectedAnswer" ]
       then
           echo "correct output"
    else
        echo "incorrect output"
        printf "[TEST %s] program error. expected was %s. returned was %s\n" "$COUNTER" "$programExpectedAnswer" "$programOutput" >> grading.txt
    fi

    # Here I could've used $progRet, but I want to show inline conditionals which require [[ and ]]
    if [ $memRet -eq 0 ]
    then
        echo "correct memory"
    elif [[ ($memRet -eq 1) && ($programExpectedAnswer == "PARSE ERROR") ]]
    then
        echo "correct memory"
    elif [ $memRet -eq 1 ]
    then
        echo "correct memory, incorrect program"
    elif [ $memRet -eq 123 ]
    then
        echo "memory error:"
        cat log.txt
    fi

    printf "\n"

    # In the valgrind args we used mem_log.txt for valgrind output, but we want to reset it between tests
    rm -f log.txt

done < tmp_test.txt
# The loop ends after done. the weird '<' operator basically states that the construct we just declared (the loop) should be run with tmp_test.txt as input.

# Always clean up after yourself
rm -f tmp_test.txt
