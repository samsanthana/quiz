package main

import (
	"fmt"
	"os"
	"bufio"
	"strings"
)


type Node struct {
	letter byte
	isLeaf bool
	children []*Node
}
/*Append a child to Trie*/
func (node *Node) addChild(childNode *Node){
	node.children = append(node.children,childNode)
}

/*Set the end of word marker*/
func (node *Node) setisLeaf() {
	node.isLeaf = true
}

/*Check whether end of word*/
func (node *Node) checkIsLeaf()(isLeaf bool) {
	return node.isLeaf
}

/*Set the letter or form trie of each char*/
func (node *Node) setLetter( c byte) {
	node.letter = c
}

/*Find the node in Trie*/
func (node *Node) FindNode(c byte) (fnode *Node, found bool) {
	for _, child := range node.children {
		if child.letter == c {
			return child, true
		}
	}
	return nil, false
}


type Rtree struct {
	root *Node
}

/*Insert the word into the Trie*/
func (r *Rtree) InsertWord(word string) {
	var curr *Node
	var child *Node
	curr = r.root
	cnt := 0

	for ;cnt < len(word); cnt++ {
		child, _  = curr.FindNode(word[cnt])
		if nil != child {
			curr = child
		} else {
			var tempNode *Node
			tempNode = NewNode()
			tempNode.setLetter(word[cnt])
			curr.addChild(tempNode)
			curr = tempNode
		}

	}
	if cnt == len(word) {
		curr.setisLeaf()
	}
}

/*Check whether the word exist already in the Tire*/
func (r *Rtree) isExist(word string) (found bool) {
	var curr *Node
	var tmpNode *Node
	curr = r.root

	cnt :=0

	if len(word) == 0 {
		return false
	}
	for ; curr != nil ; {
		for ; cnt < len(word); cnt++ {
			tmpNode, _ = curr.FindNode(word[cnt])
			if nil == tmpNode {
				return false
			}
			curr = tmpNode
		}
		return curr.checkIsLeaf()
	}
	return false
}

/*Get all the prefixes for the word*/
func (r* Rtree) getAllPrefixes(word string) (prefixes[] string) {
	var tmpNode *Node
	var curr *Node
	cnt := 0
	strlen := 0

	curr = r.root

	for ;cnt < len(word); cnt++ {
		tmpNode, _ = curr.FindNode(word[cnt])
		if nil == tmpNode {
			return prefixes
		}
		curr = tmpNode
		strlen++

		if curr.checkIsLeaf() {
			prefixes = append(prefixes, word[0:strlen])
		}
	}
	return prefixes
}


func NewNode() *Node {
	return &Node{isLeaf: false, children: make([]*Node, 0)}
}

func NewRtree() *Rtree {
	return &Rtree{root: NewNode()}
}

/*Main function to find the longeset compound word*/
func FindLongestCompound(wordlist[] string)(string){
	var prefixes[] string
        var qprefixes[] string

	var rtree *Rtree

	rtree = NewRtree()

	wfile, _ := os.Create("tmp.txt")

	for cnt := range wordlist {
		iword := wordlist[cnt]
		rtree.InsertWord(iword)
	}

	for cnt := range wordlist {
		word := wordlist[cnt]
		prefixes = rtree.getAllPrefixes(word)

		for pcnt:= range prefixes {
			pword := prefixes[pcnt]
			suffix := word[len(pword):len(word)]
			wfile.WriteString(word + " " + suffix + "\n")
		}
	}
	wfile.Close()

	longest := ""
	ofile, _ := os.Open("tmp.txt")

	scanner := bufio.NewScanner(ofile)

	for scanner.Scan() {
		word_str := scanner.Text()

		if len(word_str) == 0 {
			continue
		}

		idx := strings.Index(word_str, " ")
		qword := word_str[0:idx]
		qsuffix := word_str[idx+1:len(word_str)]

		if rtree.isExist(qsuffix) && (len(longest) < len(qword)) {
			longest = qword

		} else {
			qprefixes = rtree.getAllPrefixes(qsuffix)
			if len(qprefixes) == 0 {
				continue
			}
		}
	}
	ofile.Close()
	return longest

}

func main() {
	word_list := make([]string, 1000000)

	file,_ := os.Open("word.txt")
	defer file.Close()

	scanner := bufio.NewScanner(file)
	cnt := 0
	for scanner.Scan() {
		word_list[cnt] = scanner.Text()
		cnt++
	}

	fmt.Println(FindLongestCompound(word_list))
}
