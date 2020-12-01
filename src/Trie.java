// Marco Tabacman
// CSCI 1913 ---- Project 3

/**
 * Maintains order of a TrieNode Tree
 * @param <T> Type parameter for this Trie
 */
public class Trie<T> {

    private TrieNode<T> root;

    /**
     * Class constructor
     */
    public Trie() {
        this.root = new TrieNode<>();
    }

    /**
     * Returns the trieNode that represents the last letter of the target string
     * @param target String to find in this TrieNode
     * @return Associated TrieNode
     */
    private TrieNode<T> getNode(String target) {
        char[] string = new char[target.length()];
        for (int i = 0; i < target.length(); i++) {
            string[i] = target.charAt(i);
        }
        TrieNode<T> retVal = this.root;
        for (int i = 0; i < string.length; i++) {
            retVal = retVal.getChild(string[i]);
        }
        return retVal;
    }

    /**
     * Gets the data currently stored on the TrieNode associated with the input string
     * @param target Input string
     * @return Data stored on the associated TrieNode
     */
    public T get(String target) {
        return getNode(target).getData();
    }

    /**
     * Sets the data currently stored on the TrieNode associated with the input string to the T value provided
     * @param target Input string to find associated TrieNode
     * @param data Data to set said TrieNode to
     */
    public void set(String target, T data) { getNode(target).setData(data); }

    /**
     * Used for testing, returns the root node
     * @return Root TrieNode
     */
    public TrieNode<T> getRoot() { return root; }

}
