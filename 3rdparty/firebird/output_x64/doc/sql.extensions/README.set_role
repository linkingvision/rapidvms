SQL Language Extension: SET ROLE

   Implements capability to change role in already establishment attachment.


Author:
   Alex Peshkoff <peshkoff@mail.ru>


Syntax is:

   SET ROLE rolename;
   SET TRUSTED ROLE;

Description:

Makes it possible to set current role to one, granted to current user, or to one assigned
to database attachment as trusted.
By default trusted role is not assigned to the attachment. This behavior can be changed using
appropriate authentication plugin and CREATE/ALTER MAPPING command. Well know sample of
trusted role is RDB$ADMIN assigned to windows admins when windows trusted authentication is
used.

Samples:

   SET ROLE manager;
   select current_role from rdb$database;

  Displays:

ROLE
=============================== 
MANAGER
