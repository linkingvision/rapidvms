----------------
Cursor variables
----------------

  Function:
    Allow usage of explicit or implicit cursors without needing the use of INTO clause in FETCH and
    FOR SELECT.
    An explicit cursor automatically becomes a cursor variable.
    An implicit cursor (FOR SELECT) needs the {AS CURSOR <name>} clause to become a cursor variable.

  Author:
	Adriano dos Santos Fernandes <adrianosf@gmail.com>
    Dmitry Yemanov <dimitr@users.sf.net>

  Syntax rules:
    1) To unambiguously access a cursor variable, the cursor name should be prefixed by a colon.
    2) A cursor variable may be accessed without a colon prefix, but in this case, depending on the
       scope of contexts present in a query, the name may resolve to a query context intead of a
       cursor.
    3) It's allowed to use the colon prefix with trigger's NEW and OLD contexts.
    4) Cursor variables are read-only.
    5) A FOR SELECT without AS CURSOR needs the use of INTO, while with AS CURSOR it's not required,
       but still allowed. With FETCH, INTO is now optional.
    6) It's allowed now to use the colon prefix when assigning to variables or NEW's fields.
    7) Reading from a cursor variable returns the current field values. That means an UPDATE (with
       WHERE CURRENT OF) also updates the fields for subsequent reads. And DELETE (with WHERE
       CURRENT OF) makes subsequent reads to return NULL.

  Examples:
    1.
       execute block returns (o char(31))
       as
         declare c cursor for (
           select rdb$relation_name name
             from rdb$relations
         );
       begin
         open c;
         while (1 = 1) do
         begin
           fetch c;

           if (row_count = 0) then
               leave;

           o = :c.name;
           suspend;
         end

         close c;
       end

    2.
       execute block returns (o char(31))
       as
       begin
         for select rdb$relation_name name
               from rdb$relations
             as cursor c
         do
         begin
           o = :c.name;
           suspend;
         end
       end

    3.
       execute block returns (o1 char(31), o2 char(31))
       as
       begin
         for select rdb$relation_name
               from rdb$relations
               where rdb$relation_name = 'RDB$RELATIONS'
             as cursor c
         do
         begin
           for select :c.rdb$relation_name x1, -- prefixed, resolves to C cursor
                      c.rdb$relation_name x2   -- unprefixed, resolved to this query rdb$relations (c)
                 from rdb$relations c
                 where rdb$relation_name = 'RDB$DATABASE'
               as cursor d
           do
           begin
             o1 = :d.x1;
             o2 = :d.x2;
             suspend;
           end
         end
       end

