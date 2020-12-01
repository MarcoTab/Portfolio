// Marco Tabacman
// CSCI 1913 ---- Project 3

/**
 * Implements a TrieNode, a node of the tree based ADT Trie.java
 * @param <T> Type of data to store in this TrieNode
 */
public class TrieNode<T> {
    private T data;
    private TrieNode<T>[] children;

    /**
     * Class constructor
     */
    public TrieNode() {
        this.data = null;
        this.children = new TrieNode[26];
    }

    /**
     * Gets data in this TrieNode
     * @return data in this TrieNode
     */
    public T getData() { return this.data; }

    /**
     * Sets data in this TrieNode
     * @param data this TrieNode will represent
     */
    public void setData(T data) { this.data = data;}

    /**
     * Returns the child of this TrieNode associated with given char
     * @param letter target node
     * @return child node associated with target node
     */
    public TrieNode<T> getChild(char letter) {
        if (letter >= 97 && letter <= 122) {
            int index = letter - 97;
            if (children[index] == null) {
                children[index] = new TrieNode<>();
            }
            return children[index];
        } else {
            return null;
        }
    }

    /**
     * Returns the size of the tree with this TrieNode as the root
     * @return an int representing the number of nodes in this tree
     */
    public int getTreeSize() {
        int retVal = 1;
        for (int i = 0; i < children.length; i++) {
            if (children[i] != null) {
                retVal += children[i].getTreeSize();
            }
        }
        return retVal;
    }

}
