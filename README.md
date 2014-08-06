SQLite 3 for Matlab
==============

This repository provides Matlab wrappers around the [exec](http://www.sqlite.org/c3ref/exec.html) function of SQLite 3's C API.  This allows you to execute SQLite queries on an embedded database.


SQLite is a software library that implements a self-contained, serverless, zero-configuration, transactional SQL database engine. SQLite is the most widely deployed SQL database engine in the world. The source code for SQLite is in the public domain.


###install
 

The API is written in C and will need to be compiled for your platform. The program [iff](elseiff.com) makes this easy. With iff installed, run ```iff build``` in the root of the repository.


###usage

The Matlab API exposes 3 mex functions ```sqlite3_open```, ```sqlite3_exec``` and ```sqlite3_close```.

The function ```sqlite3_open``` takes the path to an SQLite3 database as a parameter and returns a database handle. Use ```sqlite3_close``` to close the database once you've finished using it.

```matlab
% path to the database file
path = '~/Desktop/sql3file_file.db';

% open the database
db = sqlite3_open(path);

% close the database
sqlite3_close(db);

```

To access data, use the function 'sqlite3_exec', which takes the following syntax: 

```matlab
cellArray = sqlite3_exec( db, sqlQuery );
```

### examples
 

To query basic information about a database, you could use a query such as:

```matlab

sql = 'SELECT name, sql FROM sqlite_master WHERE type="table"';
result = sqlite3_exec( db, sql );

```

The variable 'result' will be a cell array of structs.  Each key-value pair in of these struct corresponds to a column name and value returned from the SQL table:

```
result{1}

{
  'name' : 'table_name',
  'sql' : 'CREATE TABLE ...'
}

```
Now that the table names are known, you could then issue the following query to see the contents of the table:

```

sql2 = 'PRAGMA table_info(table_name)';
result2 = sqlite3_exec( db, sql2 );

```

