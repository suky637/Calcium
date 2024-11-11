# Calcium Programming Language

![image](https://github.com/suky637/Calcium/blob/main/res/calcium_logo.png?raw=true)

The Calcium Compiled Programming Language was build on C++, with an emphesis on the community. Our goal is to create a programming language that is community managed, starting with a basic compiler. Feel free to fork and make your own version of the calcium programming language. We use the Creative Commons Zero v1.0 Universal license, which grants you a lot of permissions, without conditions. If you are a designer, but you can't code, or you don't want to code, you can just go to discussions/ideas to give an idea, feel free to do it, we will most likely implement it!

Here is some basic program in Calcium:
Example 1: Hello World program
```
import "stdio.h"

fn main() -> int
{
    printf("Hello, World!\n")
    return = 0
}
```

Example 2: two Sum program
```
import "stdio.h"
import "string.ca"

fn twoSum(point int nums?, int size?, int target?) -> point int
{
    point int ret = malloc(sizeof(int) * 2)
    int i = 0
    while i < size
    {
        int j = 0
        while j < size
        {
            if i != j and nums[i] + nums[j] == target
            {
                ret[0] = i
                ret[1] = j
                return = ret
            }
            j = j + 1
        }
        i = i + 1
    }
    ret[0] = -1
    ret[1] = 0
    return = ret
}

fn main() -> int
{
    point int list = malloc(sizeof(int) * 4)
    list[0] = 3
    list[1] = 4
    list[2] = 5
    list[3] = 6
    point int res = twoSum(list, 4, 10)
    if res[0] != -1
    {
        int res0 = res[0]
        int res1 = res[1]
        printf("Number #1: %d\nNumber #2: %d\n", res0, res1)
    }
    return = 0
}
```