// Marco Tabacman
// CSCI 1913 ---- Project 2

/**
 * A class that implements a Word Replacement Cipher.
 */
public class WordReplacementCipher extends BaseCipher {

    private String target;
    private String codeword;

    /**
     * Class constructor
     * @param target Target word for this WordReplacementCipher.
     * @param codeword Codeword for this WordReplacementCipher.
     */
    public WordReplacementCipher(String target, String codeword) {
        super("WordReplacementCipher");
        this.target = target.toLowerCase();
        this.codeword = codeword.toLowerCase();
    }

    /**
     * Encrypts a string using this instance of the word replacement cipher.
     * @param plaintext Plaintext to be encrypted.
     * @return Encrypted string.
     */
    @Override
    public String encrypt(String plaintext) {
        plaintext = plaintext.toLowerCase();
        return plaintext.replace(target, codeword);
    }

    /**
     * Decrypts a string using this instance of the word replacement cipher.
     * @param encrypted Encrypted string to be decrypted.
     * @return Decrypted string.
     */
    @Override
    public String decrypt(String encrypted) {
        encrypted = encrypted.toLowerCase();
        return encrypted.replace(codeword, target);
    }

    /**
     * Returns a string representation of the object. A string that "textually represents" this object.
     * @return A string representation of the object.
     */
    @Override
    public String toString() {
        return "WordReplacementCipher(" + target + ", " + codeword + ")";
    }

    /**
     * Returns whether some other object is "equal to" this one.
     * @param other - the reference object with which to compare
     * @return true if this object is the same as the obj argument; false otherwise.
     */
    @Override
    public boolean equals(Object other) {
        if (this == other) {
            return true;
        } else if (other == null) {
            return false;
        } else if (other instanceof WordReplacementCipher) {
            WordReplacementCipher replace = (WordReplacementCipher) other;
            return (this.target.equals(replace.target) && this.codeword.equals(replace.codeword));
        } else {
            return false;
        }
    }
}
