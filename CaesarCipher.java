// Marco Tabacman
// CSCI 1913 ---- Project 2

/**
 * A class that implements the Caesar Cipher.
 */
public class CaesarCipher extends BaseCipher {

    private int key;

    /**
     * Class constructor.
     * @param key The rotation value this CaesarCipher object uses.
     */
    public CaesarCipher(int key){
        super("CaesarCipher");
        this.key = key;
    }

    /**
     * Checks if this CaesarCipher object has a useful key.
     * @return The key is between 1 and 25.
     */
    @Override
    public boolean isValid() {
        return key > 0 && key < 26;
    }

    /**
     * Helper function that rotates a letter by a given rotation key.
     * @param letter to be rotated
     * @param k Rotation key
     * @return Rotated letter
     */
    private char rotate(char letter, int k) {
        int let = (int) letter;
        let += k;
        if (let > 122) {
            let -= 26;
        } else if (let < 97) {
            let += 26;
        }
        return (char) let;
    }

    /**
     * Encrypts a string using this instance of a caesar cipher.
     * @param mess Plaintext to be encrypted.
     * @return Encrypted text.
     */
    @Override
    public String encrypt(String mess) {
        mess = mess.toLowerCase();
        String retVal = "";
        for (int i = 0; i < mess.length(); i++) {
            char currChar = mess.charAt(i);
            if (currChar > 96 && currChar < 123) {
                currChar = rotate(currChar, this.key);
            }
            retVal += currChar;
        }

        return retVal;
    }

    /**
     * Decrypts a string using this instance of a caesar cipher.
     * @param mess Encrypted string to be decrypted.
     * @return Decrypted string.
     */
    @Override
    public String decrypt(String mess) {
        mess = mess.toLowerCase();
        String retVal = "";
        for (int i = 0; i < mess.length(); i++) {
            char currChar = mess.charAt(i);
            if (currChar > 64 && !(currChar > 90 && currChar < 97) && currChar < 123) {
                currChar = rotate(currChar, -this.key);
            }
            retVal += currChar;
        }

        return retVal;
    }


    /**
     * Returns a string representation of the object. A string that "textually represents" this object.
     * @return A string representation of the object.
     */
    @Override
    public String toString() {
        return "Caesar("+key+")";
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
        } else if (other instanceof CaesarCipher) {
            CaesarCipher caesar = (CaesarCipher) other;
            return this.key == caesar.key;
        } else {
            return false;
        }
    }
}