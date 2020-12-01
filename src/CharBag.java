// Marco Tabacman
// CSCI 1913 ---- Project 3

import java.util.Random;

/**
 * Implements a CharBag, which only contains the 27 letters of the (lowercase) alphabet and a special STOP character.
 */
public class CharBag {

    private int size;
    private int[] reps;
    // We're not storing the chars themselves
    // This'll store how many times 'a' is in the bag at index 0 and so on with 'b' at index 1, etc
    // For any index (except 26), add 97 and get the unicode representation of the char it's counting
    // Index 26 represents LetterSample.STOP

    /**
     * Class constructor
     */
    public CharBag() {
        size = 0;
        reps = new int[27];
        for (int i = 0; i < 27; i++) {
            reps[i] = 0;
        }
    }

    /**
     * Adds a char to the CharBag. Accepts repetition.
     * @param toAdd Char to be added to the bag.
     */
    public void add(char toAdd) {
        if (!Character.isAlphabetic(toAdd)) { // Increment the STOP char
            reps[26]++;
        } else { // Increment whatever other char
            reps[Character.toLowerCase(toAdd) - 97]++;
        }
        size += 1;
    }

    /**
     * Removes one count of a char in the CharBag iff there is an instance of that char in the bag.
     * @param toRem Target char to be removed
     */
    public void remove(char toRem) {
        if (!Character.isAlphabetic(toRem)) {
            if (reps[26] != 0) {
                reps[26] -= 1;
                size -= 1;
            }
        } else {
            if (reps[Character.toLowerCase(toRem) - 97] != 0) {
                reps[Character.toLowerCase(toRem) - 97] -= 1;
                size -= 1;
            }
        }
    }

    /**
     * Returns the amount of times the target char is in the bag
     * @param c Target char
     * @return An int representing number of times target char is in the bag
     */
    public int getCount(char c) {
        if (!Character.isAlphabetic(c)) {
            return reps[26];
        } else {
            return reps[Character.toLowerCase(c)-97];
        }
    }

    /**
     * Returns the total size of the CharBag
     * @return An int representing the size of the CharBag
     */
    public int getSize() { return size; }

    /**
     * A string representation of a CharBag
     * @return A string representation of a CharBag
     */
    @Override
    public String toString() {
        return "CharBag{a:"+reps[0]+", b:"+reps[1]+", c:"+reps[2]+", d:"+reps[3]+", e:"+reps[4]+", f:"+reps[5]+", g:"+reps[6]+", h:"+reps[7]+", i:"+reps[8]+", j:"+reps[9]+", k:"+reps[10]+", l:"+reps[11]+", m:"+reps[12]+", n:"+reps[13]+", o:"+reps[14]+", p:"+reps[15]+", q:"+reps[16]+", r:"+reps[17]+", s:"+reps[18]+", t:"+reps[19]+", u:"+reps[20]+", v:"+reps[21]+", w:"+reps[22]+", x:"+reps[23]+", y:"+reps[24]+", z:"+reps[25]+", .:"+reps[26]+"}";
    }

    /**
     * Returns a random char, weighted with however many times a certain char is in the bag.
     * @return A weighted random char
     */
    public char getRandomChar() {
        int count = 0;
        if (this.getSize() != 0) {
            Random rand = new Random();
            count = rand.nextInt(this.getSize());
        }
        for (char c = 'a'; c <= 'z'; c++) {
            count -= this.getCount(c);
            if (count < 0) {
                return c;
            }
        }
        return LetterSample.STOP;
    }

}
