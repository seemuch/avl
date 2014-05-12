\begin{verbatim}
primary_expression
	: IDENTIFIER
	| CONSTANT
	| TRUE
	| FALSE
	| CHAR_LITERAL
	| STRING_LITERAL
	| '(' conditional_expression ')'
	;

postfix_expression
	: primary_expression
	| IDENTIFIER '[' conditional_expression ']'
	| IDENTIFIER '[' conditional_expression ':' conditional_expression ']'
	| IDENTIFIER '(' ')'
	| IDENTIFIER '(' argument_expression_list ')'
	| postfix_expression INC_OP
	| postfix_expression DEC_OP
	;

argument_expression_list
	: conditional_expression
	| argument_expression_list ',' conditional_expression
	;

unary_expression
	: postfix_expression
	| INC_OP unary_expression
	| DEC_OP unary_expression
	| unary_operator unary_expression
	| LEN '(' unary_expression ')'
	;

unary_operator
	: '+'
	| '-'
	| '!'
	;

multiplicative_expression
	: unary_expression
	| multiplicative_expression '*' unary_expression
	| multiplicative_expression '/' unary_expression
	| multiplicative_expression '%' unary_expression
	;

additive_expression
	: multiplicative_expression
	| additive_expression '+' multiplicative_expression
	| additive_expression '-' multiplicative_expression
	;

relational_expression
	: additive_expression
	| relational_expression '<' additive_expression
	| relational_expression '>' additive_expression
	| relational_expression LE_OP additive_expression
	| relational_expression GE_OP additive_expression
	;

equality_expression
	: relational_expression
	| equality_expression EQ_OP relational_expression
	| equality_expression NE_OP relational_expression
	;

logical_and_expression
	: equality_expression
	| logical_and_expression AND_OP equality_expression
	;

logical_or_expression
	: logical_and_expression
	| logical_or_expression OR_OP logical_and_expression
	;

conditional_expression
	: logical_or_expression
	| logical_or_expression '?' conditional_expression ':' conditional_expression
	;

assignment_expression
	: postfix_expression '=' conditional_expression
	;

type_specifier
	: VOID
	| CHAR
	| INT
	| STRING
	| INDEX
	| BOOL
	;

expression
	: conditional_expression
	| assignment_expression
	| DISPLAY IDENTIFIER
	| HIDE IDENTIFIER
	| SWAP '(' IDENTIFIER ',' conditional_expression ',' conditional_expression ')'
	| PRINT print_list
	;

print_list
	: conditional_expression
	| print_list ',' conditional_expression
	;

declaration
	: type_specifier init_declarator
	| DISPLAY type_specifier init_declarator
	| HIDE type_specifier init_declarator
	;

init_declarator
	: declarator
	| declarator '=' initializer
	;

declarator
	: IDENTIFIER
	| IDENTIFIER '[' conditional_expression ']'
	| IDENTIFIER '[' ']'
	;

initializer
	: conditional_expression
	| '{' initializer_list '}'
	;

initializer_list
	: conditional_expression
	| initializer_list ',' conditional_expression
	;

statement
	: compound_statement
	| expression_statement
	| declaration_statement
	| display_statement
	| selection_statement
	| iteration_statement
	| jump_statement
	| ';'
	;

expression_statement
	: expression ';'
	;

declaration_statement
	: declaration ';'
	;

compound_statement 
	: '{' '}'
	| '{' statement_list '}'
	;

statement_list
	: statement
	| statement_list statement
	;

display_statement
	: BEGIN_DISPLAY statement_list END_DISPLAY
	;

selection_statement
	: IF '(' conditional_expression ')' statement
	| IF '(' conditional_expression ')' statement ELSE statement
	;

iteration_statement
	: WHILE '(' conditional_expression ')' statement
	| DO statement WHILE '(' conditional_expression ')' ';'
	| FOR '(' expression ';' conditional_expression ';' ')'	statement
	| FOR '(' expression ';' conditional_expression ';' expression ')' statement
	| FOR '(' declaration ';' conditional_expression ';' ')' statement
	| FOR '(' declaration ';' conditional_expression ';' expression ')' statement
	;

jump_statement
	: CONTINUE ';'
	| BREAK ';'
	| RETURN ';'
	| RETURN conditional_expression ';'
	;

translation_unit
	: function_definition
	| translation_unit function_definition
	;

function_definition
	: type_specifier  IDENTIFIER '(' parameter_list ')' compound_statement
	| type_specifier  IDENTIFIER '('  ')' compound_statement
	;

parameter_list
	: parameter_declaration
	| parameter_list ',' parameter_declaration
	;

parameter_declaration
	: type_specifier declarator
	;

program
	: translation_unit
	;
\end{verbatim}
