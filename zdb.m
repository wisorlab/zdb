file = './datafiles/test.zdb';

db = sqlite3_open(file);

master = sqlite3_exec(db,'SELECT name, sql FROM sqlite_master WHERE type="table";');

SQL = struct();
for i=1:length(master)
	SQL.(master{i}.name) = struct();
	SQL.(master{i}.name).name = master{i}.name;
	SQL.(master{i}.name).sql = master{i}.sql;
	SQL.(master{i}.name).tables = sqlite3_exec(db,['PRAGMA table_info(' master{i}.name ')']);
end

sqlite3_close(db);



