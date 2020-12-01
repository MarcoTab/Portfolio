// Marco Tabacman
// CSCI 1913 ---- Project 2

/**
 * A class that implements a version of Even Odd Cipher.
 */
public class EvenOddCipher extends BaseCipher {

    /**
     * Class constructor.
     */
    public EvenOddCipher() {
        super("EvenOddCipher");
    }

    /**
     * Encrypts a string using the even odd cipher.
     * @param plaintext Plaintext to be encrypted.
     * @return Encrypted string.
     */
    @Override
    public String encrypt(String plaintext) {
        String retVal = "";
        plaintext = plaintext.toLowerCase();
        for (int i = 0; i < plaintext.length(); i += 2){
            retVal += plaintext.charAt(i);
        }
        for (int i = 1; i < plaintext.length(); i += 2){
            retVal += plaintext.charAt(i);
        }

        return retVal;
    }

    /**
     * Decrypts a string using the even odd cipher
     * @param encrypted Encrypted string to be decrypted
     * @return Decrypted string.
     */
    @Override
    public String decrypt(String encrypted) {
        String retVal = "";
        encrypted = encrypted.toLowerCase();
        int crossover = (int) Math.ceil(encrypted.length()/2.0)-1;
        int even = 0;
        for (int i = 0; i < encrypted.length(); i++) {
            if(i%2 == 0) {
                retVal += encrypted.charAt(i - even);
                even++;
            } else {
                retVal += encrypted.charAt(crossover + even);
            }
        }

        return retVal;
    }

    /**
     * Returns whether some other object is "equal to" this one.
     * @param other - the reference object with which to compare
     * @return true if this object is the same as the obj argument; false otherwise.
     */
    @Override
    public boolean equals(Object other) {
        return other instanceof EvenOddCipher;
    }
}
