// Marco Tabacman
// CSCI 1913 ---- Project 2

/**
 * A class that allows for the encryption / decryption of an array of strings.
 */
public class EncryptUtils {

    /**
     * Encrypt an array of plaintext strings.
     * @param cipher Cipher to encrypt with.
     * @param plaintextArray Array of strings to be encrypted.
     * @return Array of encrypted strings.
     */
    public static String[] encryptMany(BaseCipher cipher, String[] plaintextArray) {
        String[] encryptedArray = new String[plaintextArray.length];
        for (int i = 0; i < plaintextArray.length; i++) {
            encryptedArray[i] = cipher.encrypt(plaintextArray[i]); // Encrypt every string in the array
        }
        return encryptedArray;
    }

    /**
     * Decrypt an array of encrypted strings.
     * @param cipher Cipher to decrypt with.
     * @param encryptedArray Array of strings to be decrypted.
     * @return Array of decrypted strings.
     */
    public static String[] decryptMany(BaseCipher cipher, String[] encryptedArray) {
        String[] plaintextArray = new String[encryptedArray.length];
        for (int i = 0; i < encryptedArray.length; i++) {
            plaintextArray[i] = cipher.decrypt(encryptedArray[i]); // Decrypt every string in the array
        }
        return plaintextArray;
    }
}
