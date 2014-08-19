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
from zdbutils import *
import csv, sqlite3, contextlib, sys, argparse


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
			return [ convertNameToChar(score) for score in c.fetchall() ]
		else :
			self.open()
			return self.scores

	# convert the ZDB scoring keys to single character values
	@scores.setter
	def scores(self,newscores) :
		c = self.db.cursor()
		for i, (time,char) in enumerate(newscores) :
			score = convertCharToName(char) 
			ticks = convertScoretimeToTicks(newscores[i][0])
			c.execute('UPDATE temporary_scoring_marker SET type=\"'+score+'\" WHERE starts_at='+str(ticks)+';')
			if c.rowcount == 0 :
				print Warning("'UPDATE' failed to set score at timestamp: "+convertTicksToScoretime(ticks))

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
	
