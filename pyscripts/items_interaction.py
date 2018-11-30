#!/usr/bin/python3.7
# -*- coding: utf-8 -*-


def interact(a, b):
	print(a.getInteractType(), b.getInteractType())


def IncDec(self, other):
	if other.getState():
		self.setState( self.getState() + 1 )
		other.setState( other.getState() - 1 )
