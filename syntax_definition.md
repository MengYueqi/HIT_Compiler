# 文法定义

### Tokens:

```
INT → /* A sequence of digits without spaces */
FLOAT → /* A real number consisting of digits and one decimal point. The decimal point must be surrounded by at least one digit */
ID → /* A character string consisting of 52 upper- or lower-case alphabetic, 10 numeric and one underscore characters. Besides, an identifier must not start with a digit */
SEMI → ;
COMMA → ,
ASSIGNOP → =
RELOP → > | < | >= | <= | == | !=
PLUS → +
MINUS → -
STAR → *
DIV → /
AND → &&
OR → ||
DOT → .
NOT → !
TYPE → int | float
LP → (
RP → )
LB → [
RB → ]
LC → {
RC → }
STRUCT → struct
RETURN → return
IF → if
ELSE → else
WHILE → while
```

### High-level Definitions:

```
Program → ExtDefList
ExtDefList → ExtDef ExtDefList
    | e
ExtDef → Specifier ExtDecList SEMI
    | Specifier SEMI
    | Specifier FunDec CompSt
ExtDecList → VarDec
    | VarDec COMMA ExtDecList
```

### Specifiers:

```
Specifier → TYPE
    | StructSpecifier
StructSpecifier → STRUCT OptTag LC DefList RC
    | STRUCT Tag
OptTag → ID
    | e
Tag → ID
```

### Declarators:

```
VarDec → ID
    | VarDec LB INT RB
FunDec → ID LP VarList RP
    | ID LP RP
VarList → ParamDec COMMA VarList
    | ParamDec
ParamDec → Specifier VarDec
```

### Statements:

```
CompSt → LC DefList StmtList RC
StmtList → Stmt StmtList
    | e
Stmt → Exp SEMI
    | CompSt
    | RETURN Exp SEMI
    | IF LP Exp RP Stmt
    | IF LP Exp RP Stmt ELSE Stmt
    | WHILE LP Exp RP Stmt
```

### Local Definitions:

```
DefList → Def DefList
    | e
Def → Specifier DecList SEMI
DecList → Dec
    | Dec COMMA DecList
Dec → VarDec
    | VarDec ASSIGNOP Exp
```

### Expressions:

```
Exp → Exp ASSIGNOP Exp
    | Exp AND Exp
    | Exp OR Exp
    | Exp RELOP Exp
    | Exp PLUS Exp
    | Exp MINUS Exp
    | Exp STAR Exp
    | Exp DIV Exp
    | LP Exp RP
    | MINUS Exp
    | NOT Exp
    | ID LP Args RP
    | ID LP RP
    | Exp LB Exp RB
    | Exp DOT ID
    | ID
    | INT
    | FLOAT
Args → Exp COMMA Args
    | Exp
```