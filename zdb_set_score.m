% ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
% 
% $$$$$$$$\ $$$$$$$\  $$$$$$$\  
% \____$$  |$$  __$$\ $$  __$$\ 
%     $$  / $$ |  $$ |$$ |  $$ |
%    $$  /  $$ |  $$ |$$$$$$$\ |
%   $$  /   $$ |  $$ |$$  __$$\ 
%  $$  /    $$ |  $$ |$$ |  $$ |
% $$$$$$$$\ $$$$$$$  |$$$$$$$  |
% \________|\_______/ \_______/ 
% 
% ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
% USAGE:
% 
% This function sets all the scoring keys in a given 'ZDB' file
% 
function zdb_set_score( file, new_scores )

	% open the SQLite database
	db = build.sqlite3_open( file );

	% conversion lookup tables
	lookup_name = {'Sleep-Wake','Sleep-SWS','Sleep-Paradoxical'};
	lookup_key = {'W','S','R'};

	% select all the scoring keys from the db
	rows = build.sqlite3_exec(db,'SELECT type FROM temporary_scoring_marker;');

	% throw an error if the lengths don't match
	% TODO - we could do something else here, like only write the first n rows
	if ~(length(rows) == length(new_scores))
		error('ZDB Error: the number of scores must match the number of rows in the table.');
	end	

	% update the table
	for i=1:length(rows)

		% convert the one-letter key into Neuroscore's internal naming format
		index = find(cellfun(@(s)(strcmp(new_scores{i},s)),lookup_key)==1);
		score = lookup_name{index};

		% update the scoring keys
		% sqlite3_exec(db,['UPDATE temporary_scoring_marker WHERE id=' i ' SET type="' score '";']);
		build.sqlite3_exec(db,['UPDATE temporary_scoring_marker SET type="' score '" WHERE id=' num2str(i) ';']);
	end

	% close the db
	build.sqlite3_close(db);
end



