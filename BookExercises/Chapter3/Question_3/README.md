Question 3
----------
Determine which of the variables in the trapezoidal rule program are local and which are global.

Answer
---
The global variables are those which its value is significant to all processes. Local variables are those which their values are significant only to the process that is using them. So, of the trapezoidal rule program (file Question3.c), they are:

* Global
  * comm_sz
  * n
  * a, b and h

* Local
  * my_rank
  * local_n
  * local_int
  * local_a, local_b
  * total_int
  * source
