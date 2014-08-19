#!/usr/bin/python

import datetime, time, calendar

# convert the scoretime from a TXT file into into .Net ticks 
# (number of 100 nano second incrememnts since Jan 1, 0001 )
def convertScoretimeToTicks(scoretime) :
	unixepoch = int((datetime.datetime(1970, 1, 1, 0, 0, 0) - datetime.datetime(1, 1, 1, 0, 0, 0)).total_seconds()) * (10 ** 7)
	tup = datetime.datetime.strptime(scoretime, "%m/%d/%Y,%H:%M:%S %p" ).timetuple()
	return int(calendar.timegm(tup) * (10**7)) + unixepoch

# convert the scoretime from a TXT file into into .Net ticks 
# (number of 100 nano second incrememnts since Jan 1, 0001 )
def convertTicksToScoretime(ticks) :
	return (datetime.datetime(1,1,1) + datetime.timedelta(microseconds=ticks/10))

# takes a single character in ['W','S','R','X'] and converts it to 
# a corresponding representation in ZDB's string format
def convertCharToName(char) :
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
		