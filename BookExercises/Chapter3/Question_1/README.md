Question 1
----------

What happens in the greetins program if, instead of strlen(greeetin)+1, we use strlen(greeting) for the length of the message being sent by processes 1,2,...,comm_sz-1?
What happens if we use MAX_STRING instead of strlen(greeting)+1? Can you explain these results?

Answer
---
The strlen(greeting)+1 is the number of characters in the message plus one characther for the '\0' char. Which, in C, represents the end of a string. If we use only
strlen(greeting) we would be sending the String message without the '\0' charachter. This could lead to plenty of problems. The most dangerous of them is segmentation fault.
Since we are seding the string "without an end", an implementation that keeps parsing the string up to an '\0' char would never stop and probably attempt to access a memory
location that does not exist.

In the case we use MAX_STRING, we have some small differences. Despite our output being the same, the MPI_Send will send all the 100 locations to the receiver. In the case
of our greeting string, all of it will be sent and not only the actual text. Since we are also sending the \0, the output will be the same since printf looks for it to end
the string output.
