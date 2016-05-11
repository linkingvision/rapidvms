		New way to authenticate users in firebird.

Firebird starting with version 2.1 can use Windows security for user authentication.
Current security context is passed to the server and if it's OK for that server is used to determine
firebird user name. To use Windows trusted authentication in FB3 you should make minimum changes in
firebird.conf and tune mappings in your databases.

Parameter Authentication in firebird.conf file is not used any more - it's replaced with more
generic AuthServer (and AuthClient) parameters. Also to use trusted authentication one should turn
off mandatory wire encryption because Win_Sspi plugin (which implements trusted authentication on
Windows) does not provide an encryption key. So minimum changes in firebird.conf you need is:

AuthServer = Srp, Win_Sspi
WireCrypt = Enabled

Also mapping (see sql.extensions/README.mapping.html) should be created. To tune for all databases
do:

create global mapping trusted_auth using plugin win_sspi from any user to user;

Do not put user and password parameters in DPB/SPB. With provided firebird.conf in almost all cases
trusted authentication will be used (see environment below for exceptions). Suppose you have logged
to the Windows server SRV as user John. If you connect to server SRV with isql, not specifying
Firebird login and password:

isql srv:employee

and do:

SELECT CURRENT_USER FROM RDB$DATABASE;

you will get something like:

USER
====================================================
SRV\John

Windows users may be granted rights to access database objects and roles in the same way as
traditional Firebird users. (This is not something new - in UNIX OS users might be granted rights
virtually always).

- If domain administrator (member of well known predefined groups) connects to Firebird using trusted
authentication, he/she may be granted 'god-like' (SYSDBA) rights depending upon settings in database,
to which such user attachs. To keep CURRENT_USER value in a form DOMAIN\User, a new object (predefined
system role) is added to the database. The name of that role is RDB$ADMIN, and any user, granted it,
can attach to the database with SYSDBA rights. To configure all databases to auto-grant that role to
administrators, use the following command:

create global mapping win_admin using plugin win_sspi from predefined_group DOMAIN_ANY_RID_ADMINS to role RDB$ADMIN;

Take into an account, that if Windows administrator attaches with role set in dpb, it will not be
replaced with RDB$ADMIN, i.e. he/she will not get SYSDBA rights.

- To keep legacy behavior when ISC_USER/ISC_PASSWORD variables are set in environment, they
are picked and used instead of trusted authentication. In case when trusted authentication is needed
and ISC_USER/ISC_PASSWORD are set, add new DPB parameter isc_dpb_trusted_auth to DPB. In most
of Firebird command line utilities switch -trusted (may be abbreviated up to utility rules) is used
for it.

isql srv:db                -- log using trusted authentication
set ISC_USER=user1
set ISC_PASSWORD=12345
isql srv:db                -- log as 'user1' from environment
isql -trust srv:db         -- log using trusted authentication

				Author: Alex Peshkov, <peshkoff at mail.ru>
