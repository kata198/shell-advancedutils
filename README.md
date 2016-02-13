# shell-advancedutils
Advanced commands to extend SH/Bash shell scripting into a more powerful language.

These commands are very fast, and simply replace really advanced, ugly, and errorprone chaining of commands with simple keywords that read easily.


Testing
=======

The following commands add more advanced testing facilities, and are expected to be use in conditionals (if).


**isin**

  Takes a value on stdin. Returns true (0) if any of the arguments matches that value exactly. Otherwise, retuns false (1).

	if echo "--help" | isin $@;
	then
		printUsage;
		exit 1;
	fi

The above example script checks if any of the arguments ($@) was "--help".

**isin\_nocase**

  Same as isin, but case insensitive.

**notin**

  Takes a value on stdin. Returns true (0) if that value is not present as any of the given arguments.

	(echo "error" | notin $RESULTS1 $RESULTS2) && doSuccess;

The above script checks if the word "error" occurs in either of the strings $RESULTS1 or $RESULTS2. If it does not, "doSuccess" is called.

**notin\_nocase**

  Same as notin, but case insensitive..


Performance
===========

Tools are written in C, and only do their direct job.

Here is a benchmark on an unloaded system, showing 42x+ (.001s is rounded up) speed for a simple match. On a loaded system, this could be much higher:

	[tim shell-advancedutils]$ time ( echo "--help" | ./bin/isin "--help" "abc" "arg3" ); echo "Result: $?"

	real    0m0.001s
	user    0m0.000s
	sys     0m0.000s
	Result: 0


	[tim shell-advancedutils]$ time (echo "--help" | python -c 'import sys; x = sys.stdin.read().split(); sys.exit(int(not bool("--help" in sys.argv[1:])))' --help abc arg3); echo "Result: $?"

	real    0m0.042s
	user    0m0.027s
	sys     0m0.013s
	Result: 0

