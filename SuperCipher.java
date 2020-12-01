// Marco Tabacman
// CSCI 1913 ---- Project 2

/**
 * Implements a SuperCipher.
 * A SuperCipher is a chain of ciphers to be applied to a given string.
 */
public class SuperCipher extends BaseCipher {

    private BaseCipher[] cipherArray;

    /**
     * Class constructor
     * @param cipherArray An array of previously declared BaseCipher's
     */
    public SuperCipher(BaseCipher[] cipherArray) {
        super("SuperCipher");
        this.cipherArray = cipherArray;
    }

    /**
     * Checks if this SuperCiphers' sub-ciphers are valid.
     * @return Every cipher object in cipherArray is valid.
     */
    @Override
    public boolean isValid() {
        for (int i = 0; i < cipherArray.length; i++) {
            if (!cipherArray[i].isValid()) {
                return false;
            }
        }
        return true;
    }

    /**
     * Encrypts a string using this instance of a super cipher.
     * @param plaintext Plaintext to be encrypted.
     * @return Encrypted string.
     */
    @Override
    public String encrypt(String plaintext) {

        for (int i = 0; i < cipherArray.length; i++) {
            plaintext = cipherArray[i].encrypt(plaintext);
        }
        return plaintext;
    }

    /**
     * Decryts a string using this instance of a super cipher
     * @param encrypted Encrypted string to be decrypted.
     * @return Decrypted string.
     */
    @Override
    public String decrypt(String encrypted) {
        for (int i = cipherArray.length-1; i >= 0; i--) {
            encrypted = cipherArray[i].decrypt(encrypted);
        }
        return encrypted;
    }

    /**
     * Returns a string representation of the object. A string that "textually represents" this object.
     * @return A string representation of the object.
     */
    @Override
    public String toString() {
        String retVal = "SuperCipher(";
        for (int i = 0; i < cipherArray.length-1; i++) {
            retVal += cipherArray[i].toString() + " | ";
        }
        retVal += cipherArray[cipherArray.length-1].toString();
        return retVal + ")";
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
        } else if (other instanceof SuperCipher) {
            SuperCipher supcipher = (SuperCipher) other;
            if (this.cipherArray.length != supcipher.cipherArray.length) {
                return false;
            } else {
                for (int i = 0; i < supcipher.cipherArray.length; i++) {
                    if (!this.cipherArray[i].equals(supcipher.cipherArray[i])) {
                        return false;
                    }
                }
                return true;
            }
        } else {
            return false;
        }
    }
}
