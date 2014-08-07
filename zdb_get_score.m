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
% This function retrieves all the scoring keys from a given 'ZDB' file
% 

function [ scores, rows ] = zdb_get_score( file )

	% open the SQLite database
	db = build.sqlite3_open( file );

	% conversion lookup tables
	lookup_name = {'Sleep-Wake','Sleep-SWS','Sleep-Paradoxical'};
	lookup_key = {'W','S','R'};

	% select all the scoring keys from the DB
	rows = build.sqlite3_exec(db,'SELECT type FROM temporary_scoring_marker;');

	% make a cell to store the outputs
	scores = cell(length(rows),1);

	% look through the results and use the lookup tables to match Neuroscore's names to their one-letter keys
	for i=1:length(rows)
		key = rows{i}.type;
		index = find(cellfun(@(s)(strcmp(key,s)),lookup_name)==1);
		if index, scores{i} = lookup_key{index};
		else scores{i} = 'X';
		end
	end

	% close the db
	build.sqlite3_close(db);
end