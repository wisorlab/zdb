Read Neuroscore ZDB files in Matlab
==============

Neuroscore's ZDB files are just SQLite 3 databases with an '.zdb' file extension.

This repository provides Matlab wrappers around some basic functions of SQLite 3's C API.  This allows you to execute SQLite queries on an embedded database.


SQLite is a software library that implements a self-contained, serverless, zero-configuration, transactional SQL database engine. SQLite is the most widely deployed SQL database engine in the world. The source code for SQLite is in the public domain.


###install
 
On Windows, you will probably need MinGW32 or Cygwin to install. Use GNU Make (or mingw32-make).

On OSX use ```iff build``` to compile; the program iff can be found at [elseiff.com](http://elseiff.com).

###usage

The Matlab API exposes 2 mex functions ```zdb_get_score```, ```zdb_set_score```.


```matlab

% full path to a zdb file
file = '/path/to/neuroscore.zdb';

% retrieve all the scores in the file
scores = zdb_get_score( file );

% modify a score
scores(5) = 'W';

% update the ZDB with the modified data
zdb_set_score( file, scores );


```


