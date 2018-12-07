#!/usr/bin/python3.7
# -*- coding: utf-8 -*-
from enum import IntFlag

class InteractionType(IntFlag):
	NoneType     = 0
	Stack        = 1
	Decrement    = 2
	Increment    = 4
	Destructable = 8
	Destroer     = 16
	Volumes      = 32

IT = InteractionType #для сокращения

def check_stackable(self, other):
	if (self.getIndex() == other.getIndex() and
		self.getInteractType() & InteractionType.Stack == InteractionType.Stack):
		return True
	else:
		return False

#функция, вызываемая непосредственно из С++,
# выбирает какой конкретно метод взаимодействия выбрать
def interact(self, other):
	key = self.getInteractType() | other.getInteractType()
	print(self.getInteractType(), other.getInteractType(), key)
	interactFunc = interactFuncsDict.get(key)
	if interactFunc is not None: interactFunc(self, other)

#список функций взаимодействий
def Incr_Decr(self, other):
	if other.getState():
		self.setState( self.getState() + 1 )
		other.setState( other.getState() - 1 )

def Destroy(self, other):
	if other.getInteractType() == InteractionType.Destroer:
		self.setState(-1)

interactFuncsDict = {
						IT.Volumes				 : Incr_Decr,
						IT.Volumes | IT.Destroer : Destroy,
					}