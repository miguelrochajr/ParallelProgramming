Question 2
----------
Modify the trapezoidal rule so that it will correctly estimate the integral even if *comm_sz* doesn’t evenly divide n.
(You can still assume that n >= comm_sz.)

Answer
---
To anser this question, we first have to investigate what occurs with local_n when we have an odd number of processors. Let's see for *comm_sz*=3 using the hardwired
values already setted to a, b and n.

```cpp
a = 0.0;
b = 3.0;
n = 1024;

local_n = 1024/3 = (int)(341.33333333) = 341
```

So, using this implementation, since local_n is an integer, each of the processes would have 341 trapezoids to compute. But if you do the math, 3x341 is 1023. We are
missing one trapezoid here. The solution is simple. The last odd ranked process will receive one more trapezoid to compute. Note that this process has an odd rank and equal
to comm_sz-1. So, right after the computation of local_n, on line 27, we have:

```cpp
...

local_n = n/comm_sz;  /* So is the number of trapezoids  */
if(my_rank%2==1 && my_rank==comm_sz-1){
  local_n+=1;
}

...

```
