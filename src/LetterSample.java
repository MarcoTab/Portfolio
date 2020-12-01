// Marco Tabacman
// CSCI 1913 ---- Project 3

/**
 * Implements a class that reads a word and creates a string of the same length, splitting the string
 * into specific length substrings
 */
public class LetterSample {

    private String segment;
    private char nextLetter;
    public static final char STOP = '.';

    /**
     * The class constructor
     * @param segment Value for the segment string
     * @param nextLetter The letter that comes after segment
     */
    public LetterSample(String segment, char nextLetter) {
        this.segment = segment;
        this.nextLetter = nextLetter;
    }

    /**
     * Returns the letter right after the segment string
     * @return char nextLetter
     */
    public char getNextLetter() { return nextLetter; }

    /**
     * Returns the segment preceding the next letter
     * @return string segment
     */
    public String getSegment() { return segment; }


    /**
     * A string representation of a LetterSample object
     * @return A string representation of a Letter Sample object
     */
    @Override
    public String toString() { return "\"" + segment + "\" -> " + nextLetter; }


    /**
     * Takes an input string and generates a Letter Sample array containing every String segment determined by segmentSize
     * @param input Input string to convert to LetterSamples
     * @param segmentSize Size of the LetterSamples to be made
     * @return A LetterSample array representing every letter sample the target string contains
     */
    public static LetterSample[] toSamples(String input, int segmentSize) {
        input = input.toLowerCase(); // getting everything nice and low
        String temp = "";
        for (int i = 0; i < input.length(); i++) { // Cleaning up our filthy string
            char current = input.charAt(i);
            if(Character.isAlphabetic(current)) { // #onlyALPHASinthisfrat im sorry for this comment it was very late at the time
                temp += current;
            }
        }

        input = temp + LetterSample.STOP; // Final correct string
        LetterSample[] retVal = new LetterSample[input.length()]; // Initiating return value.

        for (int i = 0; i<input.length(); i++) {
            if (Math.min(i, segmentSize) == i) { // Once i > segmentSize-1, we switch to not including the beginning of the string
                retVal[i] = new LetterSample(input.substring(0,i), input.charAt(i));
            } else {
                retVal[i] = new LetterSample(input.substring(i-segmentSize, i), input.charAt(i));
            }
        }

        return retVal;
    }
}
