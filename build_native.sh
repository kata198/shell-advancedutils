# Builds for native system. 
export CFLAGS="-O3 -ffast-math -march=native -mtune=native -s"
exec make -j4
