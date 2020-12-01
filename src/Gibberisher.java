// Marco Tabacman
// CSCI 1913 ---- Project 3


/**
 * A class that gets trained and then produces gibberish, but pronounceable, words.
 */
public class Gibberisher {
    private Trie<CharBag> model;
    private int segmentLen;
    private int letterSsProcessed;

    /**
     * A class constructor
     * @param segmentLen an integer defining how long of a segment to train the Trie on
     */
    public Gibberisher(int segmentLen) {
        this.segmentLen = segmentLen;
        this.model = new Trie<>();
        this.letterSsProcessed = 0;
    }

    /**
     * Adds one sample into the model
     * @param letterSample LetterSample to add into the model
     */
    public void train(LetterSample letterSample) {
        String temp = letterSample.getSegment(); // Setting up for less wordiness
        if (temp.equals("")) { // We want to associate this to the root node, so we have a separate case
            if (model.getRoot().getData() == null) {
                model.getRoot().setData(new CharBag()); // Initialize the root node if empty
            }
            model.getRoot().getData().add(letterSample.getNextLetter()); // Adds the letter to the root!
        } else {
            if (model.get(temp) == null) {
                model.set(temp, new CharBag()); // Initialize the TrieNode if data == null
            }
            model.get(temp).add(letterSample.getNextLetter()); // Adds the letter to the associated string
        }
        letterSsProcessed += 1;
    }

    /**
     * Generates LetterSamples, and then uses train(LetterSample) to train the model for each LetterSample
     */
    public void train(String word) {
        LetterSample[] stringPieces = LetterSample.toSamples(word, segmentLen);
        for (int i = 0; i < stringPieces.length; i++) {
            train(stringPieces[i]);
        }
    }

    /**
     * Calls train(String) for each word in an array
     * @param words String[] containing each word to be trained
     */
    public void train(String[] words) {
        for (int i = 0; i < words.length; i++) {
            train(words[i]);
        }
    }

    /**
     * Returns the number of samples used to train the model so far
     * @return An int representing the number of samples used to train the model so far
     */
    public int getSampleCount() { return letterSsProcessed; }

    /**
     * Returns a generated Gibberish word
     * @return A Gibberish word
     */
    public String generate() {
        String word = ""; // Future generated word
        while (word.indexOf(LetterSample.STOP) == -1) { // Checking that our algorithm hasn't told us to stop the word
            String segment = ""; // Contains relevant section of word
            if (word.length() <= this.segmentLen) {
                segment = word; // Only if word is less than or equal to the segment length
            } else {
                for (int i = 0; i < segmentLen; i++) {
                    segment += word.charAt(word.length() - segmentLen + i); // Get the last segmentLen chars of the word and put them in segment
                }
            }
            if (segment.equals("")) {
                word += model.getRoot().getData().getRandomChar(); // Add first randomly generated char
            } else {
                word += model.get(segment).getRandomChar(); // Add next randomly generated char to the end of word
            }

        }

        return word.substring(0, word.length()-1); // Return the word, hold the LetterSample.STOP
    }
}
