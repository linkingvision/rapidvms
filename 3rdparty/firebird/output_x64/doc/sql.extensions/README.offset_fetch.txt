-----------------------
OFFSET and FETCH clause
-----------------------

  Function:
    SQL:2008 compliant equivalent for FIRST/SKIP, and an alternative for ROWS.
    The OFFSET clause specifies the number of rows to skip. The FETCH clause 
    specifies the number of rows to fetch.
    
    OFFSET and FETCH can be applied independently on top-level and nested query
    expressions.
    
  Author:
    Mark Rotteveel <mrotteveel@users.sourceforge.net>
    
  Syntax rules:
    SELECT ... [ ORDER BY <expr_list> ] 
      [ OFFSET <simple_value_expr> { ROW | ROWS } ]
      [ FETCH { FIRST | NEXT } [ <simple_value_expr> ] { ROW | ROWS } ONLY ]
    
    Where <simple_value_expr> is a (numeric) literal, a SQL parameter (?) or 
    PSQL named parameter (:namedparameter).
  
  Scope:
    DSQL, PSQL
  
  Example(s):
    1. SELECT * FROM T1
       ORDER BY COL1
       OFFSET 10 ROWS
    2. SELECT * FROM T1
       ORDER BY COL1
       FETCH FIRST 10 ROWS ONLY
    3. SELECT * FROM (
          SELECT * FROM T1
          ORDER BY COL1 DESC
          OFFSET 1 ROW
          FETCH NEXT 10 ROWS ONLY
       ) a
       ORDER BY a.COL1
       FETCH FIRST ROW ONLY
  
  Note(s):
    1. Firebird doesn't support the percentage FETCH defined in the SQL 
       standard.
    2. Firebird doesn't support the FETCH ... WITH TIES defined in the SQL
       standard.
    3. The FIRST/SKIP and ROWS clause are non-standard alternatives.
    4. The OFFSET and/or FETCH clauses cannot be combined with ROWS or 
       FIRST/SKIP on the same query expression.
    5. Expressions, column references, etc are not allowed within either clause.
    6. Contrary to the ROWS clause, OFFSET and FETCH are only available on 
       SELECT statements.