Suppose comm sz = 4 and suppose that x is a vector with n = 14 components.
----

With these assumptions, we have n/comm_sz values for each process. In this case, 14/4 = 3.5 values for each.

The most important thing we must keep in mind is the load balance. In order to attempt the best balance, the last n%comm_sz processes will
receive (int)(n/comm_sz) values from the vector x.
So, in this case, we have the following result.

n%comm_sz = 14%3 = 2. The last two will receive (int)(14/3) = (int)(3.5) = 3 values.
The rest will receive ceil(n/comm_sz) = ceil(3.5) = 4.

Thus, using this method we have the following answers.

## A)  How would the components of x be distributed among the processes in a program that used a block distribution?

Process 0: x0,  x1,  x2, x3
Process 1: x4,  x5,  x6, x7
Process 2: x8,  x9,  x10
Process 3: x11, x12, x13

## B) How would the components of x be distributed among the processes in a program that used a cyclic distribution?

Process 0: x0, x4, x8, x12
Process 1: x1, x5, x9, x13
Process 2: x2, x6, x10
Process 3: x3, x7, x11

## C) How would the components of x be distributed among the processes in a program that used a block-cyclic distribution with blocksize b = 2?

Process 0: x0, x1, x8,  x9
Process 1: x2, x3, x10, x11,
Process 2: x4, x5, x12,
Process 3: x6, x7, x13
