# Thread-Safe-CRUD

Implement CRUD operations over data structure which are not only thread safe, but also have maximum concurrency as possible.

There are some desciplies to follow for the subject:

1. Threads performing READ operations must not compete/block each other to get access.

2. If thread T1 performs READ operation, and thread T2 performs WRITE operation, they must not block each other unless they perform operation on same object.

3. If thread T1 performs WRITE operation, and thread T2 also performs WRITE operation, they must not block each other unless they perform operation on same object.

4. If threaad T1 is performing READ/WRITE operaiton on an object, some other thread in the system must not FREE the object memory prematurely.

5. If thread T1 is blocked on lock X, some other thread in the system must not FREE the lock X memory.
