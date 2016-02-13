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


**notin**

  Takes a value on stdin. Returns true (0) if that value is not present as any of the given arguments.

	(echo "error" | notin $RESULTS1 $RESULTS2) && doSuccess;

The above script checks if the word "error" occurs in either of the strings $RESULTS1 or $RESULTS2. If it does not, "doSuccess" is called.


