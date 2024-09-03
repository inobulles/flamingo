#!/bin/sh

all_passed=1

for test in $(ls tests); do
	echo -n "Running test $test... "
	bin/flamingo tests/$test > /dev/null

	if [ $? = 0 ]; then
		echo "PASSED"
	else
		echo "FAILED"
		all_passed=0
	fi
done

if [ $all_passed = 0 ]; then
	echo "TESTS FAILED!"
	exit 1
else
	echo "ALL TESTS PASSED!"
fi
