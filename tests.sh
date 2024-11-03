#!/bin/sh

export ASAN_OPTIONS=detect_leaks=0 # XXX For now, let's not worry about leaks.
all_passed=1

for test in $(ls -p tests | grep -v /); do
	if [ $test = "import_helper.fl" ]; then
		continue
	fi

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
