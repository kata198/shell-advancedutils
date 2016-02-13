# Builds for native system. 
export CFLAGS="-O3 -ffast-math -march=native -mtune=native"
exec make -j4
