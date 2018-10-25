## boolean-solver
a program that, given a boolean expression, will evaluate and generate a truth table for that expression.
### supported operations
 - not (`~`)
 - and (`*`)
 - or (`+`)
 - xor (`^`)

brackets can also be used to ensure that the expression inside the brackets will be evaluated before others.

#### example input
`(~a * b) + (a * ~b)` (for reference this is the equivalent of `a xor b` (or `a ^ b` in this notation)).

#### example output
```
| a b |   |
| ---------
| 0 0 | 0 |
| 1 0 | 1 |
| 0 1 | 1 |
| 1 1 | 0 |
```

### todo
- [ ] break down truth table evaluation into sub caclulations
- [ ] simplify expressions & output simplified expression before truth table generation
  - [ ] literal evaluation & simplification - e.g. `p + 0 = p`, `p + 1 = 1`, `p * 0 = 0`, `p * 1 = p`, `~1 = 0` etc...
- [ ] implication & double implication operations (will probably require multi character operators/tokens)
- [ ] expression equality evaluation (e.g. syntax `~(a * b) = ~a + ~b`) via truth table comparison.
- [ ] better formatting/system for truth tables
- [ ] web version!
- [ ] general code clean up
- [ ] multi character 'variable' (input) names
- [ ] ability to use `t/f/T/F/true/false/True/False` as well as `1/0` for true and false values.
- [ ] logic gate diagram generations
- [ ] MathML style image equation generated from input expression
- [ ] generate boolean expression from truth table (via minterms/product of sums of whatever)
- [ ] _cool idea_ natural language parsing - natural language text to boolean expression - e.g. `"it is cold and raining" = p * q where p = "it is raining" and q = "it is cold"`
