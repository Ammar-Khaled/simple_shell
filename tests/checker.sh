#!/bin/bash

name=$1

function check {
	echo "testing \"$1\""
	output=$(echo "$1" | $name)
	echo "state: $?"
	echo "output:"
	echo "$output"
	echo "======================"
}

check "/bin/ls"
check "/bin/ls"
check "   /bin/ls   "

cp /bin/ls ./my_ls
check "./my_ls"
rm ./my_ls
