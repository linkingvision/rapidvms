SQL Language Extension: GRANT/REVOKE permissions on DDL operations

   Implements capability to manage permissions on DDL operations.

Author:
   Red Soft Corporation, roman.simakov(at)red-soft.biz

Syntax is:

GRANT CREATE <OBJECT> TO [USER | ROLE] <user/role name> [WITH GRANT OPTION];
GRANT ALTER ANY <OBJECT> TO [USER | ROLE] <user/role name> [WITH GRANT OPTION];
GRANT DROP ANY <OBJECT> TO [USER | ROLE] <user/role name> [WITH GRANT OPTION];

REVOKE [GRANT OPTION FOR] CREATE <OBJECT> FROM [USER | ROLE] <user/role name>;
REVOKE [GRANT OPTION FOR] ALTER ANY <OBJECT> FROM [USER | ROLE] <user/role name>;
REVOKE [GRANT OPTION FOR] DROP ANY <OBJECT> FROM [USER | ROLE] <user/role name>;

Where <OBJECT> could be:
TABLE, VIEW, PROCEDURE, FUNCTION, PACKAGE, GENERATOR, SEQUENCE, DOMAIN,
EXCEPTION, ROLE, CHARACTER SET, COLLATION, FILTER

Description:

Makes it possible to grant and revoke privileges on DDL operations.

DDL operations for managing triggers and indices re-use table privileges.

If ANY keyword is used a user will be able to perform operation on any object. Otherwise only on object which he owns.
If ANY keyword was used due GRANT operation it also must be used in according REVOKE operation.

Sample:

GRANT CREATE TABLE TO Joe;
GRANT ALTER ANY TABLE TO Joe;
REVOKE CREATE TABLE FROM Joe;

For database access special form is supported:

GRANT CREATE DATABASE TO [USER | ROLE] <user/role name>;
GRANT ALTER DATABASE TO [USER | ROLE] <user/role name> [WITH GRANT OPTION];
GRANT DROP DATABASE TO [USER | ROLE] <user/role name> [WITH GRANT OPTION];

REVOKE CREATE DATABASE FROM [USER | ROLE] <user/role name>;
REVOKE [GRANT OPTION FOR] ALTER DATABASE FROM [USER | ROLE] <user/role name>;
REVOKE [GRANT OPTION FOR] DROP DATABASE FROM [USER | ROLE] <user/role name>;
