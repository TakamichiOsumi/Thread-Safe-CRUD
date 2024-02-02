# Thread-Safe-CRUD

Implement CRUD operations over data structure which are not only thread safe, but also have better concurrency.

There are some desciplies to follow for the subject:

1. Threads performing READ operations must not compete/block each other to get access.

2. If thread T1 performs READ operation, and thread T2 performs WRITE operation, they must not block each other unless they perform operation on same object.

3. If thread T1 performs WRITE operation, and thread T2 also performs WRITE operation, they must not block each other unless they perform operation on same object.

4. If threaad T1 is performing READ/WRITE operaiton on an object, some other thread in the system must not FREE the object memory prematurely.

5. If thread T1 is blocked on lock X, some other thread in the system must not FREE the lock X memory.

Notes about internals:

The program utilizes two levels of locks - *container level lock* and *container-element level lock*.

The former, the lock which resides in the data structure layer, protects change in the data structure in multi-threaded environment. As to CRUD operation, this level of lock takes READ lock on READ and also UPDATE operation, while it takes WRITE lock on CREATE and DELETE lock, since both of them change the entire data structure.

The latter, the lock attached to the application data layer, protects change of the application object in multi-threaded environment. With regard to CRUD operation, this level of lock takes NO locks on CREATE and WRITE operation for the data structure. Both types of operations effect the entire data structure but can be regarded as independent from application data manipulation or change. For READ operation, this level locks are conducted with READ lock, while UPDATE operations require WRITE lock on the level lock.
