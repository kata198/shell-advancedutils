# shell-advancedutils
Advanced commands to extend SH/Bash shell scripting into a more powerful language.

These commands are very fast, and simply replace really advanced, ugly, and errorprone chaining of commands with simple keywords that read easily.


Full List of Programs
---------------------

**aslist** - Converts input to a space-separated variable. Input is either as arguments, or if no arguments frmo stdin

So, doing the following at the start of your program will easily make your program accept arguments on commandline for on stdin

	ARGS=$(aslist $@)


**bgtask** - Runs a task in the background

**cout**   - Collate all input before output. Use this, for example, to write back to a file being read, e.x. "sort fname | cout fname" -- It will collect all output from the 'sort' pipe, THEN open the file (whereas a builtin-redirect is opened before execution, so you'll end up with a null file)


**dirnamen** - Discovers information about the Nth parent directory. First argument is the directory name, second is number of levels.

output is two lines, the first line is the dirname N levels up (so 1 is parent directory, 2 is parent-of-parent, etc), and second line is the popped dirname.

Example:

	[tim glibc]$ pwd  # Show current directory
	/usr/src/arch/glibc

	[tim glibc]$ dirnamen "`pwd`" 2  # Get information two-levels-up
	/usr/src
	arch


**echoerr**  - Shortcut to echo to stderr

**nmtmp** - Generate a timed temporary file with a given key and optional timeout.

First arg is the key (you make one up specific to application), second is optional and timeout (default 60 seconds).

After timeout, file will automatically be deleted.

Prints filename on stdout, so you can gather like:

    MYNAME="$(nmtmp myprog 360)"  # Generate a temp file with key 'myprog' that deletes after 360 seconds and store filename in $MYNAME

Useful with gtmp

**gtmp** - Get the last temporary name created by nmtmp with a given key.

**isdigit** - Test if argument is a digit.

	if ( ! isdigit "${ARG}" );
	then
		echoerr "Argument must be an integer. Got: ${ARG}"
	fi

**isfloat** - Test if an argument is a floating point number (may contain decimal)

**isempty** - Check if a file is empty. Works with stdout and exit code, see --help

**isin** - Reads  value on stdin, and tests if any of the args match it.

	if ( echo "${ARG}" | isin "-h" "--help" "-?" );
	then
		printUsage;
	fi

**isin_nocase** - Same as *isin*, but case-insensitive

**notin** - Reads value on stdin, and tests that it does NOT match any of the args

	if ( echo "${ARG}" | notin "-a" "-b" "-c" "-d" );
	then
		echoerr "Invalid argument: ${ARG}";
	fi

**notin_nocase** - Same as *notin*, but case-insensitive

**listContains** - First argment is needle, remainder is haystack. Checks if needle is in haystack.

	if ( listContains "--help" "$@" );
	then
		printUsage;
	fi

**stripstr** - Strip whitespace from left and right side of a string

	[tim ~]$ STRIPPED="$(stripstr "   Hello  World   ")"

	[tim ~]$ printf "'%s'\n" "${STRIPED}"
	'Hello World'

**lstripstr** - Strip the left side of a string

	[tim ~]$ echo "   Hello world" | lstripstr
	Hello world

**rstripstr** - Strip the right side of a string

**noecho** - Run command without output

**prefixitems** - First argument is prefix, and remainder are strings to which the prefix will be prepended. -l to out one-per-line.

	[tim ~]$ prefixitems -l  "Cheese_" "one" "two" "three"
	Cheese_one
	Cheese_two
	Cheese_three

**printferr** - Shortcut to printf to stderr

**prompt** - Prompt for user input, and keep displaying prompt until input matches against a provided list of valid input

	[tim ~]$ DO_CONTINUE="$(prompt 'Continue? (y/n): ' 'y' 'Y' 'n' 'N')"
	Continue? (y/n): hello
	Continue? (y/n): bad
	Continue? (y/n): input
	Continue? (y/n): n
	[tim ~]$ echo ${DO_CONTINUE}
	n

**readall** - Reads all input from stdin into a variable, multiple-lines. Kind of like "cat --", but includes escaping options. See --help

**sortByCol** - Sorts input on stdin based on a 1-origin column number. See --help for options.

	[tim ~]$ ps auxh | sortByCol --int 2   # Sorts output by second column (pid), to display all processes in ascending order by PID

**splitContains** - Split a string by a token, and determine if any of the resulting elements match a needle. NOTE: Args start with --% and that is NOT a typo, that is so you can test args without overlap.

	if ( splitContains "${AND_ARGS}" '&' 'ARG_MAGIC' );
	then
		HAS_ARG_MARGIC=1
	else
		HAS_ARG_MAGIC=0
	fi

**strerror** - Takes an error code ( $? ) and converts it to the POSIX-defined string for that number. Many numbers have no defined meaning, and programs can use any exit code they want (supposed to use in the unassigned range for their own meanings, but many programs don't. )

Example:

	do_something;
	RET=$?

	if [ ${RET} -ne 0 ];
	then
		printferr "Got error executing 'do_something': Error %s\n" "$(strerror ${RET})"
	fi

Output:

	Got error executing 'do_something': Error 24 - Too many open files

**strlen** - Takes input either on stdin or on arguments. Use this instead of wc -c, because this behaves better with terminal applications. i.e. it does not count the trailing newline if there is one.

See the potential off-by-one error below:

Expect "abc" to have strlen of 3:

	[tim ~]$ echo "abc" | wc -c   # WRONG!
	4
	[tim ~]$ echo "abc" | strlen  # RIGHT!
	3

Show that newlines ARE counted if found inside the string:

	[tim ~]$ echo -e "abc\ndef" | wc -c
	8
	[tim ~]$ echo -e "abc\ndef" | strlen
	7


**te** - Performs a test (via /bin/test), but outputs "true" or "false" in addition to having exit code. This is useful in some circumstances.

	[tim ~]$ IS_EQUAL=$(te 1 -eq 2)
	[tim ~]$ echo "${IS_EQUAL}"
	false

**teq** - "te" QUIET. Not sure why I wrote this, it's basically same as "test"


**sau-activate.sh** - This file is automatically added to /etc/profile.d and will be sourced for interactive sessions. It implements several functions. TODO: Document these


Error Messages (strerror)
========================

Ever get a return code from an application, like 52, and not know what the hell that means?

Well, *strerror* is the solution! It will take a standard POSIX exit code, and print the associated message!

NOTE: Applications are free to define and use their own exit codes. Standard system utils should follow the POSIX standards, however.

Example:

	[tim ]$ strerror 52
	52 - Invalid exchange

	[tim ]$ strerror 1
	1 - Operation not permitted

	[tim ]$ strerror 4
	4 - Interrupted system call

	[tim ]$ strerror 22
	22 - Invalid argument

	[tim ]$ strerror 0
	0 - Success


This is very useful for things like:

	tar -cf "mytar.tar" "${FILES}"
	RET=$?

	if [ $RET -ne 0 ];
	then
		echo "Failed to create 'mytar.tar'. Error: $(strerror ${RET})" >&2
	fi



Other Productivity Tools
========================

Here are links to other tools I use daily for advanced shell scripting and direct problem solving:


**setutils** - Set operations using 2+ files (difference, intersection, union)

https://github.com/kata198/setutils


**popLines** - Pops lines from files; makes simple the art of using files as queues.

https://github.com/kata198/popLines


**findProcessesUsing** - Scans all running processes for a mapping of a shared object (library), or a specific file, or a given directory or anything beneath it

https://github.com/kata198/findProcessesUsing


**myps2** - Quick commands to filter processes and pids without chaining a ton of greps/awks

https://github.com/kata198/myps2


**bash-resume** - Simple pure-shell extension which can support tracking command success, and allows you to resume a script that failed at the failing point. Kind of like a Makefile for general bash (or most /bin/sh implementors)

https://github.com/kata198/bash-resume



**disttask** - Tool to take advantage of multiprocessing with existing tasks. A Process job queue.

https://github.com/kata198/disttask


**remote\_copy\_and\_execute** - Copies-and-executes a provided script and arguments to a list of hosts. Can do in serial, in parallel, and many other options.

https://github.com/kata198/remote_copy_and_execute
