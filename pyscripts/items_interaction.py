#!/usr/bin/python3.7
# -*- coding: utf-8 -*-
from enum import IntFlag

class InteractionType(IntFlag):
	NoneType = 0x0
	Stack    = 0x1
	Volumes  = 0x2

#функция, вызываемая непосредственно из С++,
# выбирает какой конкретно метод взаимодействия выбрать
def interact(self, other):
	key = self.getInteractType() & other.getInteractType()
	interactFunc = interactFuncsDict.get(key)
	if interactFunc is not None: interactFunc(self, other)

#список функций взаимодействий
def IncrDecr(self, other):
	if other.getState():
		self.setState( self.getState() + 1 )
		other.setState( other.getState() - 1 )

interactFuncsDict = {
						InteractionType.Volumes : IncrDecr
					}