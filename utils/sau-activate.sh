#!/bin/bash
##########################################################################
# Copyright (c) 2014 - 2017, 2018 Timothy Savannah All Rights Reserved
#
#  Licensed under terms of the LGPLv2
#
#########################################


# vim: set ts=4 sw=4 expandtab


##########################
#
#  Functions that exist only after sourcing this file:
#   ------------------------------------------------
#
# *temp workspaces*
#
#      tmp - Go to a special temp dir for your user
#
#    mktmp - Create a temp directory and cd to it (for scratch space).
#        @arg1 = Optional string that will be included at the beginning of the temp folder name
#
#    untmp - Go back to previous directory (cwd when 'mktmp' as called). Unless "-k" is specified, also remove the temp area "mktmp" created.
#        @arg1 = Pass "-k" to keep the temp directory
#
#

########################################
#
#  Functions that exist as standalone programs, but are redefined as functions
#    here (for performance/resource reasons)    ------------------------------
#   ----------------------------------------
#
# *Subprocess shorthands*
#
#   bgtask - Run given command in background with no output. E.x. "bgtask firefox www.kernel.org"
#
#   noecho - Run given command but silence stderr and stdout. E.x. "noecho grep RECORD1 /tmp/my.db || add-record.py RECORD1 /tmp/my.db"
#
#   noerr  - Run given command but silence stderr
#
#
# *Input/Argument Handling*
#
#   prompt - Prompt stdin for a string.
#     @arg1 = The string to prompt the user
#     @arg2..N = Any additional arguments, if provided, make up the set of "allowed" answers (e.x. prompt "Continue (y/n)? " "y" "Y" "n" "N")
#
#   readall -  Reads all data on stdin, and outputs to stdout. Use this to capture multi-line input in a shell script (like someone piping in a file),
#                like "all_data=$(readall)"
#     @arg1 - Optionally -e to "escape" the data (for use to later print with echo -e)
#
#
#   aslist - Converts data on stdin or as arguments into a space-separated list of arguments.

########################################
#
#  Functions that exist only as standalone programs
#   ----------------------------------------------
#
# *Conditionals*
#
#  isin - Compare data fed to stdin to check if contained in arguments.  E.x. "echo 'abc' | isin 'one' 'abc'" would be true (return 0)
#    @args - List of values in set to check
#
#  notin - Same as isin, but returns 0 if the data on stdin is NOT found in any of the arguments
#
#  splitContains - Usage: splitContains (Options) [needle] [splitBy] [haystack]
#     Splits the given "haystack" by "splitBy", and checks if one of the elements is "needle."
#     Run "splitContains" with no arguments for the full help message and additional options
#
#
# *Reformatting/Sorting*
#
#   sortByCol - Takes in data on stdin, and a 1-origin column number, and sorts by that column number.
#     See sortByCol --help for all options and usage
#
#
#
# *Named Auto-destroy temp files*
#
#  (Example:   echo "Hello World" > `nmtmp mymsg 30`; cat `gtmp mymsg`  )
#
#   ^ This example outputs a string to a file named with the handle "mymsg" which will self-destruct in 30 seconds.
#   We use "gtmp" to fetch the filename assigned to the 'mymsg' handle.
#
#    nmtmp - Create a temp file that will self-destruct after a given timeout. Outputs the name of the temp file.
#      @arg1 - Handle name
#      @arg2 - timeout (seconds) [default 60]
#
#   gtmp   - Fetch a named temp file
#



tmp() {
    if [ -z "$_TMP_OLD_DIR" ];
    then
        export _TMP_OLD_DIR=$(pwd)
    fi

    MYTMPDIR="/tmp/shtmp-`whoami`";

    mkdir -p $MYTMPDIR

    cd $MYTMPDIR;
}

untmp() {
    if [ "$1" = "-k" ];
    then
        _DEL_OLD_DIR=
    else
        _DEL_OLD_DIR="$(pwd)"
    fi
    if [ -z "$_TMP_OLD_DIR" ];
    then
        echo "Not in temp mode." >&2
        return 1
    fi
    cd $_TMP_OLD_DIR
    export _TMP_OLD_DIR=
    unset _TMP_OLD_DIR
    if [ ! -z "$_DEL_OLD_DIR" ];
    then
        rm -Rf "$_DEL_OLD_DIR";
    fi
}

mktmp() {
    untmp >/dev/null 2>&1 || true
    tmp;
    NAME=$(mktemp -d __$1_XXXXXXXX)
    cd "$NAME"
    pwd;
}


alias cgrep="grep --color=auto"
bgtask() {
    $@ >/dev/null 2>&1 &
}

noecho() {
    $@ >/dev/null 2>&1
}

noerr() {
    $@ 2>/dev/null
}


aslist() {
# Converts the arguments to a space-separated list.

if [ $# -gt 0 ]; then
    echo $@;
else
    echo `readall`
fi
}


######
### die - Exit, with an optional exit code and
##         a message printed to stderr.
##
##    ARGS -
##          If > 1 arg, and first arg is a digit,
##            sets error code to first arg and
##            remainder becomes message.
##
##            If first arg is not a number, the
##              default ( 1 ) is used as exit code,
##              and first arg becomes part of message.
##
##          Remainder of args are the message to print
##
##       NOTE:  To use a number as your message,
##                (for some reason...), pass first
##                arg as empty string
die() {
    EXIT_CODE=1

    if [ $# -eq 0 ];
    then
        exit ${EXIT_CODE}
    fi

    # If more than one arg and first arg is digit, treat as exit code
    if [ "$#" -gt 1 ] && ( echo "${1}" | grep -qE "^[0-9][0-9]*$" );
    then
        EXIT_CODE="${1}"
        shift;
    fi

    # If first arg is empty string, they want to use a number
    #   as the exit message (why?). So strip it so we don't get
    #   a space.
    if [ -z "$1" ];
    then
        shift;
    fi

    MSG="$@"

    printf "\n%s\n\n" "${MSG}" >&2

    exit ${EXIT_CODE}
}


# vim: set ts=4 sw=4 expandtab
