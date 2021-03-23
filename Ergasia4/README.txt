Interpreter assumes that all variables cannot be consisted of more than 100 characters. 

Interpreter assumes that each line cannot be consisted of more than 1000 characters. (spaces included )

If someone tries to allocate memory for an already existing array, the interpreter will terminate the program and display an error message.

All different words (including operators ) must be seperated by a spacebar .

Negative and decimal constants may be used. 

The negative sign should not be seperated from the number by spacebar. For example the statement x = -3 is valid when x = - 3 is not valid. 

Decimal points are used to recognize decimal numbers .

You cannot assign a decimal value to an integral variable.

Complex arithmetic operations are implemented ( including parenthesis ).

Condition statements also support parenthesis,  "||" for logical or, "&&" for logical and.

For loop is also implemented. 

The syntax of for loop is : for <var> from <var-con1> to <var-con2> step <var-con3>

This is equivalent to:
<var> = <var-con1> 
while var >=< <var-con3>	( >= or <= depends on step, if positve then the operand is >= else <= )
	...
	<var> = <var> + <var-con3>
...

Write and writeln can also print strings and complex arithmetic operations.

The syntax is: 
write <expression-string1> <expression-string2> .... <expression-stringK>

Whatever is inside " and " is printed as a string.

Escape sequences are implemented.

Expression may be just one variable or a complex expression. 

The different terms to be printed are seperated by a spacebar.

The random number generator also supports a seed using the command: 

seed <var-epxression>

The random number generator is seeded with the expression or variable after the command "seed"

There is a Makefile resposible for the compilation. Give the command make at the command line. 