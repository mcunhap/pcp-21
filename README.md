#Repo for PCP-21 class


**Using clang for ThreadSanitizer instead of Valgrind**

```
clang -fsanitize=thread -g -O1 <program>.c

-g to get file names and line numbers in the warning
 messages
-01 to get reasonable performance

Output at `a.out` file
```

