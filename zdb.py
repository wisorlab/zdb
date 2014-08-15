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


import csv
import sqlite3
import contextlib


class ZDB(object):

	"""Neuroscore database manager"""

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

	# convert the ZDB scoring keys to single character values
	@scores.setter
	def scores(self,newscores) :
		c = self.db.cursor()
		for i, char in enumerate(newscores) :
			score = self.convertCharToName(char) 
			c.execute('UPDATE temporary_scoring_marker SET type=\"'+score+'\" WHERE id='+unicode(i+1)+';')
			# self.db.commit()
			
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

	def close(self) :
		self.db.commit()
		self.db.close()

# get the score field from a text file
def getScoreFromText( txt ) :
	with open(txt) as f :
		scores = []
		reader = csv.reader(f,delimiter='\t')
		for cell in reader :
			scores.append(cell[1])

		return scores[2:]


class TimeStamp(object):

	"""docstring for TimeStamp"""

	def __init__( self, zdb_ts=634677840000000000, txt_ts='03/19/2012,20:00:00 PM' ):
		self.zdbts = zdbts;
		


if __name__ == '__main__':

	# zdbfile = 'BA1211_2.zdb'
	zdbfile = 'BA1211_1.zdb'
	txtfile = 'BA-1211human_scored_marked.txt'
	# txtfile = 'BA-1211human_scored_marked.txt'

	zdb = ZDB(zdbfile)
	zdb.open()

	autoscores = getScoreFromText(txtfile)
	# zdbscores = zdb.scores
	# print autoscores
	zdb.scores = autoscores
	# zdb.scores = ['R','R','R']

	# print zdbscores[0:5]

	# print autoscores[0:5]

	# print len(autoscores[2:len(autoscores)])
	# print len(zdb.scores)
	# zdb.scores = autoscores

	zdb.close()
	
