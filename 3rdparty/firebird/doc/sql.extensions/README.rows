-----------
ROWS clause
-----------

  Function:
    Limits the number of rows retrieved from a select expression. For the 
    top-level select statement, it would mean a number of rows sent to the host
    program.

  Author:
    Dmitry Yemanov <yemanov@yandex.ru>

  Syntax rules:
    SELECT ... [ORDER BY <expr_list>] ROWS <expr1> [TO <expr2>]

  Scope:
    DSQL, PSQL

  Example(s):
    1. SELECT * FROM T1
       UNION ALL
       SELECT * FROM T2
       ORDER BY COL
       ROWS 10 TO 100
    2. SELECT COL1, COL2, ( SELECT COL3 FROM T3 ORDER BY COL4 DESC ROWS 1 )
       FROM T4
    3. DELETE FROM T5
       ORDER BY COL5
       ROWS 1

  Note(s):
    1. ROWS is a more understandable alternative to the FIRST/SKIP clauses with
       some extra benefits. It can be used in unions and all kind of subqueries. 
       It is also available in the UPDATE/DELETE statements.
    2. When <expr2> is omitted, then ROWS <expr1> is a semantical equivalent for
       FIRST <expr1>. When both <expr1> and <expr2> are used, then ROWS <expr1>
       TO <expr2> means:
       FIRST (<expr2> - <expr1> + 1) SKIP (<expr1> - 1). Note that there's no 
       semantic equivalent for a SKIP clause used without a FIRST clause.
    3. The ROWS-clause is not defined in the SQL standard. For SELECT, consider 
       the alternative OFFSET and FETCH clauses defined in the SQL standard.
