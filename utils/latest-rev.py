#!/usr/bin/env python

from bzrlib import branch
import os

curpath = os.getcwd()

b = branch.Branch.open('file://%s' % curpath)
revision_number, revision_id = b.last_revision_info()

print "Revision:\t%d" % revision_number
print "ID:\t\t%s" % revision_id
