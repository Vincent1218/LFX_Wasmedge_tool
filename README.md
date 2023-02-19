# LFX Mentorship 2023 Challenge

### Applicant: Chai Wen Xuan

This is my solution to the pretest challenge for LFX mentorship for #2226 and #2241.
It is a standalone tool that uses WasmEdge C SDK to execute a given Wasm application.

<br/><br/>

# Requirements

The tool should support the following commands:

```
tool [version] [run] [wasm path] [arguments]
`version` is optional and prints the used wasmedge version; if this is given, you can ignore any other options.
`run` is optional.
`wasm path` is required; the relative and absolute path should be supported.
`arguments` is optional; some of the hidden wasm applications may not have arguments.
```

# Functionality accomplished

1. Argument passing

- required 'wasm path' & optional 'version','run', 'arguments'

2. WasmEdge_Value type

- supports I32 & F32, hence we are able to execute wasm function like float addition, division

3. Multiple function

- supports wasm file with multiple functions, for example, add(),factorial(),fibonacci() in one wasm file


<br/><br/>

# Installation

1. Clone this repo

```
$ git clone https://github.com/Vincent1218/LFX_Wasmedge_tool.git
```

2. Install WasmEdge [Click Me](https://wasmedge.org/book/en/quick_start/install.html)

3. Building the tools

```
$ mkdir build && cd build
$ cmake ../
$ make
```

## Usage

1. No arguments

- Error will be printed out as wasm path is required.

```
$ ./x
```

![1](/static/1.png)

2. Version

- If version is given, we will ignore the arguments passed after it.
- In this example, we can see that the argument number is also calculated when parsing.

```
# only "version"
$ ./x version

# extra arguments
$ ./x version run ../wasm_app/add.wasm 23 522
```

![2](/static/2.png)
![8](/static/8.png)

3. With "run"

- Relative path and absolute path is handled in this program.

```
# relative path
$ ./x run ../wasm_app/add.wasm 23 522

# absolute path
$ ./x run /Users/pikacent/Project/LFX_Wasmedge_tool/wasm_app/float_add.wasm 12.44 63.15
```

![3](/static/3.png)
![4](/static/4.png)

4. Without "run"

- "run" is optional, without "run" the .wasm file executes normally

```
$ ./x ../wasm_app/factorial.wasm 10
```

![5](/static/5.png)

5. Arguments number passed wrongly

- The program will compare the sum of params in each function and the number of application argument
- If it is not equal, error will be printed out

```
$ ./x ../wasm_app/fibonacci.wasm 23 12
```

![6](/static/6.png)

6. Multiple function

- add_fac_fibo.wasm includes 3 functions, add(), factorial() and fibonacci()
- To execute this .wasm file, we need to pass in total of 4 arguments, 1st&2nd for add(), 3rd for factorial() and 4th for fibonacci().

```
$ ./x ../wasm_app/add_fac_fibo.wasm 2 5 3 10
```

![7](/static/7.png)

