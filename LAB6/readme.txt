The goal of this homework is to modify code in main.c and Makefile to compare the 
time with single thread and multiple threads. 

After I downloaded the coded from online using wget command, I unzipped it
using tar xvf srt.tgz. I ran make clean check, I got the following 
rm -f *.o *.tmp 1-test.ppm 2-test.ppm 4-test.ppm 8-test.ppm srt srt.tgz
gcc -std=gnu11 -g -O2 -Wall -Wextra -Wno-unused-parameter   -c -o main.o main.c
gcc -std=gnu11 -g -O2 -Wall -Wextra -Wno-unused-parameter   -c -o raymath.o raymath.c
gcc -std=gnu11 -g -O2 -Wall -Wextra -Wno-unused-parameter   -c -o shaders.o shaders.c
gcc -std=gnu11 -g -O2 -Wall -Wextra -Wno-unused-parameter -o srt main.o raymath.o shaders.o -lm
time ./srt 1-test.ppm >1-test.ppm.tmp

real 0m41.042s
user 0m41.034s
sys  0m0.002s
mv 1-test.ppm.tmp 1-test.ppm
time ./srt 2-test.ppm >2-test.ppm.tmp
./srt: Multithreading is not supported yet.

real   0m0.002s
user   0m0.000s
sys    0m0.001s
make: *** [2-test.ppm] Error 1

I got an error on the 2nd test because the original one was only supporting 
for a single thread. I had to change it to support multithread in order to 
compare the running time with the single thread. 

By doing this, I realized that I had to change the nested loops that went through
the width and height of the image. Knowing that pthread_create took only one function
and allow only one argument to pass in. pthread_create function take 1 argument 
as a type of void* and return type of void*.

I had to use global variable because it is easy not to call any function
whenever I want to use it.

One more issue of this homework is to print the color values for each pixel, as
there could be race conditions in the printing. By solving this, I created 
3 dimensional array with the width, height, and color of the image 
[width][height][number of colors] in order to store each value for each thread 
in its indices. 

I also add -lpthread in my make file in order to make multithread work.

After that I ran make clean check again, I got the following:

time ./srt 1-test.ppm >1-test.ppm.tmp

real 0m41.121s
user 0m41.112s
sys  0m0.002s
mv 1-test.ppm.tmp 1-test.ppm
time ./srt 2-test.ppm >2-test.ppm.tmp

real 0m20.978s
user 0m41.716s
sys  0m0.001s
mv 2-test.ppm.tmp 2-test.ppm
time ./srt 4-test.ppm >4-test.ppm.tmp

real 0m11.147s
user 0m43.734s
sys  0m0.001s
mv 4-test.ppm.tmp 4-test.ppm
time ./srt 8-test.ppm >8-test.ppm.tmp

real 0m5.771s
user 0m44.674s
sys  0m0.005s
mv 8-test.ppm.tmp 8-test.ppm

As I can see, the more threads the better run (real) time. I can see that 8 parallel threads
is 8 time faster than a single thread. I think the reason is because one uses serial and
the other one uses parallel to do the task. 
