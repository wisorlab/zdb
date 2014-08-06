file = './datafiles/BA1216 05_07_2012.zdb';
% sql = 'PRAGMA table_info(temporary_scoring_key)';

sql = 'SELECT name, sql FROM sqlite_master WHERE type="table"';

tableinfo = sqlite3_exec(file,sql);

