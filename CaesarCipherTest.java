public class CaesarCipherTest {
    public static void main(String[] args) {
        // HOW TO USE THESE TESTS:
        // PLEASE UNCOMMENT AS YOU COMPLETE CODE
        // YOU WILL BE UNABLE TO RUN ANY CODE IF YOU UNCOMMENT A CALL TO A FUNCTION YOU HAVE YET TO WRITE

        // This file only contains tests for the additional properties of the CaesarCipher class from Project 2.

        // Tests for equals
        CaesarCipher c1 = new CaesarCipher(1);
        CaesarCipher c2 = new CaesarCipher(2);
        CaesarCipher c3 = new CaesarCipher(1);
        CaesarCipher c4 = new CaesarCipher(2);
        CaesarCipher c5 = new CaesarCipher(3);

        System.out.println(c1.equals(c2)); // false
        System.out.println(c1.equals(c3)); // true
        System.out.println(c2.equals(c4)); // true
        System.out.println(c1.equals(c5)); // false
        System.out.println(c2.equals(c5)); // false

        // Tests that make sure the inheritance structure is as expected.
        System.out.println(c1 instanceof BaseCipher); // true
        // the following code should compile and run correctly.
        BaseCipher bc = c1;
        System.out.println(bc.encrypt("abc-xyz")); // bcd-yza
    }
}
/* correct output:

false
true
true
false
false
true
bcd-yza

 */

