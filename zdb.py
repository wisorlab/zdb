#!/usr/bin/python

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# 
# $$$$$$$$\ $$$$$$$\  $$$$$$$\  
# \____$$  |$$  __$$\ $$  __$$\ 
#     $$  / $$ |  $$ |$$ |  $$ |
#    $$  /  $$ |  $$ |$$$$$$$\ |
#   $$  /   $$ |  $$ |$$  __$$\ 
#  $$  /    $$ |  $$ |$$ |  $$ |
# $$$$$$$$\ $$$$$$$  |$$$$$$$  |
# \________|\_______/ \_______/ 
# 
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# USAGE:
# some python functions to set and retrieve scores from a ZDB file
# 
# by @jonbrennecke / github.com/jonbrennecke
# 
# 

import csv, sqlite3, contextlib, sys, argparse, datetime, time, calendar


class ZDB(object):

	"""

	Neuroscore database thingy

	"""

	def __init__(self, zdbfile):
		self.file = zdbfile

	@contextlib.contextmanager
	def open_w_error(self) :
		try:
			db = sqlite3.connect(self.file)
		except IOError, err:
			yield None, err
		else:
			yield db, None

	# open the database
	def open(self):
		with self.open_w_error() as (db,err) :
			if err :
				raise err
			else :
				self.db = db

	# scores property retrieves all the scores from the file 
	@property
	def scores(self):
		if self.db :
			c = self.db.cursor()
			c.execute("SELECT type FROM temporary_scoring_marker;")
			return [ self.convertNameToChar(score) for score in c.fetchall() ]
		else :
			self.open()
			return self.scores

	# convert the scoretime from a TXT file into into .Net ticks 
	# (number of 100 nano second incrememnts since Jan 1, 0001 )
	# 
	# can be reversed by:
	# print (datetime.datetime(1,1,1) + datetime.timedelta(microseconds=ticks/10))
	def convertScoretimeToTicks(self,scoretime) :
		unixepoch = int((datetime.datetime(1970, 1, 1, 0, 0, 0) - datetime.datetime(1, 1, 1, 0, 0, 0)).total_seconds()) * (10 ** 7)
		tup = datetime.datetime.strptime(scoretime, "%m/%d/%Y,%H:%M:%S %p" ).timetuple()
		return int(calendar.timegm(tup) * (10**7)) + unixepoch

	# convert the ZDB scoring keys to single character values
	@scores.setter
	def scores(self,newscores) :
		c = self.db.cursor()
		for i, (time,char) in enumerate(newscores) :
			score = self.convertCharToName(char) 
			ticks = self.convertScoretimeToTicks(newscores[i][0])
			c.execute('UPDATE temporary_scoring_marker SET type=\"'+score+'\" WHERE starts_at='+unicode(ticks)+';')
			# c.execute('UPDATE temporary_scoring_marker SET type=\"'+score+'\" WHERE id='+unicode(i+1)+';')
			
	# takes a single character in ['W','S','R','X'] and converts it to 
	# a corresponding representation in ZDB's string format
	def convertCharToName(self,char) :
		if char == 'W' :
			return u'Sleep-Wake'
		elif char == 'S' :
			return u'Sleep-SWS'
		elif char == 'R' :
			return u'Sleep-Paradoxical'
		elif char == 'X' :
			return u'Sleep-Artifact'
		elif char == '' :
			return u'Sleep-Artifact'
		else:
			raise Warning('key ' + char + ' did not match any known names.')
			return None

	# takes a single character in ZDB's string format and converts it to 
	# a corresponding single-character representation
	def convertNameToChar(self,name) :
		if name == (u'Sleep-Wake',) :
			return 'W'
		elif name == (u'Sleep-SWS',) or name == (u'Sleep-SWS1',) :
			return 'S'
		elif name == (u'Sleep-Paradoxical',) :
			return 'R'
		elif name == (u'Sleep-Artifact',) :
			return 'X'
		else:
			raise Warning('key ' + name + ' did not match any known keys.')
			return None

	# save and close the zdb
	def close(self) :
		self.db.commit()
		self.db.close()
		

# get the timestamp and score field from a text file
def getScoreFromText( txt ) :
	with open(txt) as f :
		scores = []
		reader = csv.reader(f,delimiter='\t')
		for cell in reader :
			scores.append(cell[0:2])

		return scores[2:]

# class Textfile(object):

# 	"""docstring for Textfile"""

# 	def __init__(self, arg):
# 		self.arg = arg

# class TimeStamp(object):

# 	"""docstring for TimeStamp"""

# 	def __init__( self, zdb_ts=634677840000000000, txt_ts='03/19/2012,20:00:00 PM' ):
# 		self.zdbts = zdbts;
		


if __name__ == '__main__':

	# parse arguments
	parser = argparse.ArgumentParser(description='Read and write data to a ZDB file.')
	parser.add_argument('-zdb', dest='zdbfile', type=str, help='the ZDB database file')
	parser.add_argument('-txt', dest='txtfile', type=str, help='the TXT file')
	args = parser.parse_args(sys.argv[1:])

	zdb = ZDB(args.zdbfile)
	zdb.open()

	autoscores = getScoreFromText(args.txtfile)

	zdb.scores = autoscores

	zdb.close()
	
