
  Database validation allows to run low-level checks of consistency of on-disk 
structures and even to fix some minor corruptions. It is recommended procedure
for any valuable database, i.e. DBA should validate database from time to time
to make sure it is healthy. But validation process requires exclusive access to 
database, i.e. it forbids any kind of concurrent access to database while 
validation runs. It could be a big problem to stop user access, especially when 
database is large and validation takes notable amount of time. 

  Online validation is a new feature which allows to perform some consistency
checks without exclusive access to database. Online validation allows to:
- validate some (or all) user tables in database
- validate some (or all) indices
- system tables are not validated
- other ODS checks (such as Header\PIP\TIP\Generators pages) are not run by
  online validation
- while table (and/or its index) is validated user attachments are allowed to 
  read this table. Attempt to INSERT\UPDATE\DELETE will wait until validation 
  finished or will return lock timeout error (depends on lock timeout of user 
  transaction)
- while table (and/or its index) is validated any kind of garbage collection at
  this table is disabled - background and cooperative garbage collection will 
  just skip this table, sweep will be terminated with error.

  When online validation starts to check table it makes few actions to prevent 
concurrent modifications of table's data:
- acquires relation lock in PR (protected read) mode
- acquires (new) garbage collection lock in PW (protected write) mode.
Both locks are acquired using user-specified lock timeout. If any lock request
fails error is reported and table is skipped.
Then table and its indices are validated in the same way as full validation does.
Then locks are released and next table is validated.

  Online validation is implemented as Firebird service and accessible via Services 
API. Therefore gfix utility can't run online validation. fbsvcmgr utility has
full support for new service, syntax is:

fbsvcmgr [host:]service_mgr [user <...>] [password <...>]
	action_validate dbname <filename>
	[val_tab_incl <pattern>]
	[val_tab_excl <pattern>]
	[val_idx_incl <pattern>]
	[val_idx_excl <pattern>]
	[val_lock_timeout <number>] 

where
	val_tab_incl		pattern for tables names to include in validation run
	val_tab_excl		pattern for tables names to exclude from validation run
	val_idx_incl    	pattern for indices names to include in validation run, 
						by default %, i.e. all indices
	val_idx_excl		pattern for indices names to exclude from validation run
	val_lock_timeout 	lock timeout, used to acquire locks for table to validate,
						in seconds, default is 10 sec
						 0 is no-wait
						-1 is infinite wait

  Patterns are regular expressions, they are processed by the same rules as 
"SIMILAR TO" expressions. All patterns are case-sensitive (despite of database 
dialect!). 
If pattern for tables is omitted then all user tables will be validated. 
If pattern for indices is omitted then all indices of tables to validate will 
be validated. 
System tables are not validated.

Examples:

1. fbsvcmgr.exe service_mgr user SYSDBA password masterkey 
		action_validate dbname c:\db.fdb
		val_tab_incl A% 
		val_idx_excl % 
		val_lock_timeout 0

this command will validate all tables in database "c:\db.fdb" with names 
starting with "A". Indices are not validated. Lock wait is not performed.

2. fbsvcmgr.exe service_mgr user SYSDBA password masterkey 
		action_validate dbname c:\db.fdb
		val_tab_incl "TAB1|TAB2"

this command will validate tables TAB1 and TAB2 and all their indices. 
Lock wait timeout is 10 sec.

Note, to specify list of tables/indices it is necessary to:
a) separate names by character "|"
b) don't use spaces : TAB1 | TAB2 is wrong
c) whole list should be enclosed in double quotes to not confuse command 
   interpreter


Vlad Khorsun, <hvlad at users sourceforge net>
