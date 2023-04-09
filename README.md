# Lambda Calculus Interpreter

A basic lambda calculus interpreter.

## Syntax

Rather than the standard lambda calculus syntax `λx.y x` the equivelant for lci is `\x y -> x`. This is for three main reasons: it's easier to parse, it's easier type and it's easier to read.

## Constants

All assignment operations are ran during in the parsing process. For example `TRUE = \x y -> x` will lexed and then parsed and this will be stored in a hash map. These tokens will then be removed. This "feature" was more of an oversight than anything and maybe fixed in the future.

## Examples

1. Logic (if true and true a else b)
```
TRUE = \x y -> x
FALSE = \x y -> y
AND = \x y -> x y (\x y -> y)
RESULT = AND TRUE TRUE

RESULT a b
```

```
TRUE = \x y -> x
FALSE = \x y -> y
AND = \x y -> x y (\x y -> y)
RESULT = AND TRUE TRUE

RESULT a b

((\x y -> x y (\x y -> y))) ((\x y -> x)) ((\x y -> x)) a b
(\x y -> x y (\x y -> y)) ((\x y -> x)) ((\x y -> x)) a b
(\x y -> x y (\x y -> y)) (\x y -> x) ((\x y -> x)) a b
(\x y -> x y (\x y -> y)) (\x y -> x) (\x y -> x) a b
(\y -> (\x y -> x) y (\x y -> y)) (\x y -> x) a b
(\x y -> x) (\x y -> x) (\x y -> y) a b
(\y -> (\x y -> x)) (\x y -> y) a b
(\x y -> x) a b
(\y -> a) b
a
```
