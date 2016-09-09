#!/usr/bin/python
# -*- coding: utf-8 -*-


class Rtree:

    def __init__(self):

        self.root = Rnode('')

    def insWord(self, word):

        currNode = self.root

        for char in word:

            if char not in currNode.child:

                currNode.child[char] = Rnode(char)

            currNode = currNode.child[char]

        currNode.isWordEnd = True

    def isExist(self, word):

        currNode = self.root

        for char in word:

            if char not in currNode.child:

                return False

            currNode = currNode.child[char]

        return currNode.isWordEnd

    def findPrefixes(self, word):

        prefix = ''

        prefixes = []

        currNode = self.root

        for char in word:

            if char not in currNode.child:

                return prefixes

            currNode = currNode.child[char]

            prefix += char

            if currNode.isWordEnd:

                prefixes.append(prefix)

        return prefixes


class Rnode:

    def __init__(self, char=None, isWordEnd=False):

        self.char = char

        self.child = {}

        self.isWordEnd = isWordEnd
