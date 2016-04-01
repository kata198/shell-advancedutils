
if [ -z "$PREFIX" ]; then
    if [ -w "/usr/bin" ]; then
        PREFIX="/usr"
    elif [ -n "$HOME" ]; then
        PREFIX="$HOME"
    fi
fi
exec make install PREFIX=$PREFIX
