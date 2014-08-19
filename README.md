Read Neuroscore ZDB files in Matlab
==============

Neuroscore's ZDB files are just SQLite 3 databases with an '.zdb' file extension.

SQLite is a software library that implements a self-contained, serverless, zero-configuration, transactional SQL database engine. SQLite is the most widely deployed SQL database engine in the world. The source code for SQLite is in the public domain.

##From the command line

This repository includes a python script ```zdb.py```, which provides the ability to modify ZDB files from the command line. This script can be invoked by:

```
python ./zdb.py [optional arguments]
```
###install

To run the script without calling python every time, run the following command (if you're on a unix-like system):
```
sudo chmod 755 zdb.py
```
This changes the permission of the script to 'executable'.  You can now run it as:

```
./zdb.py [optional arguments]
```

###usage

The full command-line arguments are:

```
./zdb.py [-h] [-zdb ZDBFILE] [-txt TXTFILE]
```
The ```-h``` switch will display a help menu about basic command-line usage to the zdb script.

###adding a sleep score from a TXT

The sleep score column from a TXT file can be merged with a ZDB.  To add a sleep score from a TXT file, use the following syntax when calling ```zdb.py```.

```
./zdb.py -zdb ZDBFILE.zdb -txt TXTFILE 
```
where 'ZDBFILE.zdb' is the name/path of a ZDB file. Likewise 'TXTFILE.txt' is the name of a TXT file.



##from Matlab

This repository provides Matlab wrappers around some basic functions of SQLite 3's C API.  This allows you to execute SQLite queries on an embedded database.


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


