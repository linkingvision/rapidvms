---------------------
Statistical Functions
---------------------

By the SQL specification, some statistical functions are defined.
Function about variance and standard deviation are bellow.

VAR_SAMP: return  the sample variance.
  eq. (SUM(<expr> ^ 2) - SUM(<expr>) ^ 2 / COUNT(<expr>)) / (COUNT(<expr>) - 1)

VAR_POP: return the population variance.
  eq. (SUM(<expr> ^ 2) - SUM(<expr>) ^ 2 / COUNT(<expr>)) / COUNT(<expr>)

STDDEV_SAMP: return the sample standard deviation.
  eq. SQRT(VAR_SAMP(<expr))

STDDEV_POP: return the population standard deviation.
  eq. SQRT(VAR_POP(<expr>))

COVAR_SAMP: return the sample population.
  eq. (SUM(<expr1> * <expr2>) - SUM(<expr1>) * SUM(<expr2>) / COUNT(*)) / (COUNT(*) - 1)

COVAR_POP: return the population covariance.
  eq. (SUM(<expr1> * <expr2>) - SUM(<expr1>) * SUM(<expr2>) / COUNT(*)) / COUNT(*)

CORR: returns the coefficient of correlation.
  eq. COVAR_POP(<expr1>, <expr2>) / (STDDEV_POP(<expr2>) * STDDEV_POP(<expr1>))

Author:
    Hajime Nakagami <nakagami@gmail.com>

Syntax:
    <single param statistical function> ::= <single param statistical function name>(<expr>)
    <single param statistical function name> := { VAR_POP | VAR_SAMP | STDDEV_POP | STDDEV_SAMP }

    <dual param statistical function> ::= <dual param statistical function name>(<expr1>, <expr2>)
    <dual param statistical function name> := { COVAR_POP | COVAR_SAMP | CORR }

Note:
    If VAR_SAMP, STDDEV_SAMP, COVAR_SAMP and result count is 0 or 1, return NULL.
    If VAR_POP, STDDEV_POP, COVAR_POP, CORR and result count is 0, return NULL.

Example:
    SELECT STDDEV_SAMP(salary) FROM employees;
