#!/usr/bin/python
# -*- coding: utf-8 -*-
import os
import sys
import getopt
sys.path.append(os.getcwd())

import Rtree


def lCompoundWord(wList):

    if wList == None or wList == []:
        return None

    rtree = Rtree.Rtree()

    for word in wList:
        rtree.insWord(word)

    wordSuffix = []

    for word in wList:

        prefixes = rtree.findPrefixes(word)

        for prefix in prefixes:

            wordSuffix.append((word, word[len(prefix):]))

    lWord = ''

    maxLen = 0

    while wordSuffix:

        (word, suffix) = wordSuffix.pop(0)

        if rtree.isExist(suffix) and len(word) > maxLen:

            lWord = word

            maxLen = len(word)
        else:

            prefixes = rtree.findPrefixes(suffix)

            for prefix in prefixes:

                wordSuffix.append((word, suffix[len(prefix):]))

    return lWord


def main(argv):

    wordList = []

    with open(str(sys.argv[1]), 'r') as lines:
        for line in lines:
            tmp_line = line.strip()
            if tmp_line != None and len(tmp_line):
                wordList.append(line.strip())

    lCompWord = lCompoundWord(wordList)

    print lCompWord


if __name__ == '__main__':
    main(sys.argv[1:])
