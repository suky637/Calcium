# Calcium Programming Language

![image](https://github.com/suky637/Calcium/blob/main/res/calcium_logo.png?raw=true)

The Calcium compiled programming language is a compiler that compiles directly to C. Please note that to get any of the most recent features, it is required to compile it yourself (ex. creating an array with a variable as its length)

Why isn't some features in the release?

Before Releasing, I'd rather accumulate many features to make the update be somewhat bigger. That's why some features are locked behind a build wall.

Here is some basic programs in Calcium:
Example 1: Hello World program
```
fn main() -> int {
    std.println("Hello, World!")
    return = 0
}
```

Example 2: two Sum program
```
fn twoSum(point int nums?, int amt?, int target?) -> point int {
    2 static int ret?
    int i = 0
    while i < amt {
        int j = 0
        while j < amt {
            if i != j and nums[i] + nums[j] == target {
                ret[0] = i
                ret[1] = j
                return = ret
            }
            j = j + 1
        }
        i = i + 1
    }
    ret[0] = -1
    return = ret
}

fn main() -> int {
    4 int input = {5, 2, 3, 4}
    point int output = twoSum(input, 4, 6)
    if output[0] != -1 {
        std.print("Index 1: ")
        std.printIln(output[0])
        std.print("Index 2: )
        std.printIln(output[1])
    }
    return = 0
}
```

Weird quirks

```
fn main() -> int {
    int x = 10
    if x el [0, 100] {
        std.println("X is between 0 and 100.")
    }
    x! int some_array?

    if x el ]-inf, +inf[ {
        std.println("ofc it is part of that.")
    }

    if x el [1, 5]u[9, 15] {
        std.println("unions works !")
    }

    return = 0
}
```