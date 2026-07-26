## puck

`puck` is a tiny Lisp-style language.

The interpreter core and command-line program are written in C11. The test
suite uses C++ only because it is built with Catch2.

It has numbers, booleans, strings and lists, plus variables, functions,
conditionals and a small standard environment. It intentionally does not aim
to implement Scheme.

```lisp
(define answer 42)
(define (square x) (* x x))
(define (factorial n)
  (if (< n 2)
      1
      (* n (factorial (- n 1)))))

(print (square answer))
(print (factorial 6))
```

Core forms are `define`, `fn`, `if`, `begin` and `quote`. Built-in functions
include `+`, `-`, `*`, `/`, `=`, `<`, `>`, `not`, `list`, `cons`, `car`, `cdr`
and `print`.

## Build and run

```sh
cmake -S . -B build
cmake --build build
./build/puck
```

Pass a source file to run a program:

```sh
./build/puck program.puck
```

Run the test suite with:

```sh
ctest --test-dir build --output-on-failure
```
